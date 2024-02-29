#include "../butterworth.h"
#include "../plugin.hpp"
#include "../sn.hpp"

#include "AAF.hpp"

IIR lookup(ANTIALIAS mode, float fs) {
    switch (mode) {
    case X1F1:
        return coefficients(COEFFICIENTS_12500Hz, fs);

    default:
        return coefficients(COEFFICIENTS_12500Hz, fs);
    }
}

/* Anti-aliasing filter */
AAF::AAF(ANTIALIAS mode, float fs) {
    const IIR iir = lookup(mode, fs);

    for (size_t i = 0; i < 16; i++) {
        lpf[i].setCoefficients(iir.b, iir.a);
    }

    for (size_t i = 0; i < 16; i++) {
        lpf[i].reset();
    }
}

void AAF::reset() {
    for (size_t i = 0; i < 16; i++) {
        lpf[i].reset();
    }
}

void AAF::process(const double *in, double *out, size_t channels) {
    for (size_t i = 0; i < channels; i++) {
        out[i] = lpf[i].process(in[i]);
    }
}

/* Anti-aliasing processor */
void AA::reset() {
    switch (mode) {
    case NONE:
        break;

    case X1F1:
        x1f1.reset();
        break;

    case X1F2:
    case X2F1:
    case X2F2:
    case X4F1:
    case X4F2:
        break;
    }
}

void AA::process(ANTIALIAS mode, const double *in, double *out, size_t channels) {
    if (mode != this->mode) {
        this->mode = mode;
        reset();
    }

    switch (mode) {
    case X1F1:
        x1f1.process(in, out, channels);
        break;

    default:
        for (size_t i = 0; i < channels; i++) {
            out[i] = in[i];
        }
    }
}
