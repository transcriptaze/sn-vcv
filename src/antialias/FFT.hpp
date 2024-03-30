#pragma once

typedef struct FFT {
    static const unsigned SAMPLES;
    static const float FREQUENCY;

    FFT();

    void dump();

    unsigned ix = 0;
    float phase = 0.f;
    double buffer[512] = {0.0};
    double real[512] = {0.0};
    double imag[512] = {0.0};
} FFT;
