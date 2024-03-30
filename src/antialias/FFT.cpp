#include "../plugin.hpp"

#include "FFT.hpp"

const unsigned FFT::SAMPLES = 512;
const float FFT::FREQUENCY = 1.0f;

FFT::FFT() {
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
