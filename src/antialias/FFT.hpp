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

    void recompute(ANTIALIAS antialias, float sampleRate);
    void process(size_t channels, const float frequency[16], const float velocity[16], std::function<float(float)> υ);

  private:
    void collect(std::function<float(float)> υ);
    void dft();
    void estimate(size_t channels, const float frequency[16], const float velocity[16]);
    void idle();
    void dump();

  public:
    ANTIALIAS antialias = NONE;
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
