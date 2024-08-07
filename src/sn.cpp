#include "sn.hpp"

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
    float εʼ = std::sqrt(1.0f - ε * ε);
    float a = (ε < 0.0f) ? εʼ : 1.0f;
    float b = (ε > 0.0f) ? εʼ : 1.0f;

    float cosθ = std::cos(θ);
    float sinθ = std::sin(θ);

    float u = std::atan(-b * std::tan(θ) / a);
    float v = std::atan((b / std::tan(θ)) * a);
    float tx = a * std::cos(u) * cosθ - b * std::sin(u) * sinθ;
    float ty = b * std::sin(v) * cosθ + a * std::cos(v) * sinθ;
    float δxʼ = tx * δx;
    float δyʼ = ty * δy;

    ζ.pʼ = A * a * cosθ;
    ζ.qʼ = A * b * sinθ;
    ζ.rʼ = A * δxʼ;
    ζ.sʼ = A * a * sinθ;
    ζ.tʼ = A * b * cosθ;
    ζ.uʼ = A * δyʼ;
    ζ.φ = phi(a, b, θ, Φ);
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

float SN::phi(float a, float b, float θ, float Φ) {
    float dθ = Φ - θ;
    float φ = std::atan(-(a / b) * std::tan(Φ - θ));

    if (dθ < -PI2) {
        φ += M_PI;
    } else if (dθ > PI2) {
        φ -= M_PI;
    }

    return φ;
}
