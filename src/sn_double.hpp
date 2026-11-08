#pragma once

#include "plugin.hpp"

typedef struct Ζ_DOUBLE {
    double pʼ;
    double qʼ;
    double rʼ;
    double sʼ;
    double tʼ;
    double uʼ;
    double φ;
} Ζ_DOUBLE;

typedef struct SN_DOUBLE {
    SN_DOUBLE(double ε, double θ, double A, double δx, double δy, double Φ, double m) {
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

    struct Ζ_DOUBLE ζ = {
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

    void recompute();
    float υ(float α);
    rack::simd::float_4 υ4(float α, float δt);
} SN_DOUBLE;
