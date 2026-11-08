#include "sn_double.hpp"

using rack::simd::float_4;

void SN_DOUBLE::recompute() {
    const double εʼ = std::sqrt(1.0f - ε * ε);
    const double a = (ε < 0.0f) ? εʼ : 1.0f;
    const double b = (ε > 0.0f) ? εʼ : 1.0f;

    const double cosθ = std::cos(θ);
    const double sinθ = std::sin(θ);
    const double tanθ = std::tan(θ);

    const double u = std::atan2(-b * tanθ, a);
    const double v = std::atan2(a * b, tanθ);

    const double tx = a * std::cos(u) * cosθ - b * std::sin(u) * sinθ;
    const double ty = b * std::sin(v) * cosθ + a * std::cos(v) * sinθ;
    const double δxʼ = tx * δx;
    const double δyʼ = ty * δy;
    const double δθ = Φ - θ;

    ζ.pʼ = A * a * cosθ;
    ζ.qʼ = A * b * sinθ;
    ζ.rʼ = A * δxʼ;
    ζ.sʼ = A * a * sinθ;
    ζ.tʼ = A * b * cosθ;
    ζ.uʼ = A * δyʼ;
    ζ.φ = std::atan2(-std::sin(δθ) * a, std::cos(δθ) * b);

    P = float_4(ζ.pʼ);
    Q = float_4(ζ.qʼ);
    R = float_4(ζ.rʼ);
    S = float_4(ζ.sʼ);
    T = float_4(ζ.tʼ);
    U = float_4(ζ.uʼ);
}

float SN_DOUBLE::υ(float α) {
    float αʼ = m * α - ζ.φ;

    float x = std::cos(αʼ);
    float y = std::sin(αʼ);
    float xʼ = ζ.pʼ * x - ζ.qʼ * y + ζ.rʼ;
    float yʼ = ζ.sʼ * x + ζ.tʼ * y + ζ.uʼ;
    float r = std::hypot(xʼ, yʼ);

    return r > 0.0f ? yʼ / r : 0.0f;
}

float_4 SN_DOUBLE::υ4(float α, float δα) {
    const float_4 lanes(0.f, 1.f, 2.f, 3.f);
    const float δαʼ = δα / 4;
    const float_4 α0 = (α - ζ.φ) + lanes * δαʼ;

    const float_4 x = rack::simd::cos(α0);
    const float_4 y = rack::simd::sin(α0);

    const float_4 xʼ = P * x - Q * y + R;
    const float_4 yʼ = S * x + T * y + U;
    const float_4 r = xʼ * xʼ + yʼ * yʼ + 1e-30f;

    return yʼ / rack::simd::sqrt(r);
}
