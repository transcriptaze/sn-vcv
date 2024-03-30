#pragma once

#include <functional>

#include "../sn.hpp"

typedef struct FFT {
    enum STATE {
        COLLECT,
        DFT,
        ESTIMATE,
        IDLE,
    };

    static const unsigned SAMPLES;
    static const float FREQUENCY;

    FFT();

    void process(const ANTIALIAS antialias, const float sampleRate, size_t channels, const float frequency[16], std::function<float(float)> υ);

  private:
    void collect(std::function<float(float)> υ);
    void dft();
    void estimate(ANTIALIAS antialias, const float frequency[16]);
    void idle(const float sampleRate);
    void dump();

  public:
    float q = 0.f;
    bool debug = false;

  private:
    STATE state = COLLECT;
    unsigned ix = 0;
    float phase = 0.f;
    double buffer[512] = {0.0};
    double real[512] = {0.0};
    double imag[512] = {0.0};

} FFT;
