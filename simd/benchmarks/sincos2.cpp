#include <cmath>
#include <iostream>

#include "../sn/sn.hpp"
#include "../sn/unpacked.hpp"

using std::cout;

constexpr float TAU = 2.0 * M_PI;

int main(int argc, char *argv[]) {
    const float fs = 48000.0; // 48kHz
    const unsigned long samples = 48000 * 60 * 10;
    const float δt = 1 / fs;

    float ε = std::tanh(1.0 * 0.2);
    float θ = 0.0;
    float A = 1.0;
    float δx = 0.4;
    float δy = -0.1;
    float Φ = 120.0 * M_PI / 180.0;
    float m = 1.0;

    struct SN sn(ε, θ, A, δx, δy, Φ, m);

    unpacked::recompute(sn);

    // clang-format off
    float  notes[16] = {441.7, 491.8, 556.2, 584.9, 661.5, 737.4, 833.1, 877.6, 657.8, 589.1, 742.3, 495.2, 552.7, 827.9, 442.8, 663.1};
    float  α[16]     = {  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0 };
    double vco[16]   = {  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0 };
    // clang-format on

    constexpr int N = 4; // fixed oversampling

    for (size_t sample = 0; sample < samples; sample++) {
        for (size_t ch = 0; ch < 16; ch++) {
            const float α0 = α[ch] - sn.ζ.φ;
            const float δα = sn.m * δt * notes[ch] / N;

            const float cosδα = std::cos(δα);
            const float sinδα = std::sin(δα);

            const float cos0 = std::cos(α0);
            const float sin0 = std::sin(α0);

            const float cos1 = cos0 * cosδα - sin0 * sinδα;
            const float sin1 = sin0 * cosδα + cos0 * sinδα;

            const float cos2 = cos1 * cosδα - sin1 * sinδα;
            const float sin2 = sin1 * cosδα + cos1 * sinδα;

            const float cos3 = cos2 * cosδα - sin2 * sinδα;
            const float sin3 = sin2 * cosδα + cos2 * sinδα;

            const float _x[N] = {cos0, cos1, cos2, cos3};
            const float _y[N] = {sin0, sin1, sin2, sin3};

            float υ = 0.0f;
            for (size_t n = 0; n < N; n++) {
                float x = _x[n];
                float y = _y[n];

                float xʼ = sn.ζ.pʼ * x - sn.ζ.qʼ * y + sn.ζ.rʼ;
                float yʼ = sn.ζ.sʼ * x + sn.ζ.tʼ * y + sn.ζ.uʼ;
                float r = std::hypot(xʼ, yʼ);

                υ += r > 0.0f ? yʼ / r : 0.0f;
            }

            vco[ch] += υ;
            α[ch] = std::fmod(α[ch] + N * δα, TAU);
        }
    }

    for (size_t ch = 0; ch < 16; ch++) {
        cout << ">> SINCOS2: " << ch + 1 << "  " << vco[ch] << "\n";
    }

    cout << "\n\n";

    return 0;
}
