#include "../plugin.hpp"
#include "../sn.hpp"

#include "LPF.hpp"

const double LPF::Δf = 344.53125;

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

double LPF::interpolate(ANTIALIAS mode, const double f) {
    double f0;
    int ix;
    int jx;

    switch (mode) {
    case NONE:
        return 1.0;

    case X1F1:
        f0 = std::fmod(f, 44100.0);
        ix = std::floor(f0 / Δf);
        jx = std::ceil(f0 / Δf);

        if (ix == jx) {
            return std::get<1>(TF_12500Hz[ix]);
        } else {
            const auto u = TF_12500Hz[ix];
            const auto v = TF_12500Hz[jx];

            const double f1 = std::get<0>(u);
            const double f2 = std::get<0>(v);
            const double p = std::get<1>(u);
            const double q = std::get<1>(v);

            return p + (q - p) * (f0 - f1) / (f2 - f1);
        }
        break;

    case X1F2:
        f0 = std::fmod(f, 44100.0);
        ix = std::floor(f0 / Δf);
        jx = std::ceil(f0 / Δf);

        if (ix == jx) {
            const double h = std::get<1>(TF_16kHz[ix]);

            return h * h;
        } else {
            const auto u = TF_16kHz[ix];
            const auto v = TF_16kHz[jx];

            const double f1 = std::get<0>(u);
            const double f2 = std::get<0>(v);
            const double p = std::get<1>(u);
            const double q = std::get<1>(v);
            const double h = p + (q - p) * (f0 - f1) / (f2 - f1);

            return h * h;
        }
        break;

    default:
        return 1.0;
    }
}
