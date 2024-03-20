#pragma once

#include "../plugin.hpp"
#include "../sn.hpp"

#include "butterworth.h"

struct LPF {
    static IIR lookup(ANTIALIAS mode, float fs);

    LPF(ANTIALIAS mode, float fs);
    void reset();
    void process(const double *in, double *out, size_t channels);

    dsp::IIRFilter<5, 5, double> lpf[16];
};

typedef struct AA {
    static int oversampling(ANTIALIAS mode);
    static ANTIALIAS int2mode(int v, ANTIALIAS defval);
    static double interpolate(ANTIALIAS mode, double f);

    AA();

    void reset();
    void recompute(float fs);
    void process(ANTIALIAS mode, const double in[4][16], double out[16], size_t channels);

    float fs;
    ANTIALIAS mode;

    LPF x1f1;
    LPF x1f2[2];
    LPF x2f1;
    LPF x2f2[2];
    LPF x4f1;
    LPF x4f2[2];
} AA;
