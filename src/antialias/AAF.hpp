#pragma once

#include "../butterworth.h"
#include "../plugin.hpp"
#include "../sn.hpp"

struct AAF {
    AAF(ANTIALIAS mode, float fs);
    void reset();
    void process(const double *in, double *out, size_t channels);

    dsp::IIRFilter<5, 5, double> lpf[16];
};

typedef struct AA {
    static int oversampling(ANTIALIAS mode);

    AA();

    void reset();
    void recompute(float fs);
    void process(ANTIALIAS mode, const double in[4][16], double out[16], size_t channels);

    float fs;
    ANTIALIAS mode;

    AAF x1f1;
    AAF x1f2[2];
    AAF x2f1;
} AA;
