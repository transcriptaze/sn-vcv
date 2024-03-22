#pragma once

#include "../plugin.hpp"
#include "../sn.hpp"

#include "LPF.hpp"

typedef struct AA {
    static int oversampling(ANTIALIAS mode);
    static ANTIALIAS int2mode(int v, ANTIALIAS defval);

    AA();

    void reset();
    void recompute(float fs);
    void process(ANTIALIAS mode, const double in[4][16], double out[16], size_t channels);

    float fs;
    ANTIALIAS mode;

    LPF x1f1;
    LPF x1f2[2];
    LPF x2f1;
    LPF x2f2[2];
    LPF x4f1;
    LPF x4f2[2];
} AA;
