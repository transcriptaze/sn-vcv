#pragma once

#include "../plugin.hpp"
#include "../sn.hpp"

#include "butterworth.h"

struct LPF {
    static const double Δf;

    static IIR lookup(ANTIALIAS mode, float fs);
    static double interpolate(ANTIALIAS mode, double f);

    LPF(ANTIALIAS mode, float fs);
    void reset();
    void process(const double *in, double *out, size_t channels);

    dsp::IIRFilter<5, 5, double> lpf[16];
};
