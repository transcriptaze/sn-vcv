#include "../plugin.hpp"
#include "../sn.hpp"

#include "LPF.hpp"

/* Anti-aliasing filter */
LPF::LPF(ANTIALIAS mode, float fs) {
    const IIR iir = lookup(mode, fs);

    for (size_t i = 0; i < 16; i++) {
        lpf[i].setCoefficients(iir.b, iir.a);
    }

    for (size_t i = 0; i < 16; i++) {
        lpf[i].reset();
    }
}

void LPF::reset() {
    for (size_t i = 0; i < 16; i++) {
        lpf[i].reset();
    }
}

void LPF::process(const double *in, double *out, size_t channels) {
    for (size_t i = 0; i < channels; i++) {
        out[i] = lpf[i].process(in[i]);
    }
}

IIR LPF::lookup(ANTIALIAS mode, float fs) {
    switch (mode) {
    case X1F1:
        return coefficients(COEFFICIENTS_12500Hz, fs);

    case X1F2:
        return coefficients(COEFFICIENTS_16kHz, fs);

    case X2F1:
        return coefficients(COEFFICIENTS_16kHz, 2 * fs);

    case X2F2:
        return coefficients(COEFFICIENTS_16kHz, 2 * fs);

    case X4F1:
        return coefficients(COEFFICIENTS_16kHz, 4 * fs);

    case X4F2:
        return coefficients(COEFFICIENTS_16kHz, 4 * fs);

    default:
        return coefficients(COEFFICIENTS_12500Hz, fs);
    }
}
