#include "FFT.hpp"
#include "TF.hpp"

#include "../dsp/dft.hpp"

const unsigned FFT::SAMPLES = 512;
const float FFT::FREQUENCY = 1.0f;

FFT::FFT() {
}

void FFT::process(size_t channels, const float frequency[16], const float velocity[16], FFT_RATE rate, std::function<float(float)> υ) {
    if (rate == FFT_RATE::OFF) {
        return;
    } else {
        switch (state) {
        case COLLECT:
            collect(υ);
            break;

        case DFT:
            dft();
            break;

        case ESTIMATE:
            estimate(channels, frequency, velocity);
            break;

        case IDLE:
            idle(rate);
            break;
        }

        loops++;
    }
}

void FFT::recompute(ANTIALIAS antialias, float sampleRate) {
    this->antialias = antialias;
    this->sampleRate = sampleRate;
}

void FFT::collect(std::function<float(float)> υ) {
    if (buffer.ix < FFT::SAMPLES) {
        buffer.phase += FFT::FREQUENCY / FFT::SAMPLES;

        while (buffer.phase >= 1.f) {
            buffer.phase -= 1.f;
        }

        buffer.samples[buffer.ix++] = υ(buffer.phase);
    } else {
        state = DFT;
    }
}

void FFT::dft() {
    memmove(fft.real, buffer.samples, FFT::SAMPLES * sizeof(double));
    memset(fft.imag, 0, FFT::SAMPLES * sizeof(double));

    fft_transformRadix2(fft.real, fft.imag, FFT::SAMPLES);

    state = ESTIMATE;
    acc.ch = 0;
    acc.ratio = 0.0;
    acc.velocity = 0.0;

    if (debug) {
        dump();
        debug = false;
    }
}

void FFT::estimate(size_t channels, const float frequency[16], const float velocity[16]) {
    if (acc.ch < channels) {
        double freq = frequency[acc.ch];
        double vel = fabs(velocity[acc.ch]);

        double sum = 0.0;
        double sum20 = 0.0;
        int i20 = round(0.5 + 20000.0 / freq);
        double amplitude[256];

        for (int i = 0; i < 256; i++) {
            amplitude[i] = TF::interpolate(antialias, i * freq) * fft.real[i];
        }

        for (int i = 0; i < 256; i++) {
            sum += amplitude[i] * amplitude[i];
        }

        for (int i = i20; i < 256; i++) {
            sum20 += amplitude[i] * amplitude[i];
        }

        double power = sqrt(sum);
        double power20 = sqrt(sum20);
        double ratio = power > 0.001 ? power20 / power : 0.0;

        acc.ratio += vel * ratio;
        acc.velocity += vel;

        acc.ch++;
    } else {
        q = (acc.velocity < 0.001) ? 0.0 : (acc.ratio / 0.845) / acc.velocity;
        state = IDLE;
    }
}

void FFT::idle(FFT_RATE rate) {
    unsigned N = 100 * sampleRate / 1000;

    switch (rate) {
    case FFT_RATE::OFF:
        N = 500 * sampleRate / 1000;
        break;

    case FFT_RATE::FAST:
        N = 100 * sampleRate / 1000;
        break;

    case FFT_RATE::MEDIUM:
        N = 200 * sampleRate / 1000;
        break;

    case FFT_RATE::SLOW:
        N = 500 * sampleRate / 1000;
        break;
    }

    if (loops > N) {
        loops = 0;

        buffer.ix = 0;
        buffer.phase = 0.f;

        acc.ch = 0;
        acc.ratio = 0.0;
        acc.velocity = 0.0;

        state = COLLECT;
    }
}

void FFT::dump() {
    const double fs = (double)(FFT::SAMPLES) / FFT::FREQUENCY;
    FILE *f = fopen("/tmp/sn-vco-fft.tsv", "wt");

    fprintf(f, "i\tsn.υ\tf\treal\timaginary\n");

    for (size_t i = 0; i < FFT::SAMPLES; i++) {
        const double freq = i * fs / FFT::SAMPLES;

        fprintf(f, "%-4lu\t%.5f\t%.3f\t%12.5f\t%12.5f\n", i, buffer.samples[i], freq, fft.real[i], fft.imag[i]);
    }

    fclose(f);
}

FFT_RATE FFT::int2rate(int v, FFT_RATE defval) {
    switch (v) {
    case FFT_RATE::OFF:
        return FFT_RATE::OFF;
    case FFT_RATE::FAST:
        return FFT_RATE::FAST;
    case FFT_RATE::MEDIUM:
        return FFT_RATE::MEDIUM;
    case FFT_RATE::SLOW:
        return FFT_RATE::SLOW;
    default:
        return defval;
    }
}
