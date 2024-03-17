#include <algorithm>
#include <utility>

#include "../plugin.hpp"
#include "../sn.hpp"

#include "DCF.hpp"

typedef struct Beta {
    double slow;
    double medium;
    double fast;

} Beta;

const std::map<int, std::pair<Beta, double>> BETA_Κ = {
    {11025, {{0.999, 0.995, 0.991}, 0.001}},
    {12000, {{0.999, 0.995, 0.991}, 0.001}},
    {22050, {{0.999, 0.995, 0.991}, 0.001}},
    {24000, {{0.999, 0.995, 0.991}, 0.001}},
    {44100, {{0.999, 0.995, 0.991}, 0.001}},
    {48000, {{0.999, 0.995, 0.991}, 0.001}},
    {88200, {{0.999, 0.995, 0.991}, 0.0005}},

    {96000, {{0.9990, 0.9970, 0.9950}, 0.0005}},
    {176400, {{0.9990, 0.9980, 0.9970}, 0.00025}},
    {192000, {{0.9995, 0.999, 0.9985}, 0.00025}},
    {352800, {{0.9995, 0.999, 0.9985}, 0.0001}},
    {384000, {{0.9995, 0.999, 0.9985}, 0.0001}},
    {705600, {{0.9995, 0.999, 0.9985}, 0.00005}},
    {768000, {{0.9995, 0.999, 0.9985}, 0.00005}},
};

/* DC blocking filter *
 *
 * Ref. https://www.dsprelated.com/freebooks/filters/DC_Blocker.html
 * Ref. https://kleinembedded.com/quick-and-simple-digital-filters-for-smoothing-and-dc-removal
 * Ref. http://www.digitalsignallabs.com/downloads/dcblock.pdf
 */
DCF::DCF(DCBLOCK block) {
    recompute(block, 44100.f);
}

DCF::DCF(DCBLOCK block, float fs) {
    recompute(block, fs);
}

void DCF::recompute(DCBLOCK block, float fs) {
    if (this->block != block || this->fs != fs) {
        this->block = block;
        this->fs = fs;

        int f = static_cast<int>(std::round(fs));

        auto it = BETA_Κ.find(f);
        if (it != BETA_Κ.end()) {
            switch (block) {
            case DCBLOCK_NONE:
                break;

            case DCBLOCK_SLOW:
                β = it->second.first.slow;
                break;

            case DCBLOCK_MEDIUM:
                β = it->second.first.medium;
                break;

            case DCBLOCK_FAST:
                β = it->second.first.fast;
                break;
            }

            κ = it->second.second;
        } else {
            β = 0.995;
            κ = 0.001;
        }

        G = (1.0 + β) / 2.0;
    }

    G = (1.0 + β) / 2.0;
}

// NB: in and out must NOT be the same array (compiler aliases variables)
void DCF::process(const double *in, double *out, size_t channels) {
    if (block != DCBLOCK_NONE && α < 1.0) {
        α += std::min(1.0, κ * (1.0 - α));
    } else if (block == DCBLOCK_NONE && α > 0.0) {
        α -= std::max(0.0, κ * α);
    }

    for (size_t ch = 0; ch < channels; ch++) {
        y[ch] = G * (in[ch] - x[ch]) + β * y[ch];
        x[ch] = in[ch];
        out[ch] = α * y[ch] + (1 - α) * in[ch];
    }
}

DCBLOCK DCF::int2mode(int v, DCBLOCK defval) {
    switch (v) {
    case DCBLOCK_NONE:
        return DCBLOCK_NONE;

    case DCBLOCK_SLOW:
        return DCBLOCK_SLOW;

    case DCBLOCK_MEDIUM:
        return DCBLOCK_MEDIUM;

    case DCBLOCK_FAST:
        return DCBLOCK_FAST;

    default:
        return defval;
    }
}
