#include <cmath>
#include <cstdio>

#include "validate.hpp"

// sincos single precision implementation of snʼ.
namespace sincos {
    void recompute(sn &sn) {
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

    sn::sn(float ε, float θ, float A, float δx, float δy, float Φ, float m) {
        this->ε = ε;
        this->θ = θ;
        this->A = A;
        this->δx = δx;
        this->δy = δy;
        this->Φ = Φ;
        this->m = m;

        recompute(*this);
    }

    void sn::υ(float α, float δα, float out[4]) {
        const int N = 4;

        const float δαʼ = δα / N;

        const float cos1 = std::cos(δαʼ);
        const float sin1 = std::sin(δαʼ);

        const float cos2 = cos1 * cos1 - sin1 * sin1;
        const float sin2 = 2.0f * sin1 * cos1;

        const float cos3 = cos2 * cos1 - sin2 * sin1;
        const float sin3 = sin2 * cos1 + cos2 * sin1;

        const float cL[N] = {1.0f, cos1, cos2, cos3};
        const float sL[N] = {0.0f, sin1, sin2, sin3};

        const float α0 = α - ζ.φ;
        const float c0 = std::cos(α0);
        const float s0 = std::sin(α0);

        for (int n = 0; n < N; n++) {
            float x = c0 * cL[n] - s0 * sL[n];
            float y = s0 * cL[n] + c0 * sL[n];

            float xʼ = ζ.pʼ * x - ζ.qʼ * y + ζ.rʼ;
            float yʼ = ζ.sʼ * x + ζ.tʼ * y + ζ.uʼ;
            float r = std::hypot(xʼ, yʼ);

            out[n] = r > 0.0f ? yʼ / r : 0.0f;
        }
    }

} // namespace sincos
