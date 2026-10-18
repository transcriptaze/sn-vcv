#include <cmath>

#include "validate.hpp"

// Reference implmentation of snʼ.
namespace reference {
    void recompute(sn &sn) {
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

    sn::sn(double ε, double θ, double A, double δx, double δy, double Φ, double m) {
        this->ε = ε;
        this->θ = θ;
        this->A = A;
        this->δx = δx;
        this->δy = δy;
        this->Φ = Φ;
        this->m = m;

        recompute(*this);
    }

    double sn::υ(double α) {
        double αʼ = m * α - ζ.φ;

        double x = std::cos(αʼ);
        double y = std::sin(αʼ);
        double xʼ = ζ.pʼ * x - ζ.qʼ * y + ζ.rʼ;
        double yʼ = ζ.sʼ * x + ζ.tʼ * y + ζ.uʼ;
        double r = std::hypot(xʼ, yʼ);

        return r > 0.0f ? yʼ / r : 0.0f;
    }

} // namespace reference
