#include <cmath>

#include "asm_lut.hpp"
#include "asm_sn.hpp"

namespace asm_lookup_sincos2d {
    double υ(const SN &sn, uint32_t φ) {
        const uint32_t αʼ = sn.m * φ - sn.ζ.φʼ;

        double x, y;
        sincos_lut_d(αʼ, y, x);

        const double xʼ = sn.ζ.pʼ * x - sn.ζ.qʼ * y + sn.ζ.rʼ;
        const double yʼ = sn.ζ.sʼ * x + sn.ζ.tʼ * y + sn.ζ.uʼ;
        const double r = xʼ * xʼ + yʼ * yʼ + 1e-30f;

        return yʼ / std::sqrt(r);
    }
}; // namespace asm_lookup_sincos2d
