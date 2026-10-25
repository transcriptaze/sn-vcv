#include "sn.hpp"

using rack::simd::float_4;

// Control rates
const std::vector<unsigned> KRATE = {1, 64, 128, 256};                   // k-rates
const std::vector<std::string> KRATES = {"1", "¹⁄₆₄", "¹⁄₁₂₈", "¹⁄₂₅₆"}; // k-rate labels

// AUX port
const float AUX_FREQUENCY = 25.0;                                     // AUX output fixed frequency (Hz)
const std::vector<float> AUX_GAIN = {1.f, 0.5f, 0.333f, 0.25f, 0.2f}; // AUX output gain values
const std::vector<std::string> AUX_MODES = {"OSC", "SUM", "POLY"};    // AUX mode labels
const std::vector<std::string> AUX_GAINS = {"1", "½", "⅓", "¼", "⅕"}; // AUX gain labels

// anti-aliasing menu options
const std::vector<std::string> ANTIALIASING = {
    "None",
    "12.5kHz LPF",
    "2x 16kHz LPF",
    "2x oversampling, 16kHz LPF",
    "2x oversampling, 2x 16kHz LPF",
    "4x oversampling, 16kHz LPF",
    "4x oversampling, 2x 16kHz LPF",
};

// DC blocking menu options
const std::vector<std::string> DCBLOCKING = {
    "Off",
    "Slow",
    "Medium",
    "Fast",
};

// aliasing widget update rates
const std::vector<std::string> ALIASING = {
    "Off",
    "100ms",
    "200ms",
    "500ms",
};

// SN
void SN::recompute() {
    const float εʼ = std::sqrt(1.0f - ε * ε);
    const float a = (ε < 0.0f) ? εʼ : 1.0f;
    const float b = (ε > 0.0f) ? εʼ : 1.0f;

    const float cosθ = std::cos(θ);
    const float sinθ = std::sin(θ);
    const float tanθ = std::tan(θ);

    const float u = std::atan2(-b * tanθ, a);
    const float v = std::atan2(a * b, tanθ);

    const float tx = a * std::cos(u) * cosθ - b * std::sin(u) * sinθ;
    const float ty = b * std::sin(v) * cosθ + a * std::cos(v) * sinθ;
    const float δxʼ = tx * δx;
    const float δyʼ = ty * δy;
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

float SN::υ(float α) {
    float αʼ = m * α - ζ.φ;

    float x = std::cos(αʼ);
    float y = std::sin(αʼ);
    float xʼ = ζ.pʼ * x - ζ.qʼ * y + ζ.rʼ;
    float yʼ = ζ.sʼ * x + ζ.tʼ * y + ζ.uʼ;
    float r = std::hypot(xʼ, yʼ);

    return r > 0.0f ? yʼ / r : 0.0f;
}

float_4 SN::υ4(float α, float δα) {
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
