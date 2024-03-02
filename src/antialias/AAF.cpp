#include "../butterworth.h"
#include "../plugin.hpp"
#include "../sn.hpp"

#include "AAF.hpp"

IIR lookup(ANTIALIAS mode, float fs);

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
        x1f2[0].reset();
        x1f2[1].reset();
        break;

    case X2F1:
        x2f1.reset();
        break;

    case X2F2:
    case X4F1:
    case X4F2:
        break;
    }
}

int AA::oversampling(ANTIALIAS mode) {
    switch (mode) {
    case NONE:
    case X1F1:
    case X1F2:
        return 1;

    case X2F1:
    case X2F2:
        return 2;

    case X4F1:
    case X4F2:
        return 4;
    }
}

void AA::process(ANTIALIAS mode, const double in[2][16], double out[2][16], size_t channels) {
    double intermediate[16];

    if (mode != this->mode) {
        this->mode = mode;
        reset();
    }

    switch (mode) {
    case X1F1:
        x1f1.process(in[0], out[0], channels);
        break;

    case X1F2:
        x1f2[0].process(in[0], intermediate, channels);
        x1f2[1].process(intermediate, out[0], channels);
        break;

    case X2F1:
        x2f1.process(in[0], out[0], channels);
        x2f1.process(in[1], out[1], channels);
        break;

    default:
        for (size_t i = 0; i < channels; i++) {
            out[0][i] = in[0][i];
        }
    }
}

IIR lookup(ANTIALIAS mode, float fs) {
    switch (mode) {
    case X1F1:
        return coefficients(COEFFICIENTS_12500Hz, fs);

    case X1F2:
        return coefficients(COEFFICIENTS_16kHz, fs);

    case X2F1:
        return coefficients(COEFFICIENTS_16kHz, 2 * fs);

    default:
        return coefficients(COEFFICIENTS_12500Hz, fs);
    }
}
