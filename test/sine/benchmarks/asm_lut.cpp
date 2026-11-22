#include <cmath>
#include <cstring>

#include "asm_lut.hpp"

constexpr int BITS = 11; // N = 1024 (try 9 as well)
constexpr uint32_t N = 1u << BITS;
constexpr float STEP = float(2.0 * M_PI / N);

extern SC tab[2048];
extern SCD tabd[2048];

void sincos_lut(uint32_t phase, float &s, float &c) {
    const uint32_t idx = (phase + (1u << (31 - BITS))) >> (32 - BITS);
    const uint32_t m = (((phase << BITS) ^ 0x80000000u) >> 9) | 0x3f800000u;

    float f;
    std::memcpy(&f, &m, 4);
    const float d = (f - 1.5f) * STEP;

    const SC e = tab[idx];
    s = e.s + e.c * d;
    c = e.c - e.s * d;
}
