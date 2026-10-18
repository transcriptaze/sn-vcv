#include <cmath>
#include <cstdio>

#include <simd/Vector.hpp>
#include <simd/functions.hpp>

#include "validate.hpp"

using rack::simd::float_4;

// SIMD implementation of snʼ.
namespace float4A {
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
        constexpr int N = 4;

        const float_4 P(ζ.pʼ), Q(ζ.qʼ), R(ζ.rʼ);
        const float_4 S(ζ.sʼ), T(ζ.tʼ), U(ζ.uʼ);
        const float_4 lanes(0.f, 1.f, 2.f, 3.f);

        const float δαʼ = δα / N;
        const float_4 α0 = α - ζ.φ + lanes * δαʼ;

        const float_4 x = rack::simd::cos(α0);
        const float_4 y = rack::simd::sin(α0);

        const float_4 xʼ = P * x - Q * y + R;
        const float_4 yʼ = S * x + T * y + U;
        const float_4 r = xʼ * xʼ + yʼ * yʼ + 1e-30f;

        const float_4 outʼ = yʼ / rack::simd::sqrt(r); // exact version first

        out[0] = outʼ[0];
        out[1] = outʼ[1];
        out[2] = outʼ[2];
        out[3] = outʼ[3];
    }

} // namespace float4A
