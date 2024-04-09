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
    static FFT_RATE int2rate(int v, FFT_RATE defval);

    FFT();

    void recompute(ANTIALIAS antialias, float sampleRate);
    void process(size_t channels, const float frequency[16], const float velocity[16], FFT_RATE rate, std::function<float(float)> υ);

  private:
    void collect(std::function<float(float)> υ);
    void dft();
    void estimate(size_t channels, const float frequency[16], const float velocity[16]);
    void idle(FFT_RATE rate);
    void dump();

  public:
    ANTIALIAS antialias = ANTIALIAS::NONE;
    float sampleRate = 44100.f;
    float q = 0.f;
    bool debug = false;

  private:
    STATE state = COLLECT;
    unsigned loops = 0;

    struct {
        unsigned ix;
        float phase;
        double samples[512];
    } buffer = {
        .ix = 0,
        .phase = 0.f,
        .samples = {0.0},
    };

    struct {
        double real[512];
        double imag[512];
    } fft = {
        .real = {0.0},
        .imag = {0.0},
    };

    struct {
        unsigned ch;
        double ratio;
        double velocity;
    } acc = {
        .ch = 0,
        .ratio = 0.0,
        .velocity = 0.0,
    };

} FFT;
