#include <cmath>
#include <iostream>

#include "benchmarks.hpp"

using std::cout;

namespace sn {

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

} SN;

float υ(const SN &sn, float α) {
    float αʼ = sn.m * α - sn.ζ.φ;

    float x = std::cos(αʼ);
    float y = std::sin(αʼ);
    float xʼ = sn.ζ.pʼ * x - sn.ζ.qʼ * y + sn.ζ.rʼ;
    float yʼ = sn.ζ.sʼ * x + sn.ζ.tʼ * y + sn.ζ.uʼ;
    float r = std::hypot(xʼ, yʼ);

    return r > 0.0f ? yʼ / r : 0.0f;
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
}
}; // namespace sn

int main(int argc, char *argv[]) {
    // clang-format off
    double notes[16] = {441.7, 491.8, 556.2, 584.9, 661.5, 737.4, 833.1, 877.6, 657.8, 589.1, 742.3, 495.2, 552.7, 827.9, 442.8, 663.1};
    double φ[16]     = {  0.0, 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0  };
    double vco[16]   = {  0.0, 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0  };
    // clang-format on

    struct sn::SN sn = sn::SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);

    sn::recompute(sn);

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
            const double out = sn::υ(sn, α);

            // double reference = std::sin(2.0 * M_PI * φʼ);
            // double err = std::abs(out - reference);
            //
            // if (err > 1e-6) {
            //     std::cout << φʼ << "  "
            //               << out << "  "
            //               << reference << "  "
            //               << err << "\n";
            // }

            vco[ch] = 5.0 * out;
            checksum += std::abs(vco[ch]);
        }
    }

    for (size_t ch = 0; ch < 16; ch++) {
        cout << ">> SN: " << ch + 1 << "  " << vco[ch] << "\n";
    }

    cout << ">> CHECKSUM: " << checksum << "\n";

    return 0;
}
