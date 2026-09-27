#include "riffles.hpp"

// sn implementation with improvements from 'riffles'
namespace riffles {

void recompute(SN &sn) {
    const float εʼ = std::sqrt(1.0f - sn.ε * sn.ε);
    const float a = (sn.ε < 0.0f) ? εʼ : 1.0f;
    const float b = (sn.ε > 0.0f) ? εʼ : 1.0f;

    const float cosθ = std::cos(sn.θ);
    const float sinθ = std::sin(sn.θ);
    const float tanθ = std::tan(sn.θ);

    const float u = std::atan2(-b * tanθ, a);
    const float v = std::atan2(a * b, tanθ);

    const float tx = a * std::cos(u) * cosθ - b * std::sin(u) * sinθ;
    const float ty = b * std::sin(v) * cosθ + a * std::cos(v) * sinθ;
    const float δxʼ = tx * sn.δx;
    const float δyʼ = ty * sn.δy;
    const float δθ = sn.Φ - sn.θ;

    sn.ζ.pʼ = sn.A * a * cosθ;
    sn.ζ.qʼ = sn.A * b * sinθ;
    sn.ζ.rʼ = sn.A * δxʼ;
    sn.ζ.sʼ = sn.A * a * sinθ;
    sn.ζ.tʼ = sn.A * b * cosθ;
    sn.ζ.uʼ = sn.A * δyʼ;
    sn.ζ.φ = std::atan2(-std::sin(δθ) * a, std::cos(δθ) * b);
}

float υ(const SN &sn, float α) {
    float αʼ = sn.m * α - sn.ζ.φ;

    float x = std::cos(αʼ);
    float y = std::sin(αʼ);
    float xʼ = sn.ζ.pʼ * x - sn.ζ.qʼ * y + sn.ζ.rʼ;
    float yʼ = sn.ζ.sʼ * x + sn.ζ.tʼ * y + sn.ζ.uʼ;
    float r = std::hypot(xʼ, yʼ);

    return r > 0.0f ? yʼ / r : 0.0f;
}

} // namespace riffles
