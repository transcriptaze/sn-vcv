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
    unsigned ix = 0;
    unsigned chx = 0;
    float phase = 0.f;
    double buffer[512] = {0.0};
    double real[512] = {0.0};
    double imag[512] = {0.0};

    struct {
        double ratio;
        double velocity;
    } acc = {
        .ratio = 0.0,
        .velocity = 0.0,
    };

} FFT;
