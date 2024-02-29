#pragma once

#include "../butterworth.h"
#include "../plugin.hpp"
#include "../sn.hpp"

/* Anti-aliasing filter */

template <ANTIALIAS MODE>
struct AAF {
    AAF(float fs) {
        const IIR iir = lookup(fs);

        for (size_t i = 0; i < 16; i++) {
            lpf[i].setCoefficients(iir.b, iir.a);
        }

        for (size_t i = 0; i < 16; i++) {
            lpf[i].reset();
        }
    }

    void process(const double *in, double *out, size_t channels) {
        for (size_t i = 0; i < channels; i++) {
            out[i] = lpf[i].process(in[i]);
        }
    }

    IIR lookup(float fs) {
        switch (MODE) {
        case X1F1:
            return coefficients(COEFFICIENTS_12500Hz, fs);

        default:
            return coefficients(COEFFICIENTS_12500Hz, fs);
        }
    }

    dsp::IIRFilter<5, 5, double> lpf[16];
};
