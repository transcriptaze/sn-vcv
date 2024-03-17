#pragma once

#include "../plugin.hpp"
#include "../sn.hpp"

struct DCF {
    static DCBLOCK int2mode(int v, DCBLOCK defval);

    DCF(DCBLOCK block);
    DCF(DCBLOCK block, float fs);

    void recompute(DCBLOCK block, float fs);
    void process(const double *in, double *out, size_t channels);

    DCBLOCK block;
    float fs;
    double κ = 0.001;
    double α = 1.0;
    double β = 0.995;
    double G = 0.9975; // (1.0 + β) / 2.0;

    double x[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double y[16] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
};
