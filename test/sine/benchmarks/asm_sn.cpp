#include <cmath>

#include "asm_lut.hpp"
#include "asm_sn.hpp"

namespace asm_lookup_sincos2 {
    void recompute(struct SN &sn) {
        const double εʼ = std::sqrt(1.0f - sn.ε * sn.ε);
        const double a = (sn.ε < 0.0f) ? εʼ : 1.0f;
        const double b = (sn.ε > 0.0f) ? εʼ : 1.0f;

        const double cosθ = std::cos(sn.θ);
        const double sinθ = std::sin(sn.θ);
        const double tanθ = std::tan(sn.θ);

        const double u = std::atan2(-b * tanθ, a);
        const double v = std::atan2(a * b, tanθ);

        const double tx = a * std::cos(u) * cosθ - b * std::sin(u) * sinθ;
        const double ty = b * std::sin(v) * cosθ + a * std::cos(v) * sinθ;
        const double δxʼ = tx * sn.δx;
        const double δyʼ = ty * sn.δy;
        const double δθ = sn.Φ - sn.θ;

        sn.ζ.pʼ = sn.A * a * cosθ;
        sn.ζ.qʼ = sn.A * b * sinθ;
        sn.ζ.rʼ = sn.A * δxʼ;
        sn.ζ.sʼ = sn.A * a * sinθ;
        sn.ζ.tʼ = sn.A * b * cosθ;
        sn.ζ.uʼ = sn.A * δyʼ;
        sn.ζ.φ = std::atan2(-std::sin(δθ) * a, std::cos(δθ) * b);

        const double φʼ = sn.ζ.φ / (2.0 * M_PI);

        sn.ζ.φʼ = static_cast<uint32_t>(static_cast<int64_t>(φʼ * 4294967296.0));
    }
}; // namespace asm_lookup_sincos2

namespace asm_lookup_sincos2f {
    void recompute(struct SN &sn) {
        const double εʼ = std::sqrt(1.0f - sn.ε * sn.ε);
        const double a = (sn.ε < 0.0f) ? εʼ : 1.0f;
        const double b = (sn.ε > 0.0f) ? εʼ : 1.0f;

        const double cosθ = std::cos(sn.θ);
        const double sinθ = std::sin(sn.θ);
        const double tanθ = std::tan(sn.θ);

        const double u = std::atan2(-b * tanθ, a);
        const double v = std::atan2(a * b, tanθ);

        const double tx = a * std::cos(u) * cosθ - b * std::sin(u) * sinθ;
        const double ty = b * std::sin(v) * cosθ + a * std::cos(v) * sinθ;
        const double δxʼ = tx * sn.δx;
        const double δyʼ = ty * sn.δy;
        const double δθ = sn.Φ - sn.θ;

        sn.ζ.pʼ = static_cast<float>(sn.A * a * cosθ);
        sn.ζ.qʼ = static_cast<float>(sn.A * b * sinθ);
        sn.ζ.rʼ = static_cast<float>(sn.A * δxʼ);
        sn.ζ.sʼ = static_cast<float>(sn.A * a * sinθ);
        sn.ζ.tʼ = static_cast<float>(sn.A * b * cosθ);
        sn.ζ.uʼ = static_cast<float>(sn.A * δyʼ);
        sn.ζ.φ = static_cast<float>(std::atan2(-std::sin(δθ) * a, std::cos(δθ) * b));

        const double φʼ = sn.ζ.φ / (2.0 * M_PI);

        sn.ζ.φʼ = static_cast<uint32_t>(static_cast<int64_t>(φʼ * 4294967296.0));
    }
}; // namespace asm_lookup_sincos2f

namespace asm_lookup_sincos2d {
    void recompute(SN &sn) {
        const double εʼ = std::sqrt(1.0f - sn.ε * sn.ε);
        const double a = (sn.ε < 0.0f) ? εʼ : 1.0f;
        const double b = (sn.ε > 0.0f) ? εʼ : 1.0f;

        const double cosθ = std::cos(sn.θ);
        const double sinθ = std::sin(sn.θ);
        const double tanθ = std::tan(sn.θ);

        const double u = std::atan2(-b * tanθ, a);
        const double v = std::atan2(a * b, tanθ);

        const double tx = a * std::cos(u) * cosθ - b * std::sin(u) * sinθ;
        const double ty = b * std::sin(v) * cosθ + a * std::cos(v) * sinθ;
        const double δxʼ = tx * sn.δx;
        const double δyʼ = ty * sn.δy;
        const double δθ = sn.Φ - sn.θ;

        sn.ζ.pʼ = sn.A * a * cosθ;
        sn.ζ.qʼ = sn.A * b * sinθ;
        sn.ζ.rʼ = sn.A * δxʼ;
        sn.ζ.sʼ = sn.A * a * sinθ;
        sn.ζ.tʼ = sn.A * b * cosθ;
        sn.ζ.uʼ = sn.A * δyʼ;
        sn.ζ.φ = std::atan2(-std::sin(δθ) * a, std::cos(δθ) * b);

        const double φʼ = sn.ζ.φ / (2.0 * M_PI);

        sn.ζ.φʼ = static_cast<uint32_t>(static_cast<int64_t>(φʼ * 4294967296.0));
    }
}; // namespace asm_lookup_sincos2d
