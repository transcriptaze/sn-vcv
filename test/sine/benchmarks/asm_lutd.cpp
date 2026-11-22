#include <cmath>
#include <cstring>

#include "asm_lut.hpp"

constexpr int BITS = 11; // N = 1024 (try 9 as well)
constexpr uint32_t N = 1u << BITS;
constexpr float STEP = float(2.0 * M_PI / N);

extern SCD tabd[2048];

void sincos_lut_d(uint32_t phase, double &s, double &c) {
    const uint32_t idx = (phase + (1u << (31 - BITS))) >> (32 - BITS);
    const uint64_t m = (uint64_t((phase << BITS) ^ 0x80000000u) << 20) | 0x3ff0000000000000ull;

    double f;
    std::memcpy(&f, &m, 8);
    const double d = (f - 1.5) * double(STEP); // STEP computed in double

    const SCD e = tabd[idx];
    s = e.s + e.c * d;
    c = e.c - e.s * d;
}
