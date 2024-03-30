#pragma once

#include <functional>

#include "../sn.hpp"

typedef struct FFT {
    static const unsigned SAMPLES;
    static const float FREQUENCY;

    FFT();

    void process(const ANTIALIAS antialias, const float sampleRate, size_t channels, const float frequency[16], std::function<float(float)> υ);
    void dump();

    unsigned ix = 0;
    float phase = 0.f;
    double buffer[512] = {0.0};
    double real[512] = {0.0};
    double imag[512] = {0.0};
    float aliasing = 0.f;
} FFT;
