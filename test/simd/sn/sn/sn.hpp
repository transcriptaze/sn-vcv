#pragma once

#include <string>
#include <vector>

// SN
typedef struct Ζ {
    float pʼ;
    float qʼ;
    float rʼ;
    float sʼ;
    float tʼ;
    float uʼ;
    float φ;
} Ζ;

typedef struct SN {
    SN(float ε, float θ, float A, float δx, float δy, float Φ, float m) {
        this->ε = ε;
        this->θ = θ;
        this->A = A;
        this->δx = δx;
        this->δy = δy;
        this->Φ = Φ;
        this->m = m;
    }

    float ε;
    float θ;
    float A;
    float δx;
    float δy;
    float Φ;
    float m;

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
