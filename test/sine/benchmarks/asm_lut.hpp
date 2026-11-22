#pragma once

#include <cstdint>

struct SC {
    float s, c;
};

struct SCD {
    double s, c;
};

void sincos_lut(uint32_t phase, float &s, float &c);
void sincos_lut_d(uint32_t phase, double &s, double &c);
