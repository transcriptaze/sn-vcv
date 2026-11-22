#pragma once

#include <cstdint>

namespace asm_lookup_sincos2 {

    typedef struct Ζ {
        double pʼ;
        double qʼ;
        double rʼ;
        double sʼ;
        double tʼ;
        double uʼ;
        double φ;
        double φʼ;
    } Ζ;

    typedef struct SN {
        SN(double ε, double θ, double A, double δx, double δy, double Φ, unsigned m) {
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
        unsigned m;

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

    void recompute(struct SN &sn);
    float υ(const struct SN &sn, uint32_t φ);
}; // namespace asm_lookup_sincos2

namespace asm_lookup_sincos2f {

    typedef struct Ζ {
        float pʼ;
        float qʼ;
        float rʼ;
        float sʼ;
        float tʼ;
        float uʼ;
        float φ;
        float φʼ;
    } Ζ;

    typedef struct SN {
        SN(double ε, double θ, double A, double δx, double δy, double Φ, unsigned m) {
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
        unsigned m;

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

    void recompute(struct SN &sn);
    float υ(const struct SN &sn, uint32_t φ);

}; // namespace asm_lookup_sincos2f

namespace asm_lookup_sincos2d {

    typedef struct Ζ {
        double pʼ;
        double qʼ;
        double rʼ;
        double sʼ;
        double tʼ;
        double uʼ;
        double φ;
        double φʼ;
    } Ζ;

    typedef struct SN {
        SN(double ε, double θ, double A, double δx, double δy, double Φ, unsigned m) {
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
        unsigned m;

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

    void recompute(struct SN &sn);
    double υ(const struct SN &sn, uint32_t φ);

}; // namespace asm_lookup_sincos2d
