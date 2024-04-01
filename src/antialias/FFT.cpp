#include "FFT.hpp"
#include "TF.hpp"

#include "../dsp/dft.hpp"

const unsigned FFT::SAMPLES = 512;
const float FFT::FREQUENCY = 1.0f;

FFT::FFT() {
}

void FFT::process(size_t channels, const float frequency[16], const float velocity[16], std::function<float(float)> υ) {
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
        idle();
        break;
    }

    loops++;
}

void FFT::recompute(ANTIALIAS antialias, float sampleRate) {
    this->antialias = antialias;
    this->sampleRate = sampleRate;
}

void FFT::collect(std::function<float(float)> υ) {
    if (ix < FFT::SAMPLES) {
        phase += FFT::FREQUENCY / FFT::SAMPLES;

        while (phase >= 1.f) {
            phase -= 1.f;
        }

        buffer[ix++] = υ(phase);
    } else {
        state = DFT;
    }
}

void FFT::dft() {
    memmove(real, buffer, FFT::SAMPLES * sizeof(double));
    memset(imag, 0, FFT::SAMPLES * sizeof(double));

    fft_transformRadix2(real, imag, FFT::SAMPLES);

    state = ESTIMATE;
    chx = 0;
    acc.ratio = 0.0;
    acc.velocity = 0.0;

    if (debug) {
        dump();
        debug = false;
    }
}

void FFT::estimate(size_t channels, const float frequency[16], const float velocity[16]) {
    if (chx < channels) {
        double freq = frequency[chx];
        double vel = fabs(velocity[chx]);

        double sum = 0.0;
        double sum20 = 0.0;
        int i20 = round(0.5 + 20000.0 / freq);
        double amplitude[256];

        for (int i = 0; i < 256; i++) {
            amplitude[i] = TF::interpolate(antialias, i * freq) * real[i];
        }

        for (int i = 0; i < 256; i++) {
            sum += amplitude[i] * amplitude[i];
        }

        for (int i = i20; i < 256; i++) {
            sum20 += amplitude[i] * amplitude[i];
        }

        double power = sqrt(sum);
        double power20 = sqrt(sum20);
        double ratio = power20 / power;

        acc.ratio += vel * ratio;
        acc.velocity += vel;

        chx++;
    } else {
        q = (acc.velocity < 0.001) ? 0.0 : (acc.ratio / 0.845) / acc.velocity;
        state = IDLE;
    }
}

void FFT::idle() {
    // FIXME: 0.25*sampleRate
    if (loops > 1.0 * sampleRate) {
        loops = 0;
        ix = 0;
        chx = 0;
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

        fprintf(f, "%-4lu\t%.5f\t%.3f\t%12.5f\t%12.5f\n", i, buffer[i], freq, real[i], imag[i]);
    }

    // for (size_t i = 0; i < FFT_SAMPLES; i += 8) {
    //     fprintf(f, "             %8.5f, %8.5f, %8.5f, %8.5f, %8.5f, %8.5f, %8.5f, %8.5f,\n",
    //             fft.buffer[i],
    //             fft.buffer[i + 1],
    //             fft.buffer[i + 2],
    //             fft.buffer[i + 3],
    //             fft.buffer[i + 4],
    //             fft.buffer[i + 5],
    //             fft.buffer[i + 6],
    //             fft.buffer[i + 7]);
    // }

    fclose(f);
}
