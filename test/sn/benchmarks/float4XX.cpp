#include <cmath>
#include <iostream>

#include <simd/Vector.hpp>
#include <simd/functions.hpp>

using rack::simd::float_4;
using std::cout;

constexpr float TAU = 2.0 * M_PI;

struct Ζ4XX {
    double pʼ;
    double qʼ;
    double rʼ;
    double sʼ;
    double tʼ;
    double uʼ;
    double φ;
};

struct SN4XX {
    SN4XX(float ε, float θ, float A, float δx, float δy, float Φ, float m) {
        this->ε = ε;
        this->θ = θ;
        this->A = A;
        this->δx = δx;
        this->δy = δy;
        this->Φ = Φ;
        this->m = m;

        recompute();
    };

    void recompute() {
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
    };

    double ε;
    double θ;
    double A;
    double δx;
    double δy;
    double Φ;
    double m;

    struct Ζ4XX ζ;
};

int main(int argc, char *argv[]) {
    const float fs = 48000.0; // 48kHz
    const unsigned long samples = 48000 * 60 * 10;
    const float δt = 1 / fs;

    double ε = std::tanh(1.0 * 0.2);
    double θ = 0.0;
    double A = 1.0;
    double δx = 0.4;
    double δy = -0.1;
    double Φ = 120.0 * M_PI / 180.0;
    double m = 1.0;

    struct SN4XX sn(ε, θ, A, δx, δy, Φ, m);

    // clang-format off
    float  notes[16] = {441.7, 491.8, 556.2, 584.9, 661.5, 737.4, 833.1, 877.6, 657.8, 589.1, 742.3, 495.2, 552.7, 827.9, 442.8, 663.1};
    float  α[16]     = {  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0 };
    double vco[16]   = {  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0 };
    // clang-format on

    constexpr int N = 4; // fixed oversampling

    for (size_t sample = 0; sample < samples; sample++) {
        const float_4 P(sn.ζ.pʼ), Q(sn.ζ.qʼ), R(sn.ζ.rʼ);
        const float_4 S(sn.ζ.sʼ), T(sn.ζ.tʼ), U(sn.ζ.uʼ);
        const float_4 lanes(0.f, 1.f, 2.f, 3.f);

        for (size_t ch = 0; ch < 16; ch++) {
            const float δα = sn.m * δt * notes[ch] / N;
            const float_4 α0 = (α[ch] - sn.ζ.φ) + lanes * δα;

            const float_4 x = rack::simd::cos(α0);
            const float_4 y = rack::simd::sin(α0);

            const float_4 xʼ = P * x - Q * y + R;
            const float_4 yʼ = S * x + T * y + U;
            const float_4 r = xʼ * xʼ + yʼ * yʼ + 1e-30f;

            const float_4 out = yʼ / rack::simd::sqrt(r); // exact version first

            vco[ch] += out[0] + out[1] + out[2] + out[3];

            α[ch] += N * δα;
            while (α[ch] >= TAU) {
                α[ch] -= TAU;
            }
        }
    }

    for (size_t ch = 0; ch < 16; ch++) {
        cout << ">> FLOAT4XX: " << ch + 1 << "  " << vco[ch] << "\n";
    }

    cout << "\n\n";

    return 0;
}
