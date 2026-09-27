#include "reference.hpp"

namespace reference {
const float PI2 = M_PI / 2; // π/2

float phi(float a, float b, float θ, float Φ) {

    float dθ = Φ - θ;
    float φ = std::atan(-(a / b) * std::tan(Φ - θ));

    if (dθ < -PI2) {
        φ += M_PI;
    } else if (dθ > PI2) {
        φ -= M_PI;
    }

    return φ;
}

void recompute(SN &sn) {
    float εʼ = std::sqrt(1.0f - sn.ε * sn.ε);
    float a = (sn.ε < 0.0f) ? εʼ : 1.0f;
    float b = (sn.ε > 0.0f) ? εʼ : 1.0f;

    float cosθ = std::cos(sn.θ);
    float sinθ = std::sin(sn.θ);

    float u = std::atan(-b * std::tan(sn.θ) / a);
    float v = std::atan((b / std::tan(sn.θ)) * a);
    float tx = a * std::cos(u) * cosθ - b * std::sin(u) * sinθ;
    float ty = b * std::sin(v) * cosθ + a * std::cos(v) * sinθ;
    float δxʼ = tx * sn.δx;
    float δyʼ = ty * sn.δy;

    sn.ζ.pʼ = sn.A * a * cosθ;
    sn.ζ.qʼ = sn.A * b * sinθ;
    sn.ζ.rʼ = sn.A * δxʼ;
    sn.ζ.sʼ = sn.A * a * sinθ;
    sn.ζ.tʼ = sn.A * b * cosθ;
    sn.ζ.uʼ = sn.A * δyʼ;
    sn.ζ.φ = phi(a, b, sn.θ, sn.Φ);
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

} // namespace reference
