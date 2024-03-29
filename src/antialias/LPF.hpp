#pragma once

#include "../plugin.hpp"
#include "../sn.hpp"

#include "IIR.hpp"

struct LPF {
    static IIR lookup(ANTIALIAS mode, float fs);

    LPF(ANTIALIAS mode, float fs);
    void reset();
    void process(const double *in, double *out, size_t channels);

    dsp::IIRFilter<5, 5, double> lpf[16];
};
