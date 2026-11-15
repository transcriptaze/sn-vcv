#include <cmath>
#include <iostream>

#include <simd/Vector.hpp>
#include <simd/functions.hpp>

#include "benchmarks.hpp"

using rack::simd::float_4;
using std::cout;

namespace sn4d {

    typedef struct Ζ {
        double pʼ;
        double qʼ;
        double rʼ;
        double sʼ;
        double tʼ;
        double uʼ;
        double φ;
    } Ζ;

    typedef struct SN {
        SN(double ε, double θ, double A, double δx, double δy, double Φ, double m) {
            this->ε = ε;
            this->θ = θ;
            this->A = A;
            this->δx = δx;
            this->δy = δy;
            this->Φ = Φ;
            this->m = m;
        }

        double ε;
        double θ;
        double A;
        double δx;
        double δy;
        double Φ;
        double m;

        struct Ζ ζ = {
            .pʼ = 1.f,
            .qʼ = 0.f,
            .rʼ = 0.f,
            .sʼ = 1.f,
            .tʼ = 0.f,
            .uʼ = 0.f,
            .φ = 0.f,
        };

        rack::simd::float_4 P;
        rack::simd::float_4 Q;
        rack::simd::float_4 R;
        rack::simd::float_4 S;
        rack::simd::float_4 T;
        rack::simd::float_4 U;

    } SN;

    float_4 υ4(SN &sn, double α, double δα) {
        const float_4 lanes(0.f, 1.f, 2.f, 3.f);
        const float δαʼ = δα / 4.0;
        const float_4 α0 = (α - sn.ζ.φ) + lanes * δαʼ;

        const float_4 x = rack::simd::cos(α0);
        const float_4 y = rack::simd::sin(α0);

        const float_4 xʼ = sn.P * x - sn.Q * y + sn.R;
        const float_4 yʼ = sn.S * x + sn.T * y + sn.U;
        const float_4 r = xʼ * xʼ + yʼ * yʼ + 1e-30f;

        return yʼ / rack::simd::sqrt(r);
    }

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

        sn.P = float_4(sn.ζ.pʼ);
        sn.Q = float_4(sn.ζ.qʼ);
        sn.R = float_4(sn.ζ.rʼ);
        sn.S = float_4(sn.ζ.sʼ);
        sn.T = float_4(sn.ζ.tʼ);
        sn.U = float_4(sn.ζ.uʼ);
    }
}; // namespace sn4d

int main(int argc, char *argv[]) {
    // clang-format off
    double notes[16] = {441.7, 491.8, 556.2, 584.9, 661.5, 737.4, 833.1, 877.6, 657.8, 589.1, 742.3, 495.2, 552.7, 827.9, 442.8, 663.1};
    double φ[16]     = {  0.0, 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0  };
    double vco[16]   = {  0.0, 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0  };
    // clang-format on

    struct sn4d::SN sn = sn4d::SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

    sn4d::recompute(sn);

    const double δt = 1.0 / FS;

    double checksum = 0.0;
    for (size_t sample = 0; sample < SAMPLES; sample++) {
        for (size_t ch = 0; ch < 16; ch++) {
            const double δφ = notes[ch] * δt;

            double φʼ = φ[ch] + δφ;
            while (φʼ >= 1.0) {
                φʼ -= 1.0;
            }

            φ[ch] = φʼ;

            const double α = 2.0 * M_PI * φʼ;
            const float_4 out = sn4d::υ4(sn, α, δφ);

#ifdef VALIDATE
            double reference = std::sin(2.0 * M_PI * φʼ);
            double err = std::abs(out[0] - reference);

            if (err > 1e-6) {
                std::cout << φʼ << "  "
                          << out[0] << "  "
                          << reference << "  "
                          << err << "\n";
            }
#endif

            vco[ch] = 5.0 * out[0];
            checksum += std::abs(vco[ch]);
        }
    }

    for (size_t ch = 0; ch < 16; ch++) {
        cout << ">> SN4D: " << ch + 1 << "  " << vco[ch] << "\n";
    }

    cout << ">> CHECKSUM: " << checksum << "\n";

    return 0;
}
