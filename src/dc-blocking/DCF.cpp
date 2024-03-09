#include <algorithm>
#include <utility>

#include "../plugin.hpp"
#include "../sn.hpp"

#include "DCF.hpp"

const std::map<int, std::pair<double, double>> BETAΚ = {
    {11025, {0.995, 0.001}},
    {12000, {0.995, 0.001}},
    {22050, {0.995, 0.001}},
    {24000, {0.995, 0.001}},
    {44100, {0.995, 0.001}},

    {48000, {0.995, 0.001}},
    {88200, {0.995, 0.0005}},
    {96000, {0.997, 0.0005}},
    {176400, {0.998, 0.00025}},
    {192000, {0.999, 0.00025}},
    {352800, {0.999, 0.0001}},
    {384000, {0.999, 0.0001}},
    {705600, {0.999, 0.00005}},
    {768000, {0.999, 0.00005}},
};

/* DC blocking filter *
 *
 * Ref. https://www.dsprelated.com/freebooks/filters/DC_Blocker.html
 * Ref. https://kleinembedded.com/quick-and-simple-digital-filters-for-smoothing-and-dc-removal
 * Ref. http://www.digitalsignallabs.com/downloads/dcblock.pdf
 */
DCF::DCF() {
    fs = 44100.f;
}

DCF::DCF(float fs) {
    this->fs = fs;
}

void DCF::recompute(float fs) {
    if (this->fs != fs) {
        this->fs = fs;

        int f = static_cast<int>(std::round(fs));

        auto it = BETAΚ.find(f);
        if (it != BETAΚ.end()) {
            β = it->second.first;
            κ = it->second.second;
        } else {
            β = 0.995;
            κ = 0.001;
        }

        G = (1.0 + β) / 2.0;
    }
}

// NB: in and out must NOT be the same array (compiler aliases variables)
void DCF::process(DCBLOCK block, const double *in, double *out, size_t channels) {
    if (block == DCBLOCK_ON && α < 1.0) {
        α += std::min(1.0, κ * (1.0 - α));
    } else if (block == DCBLOCK_OFF && α > 0.0) {
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
    case DCBLOCK_OFF:
        return DCBLOCK_OFF;
    case DCBLOCK_ON:
        return DCBLOCK_ON;
    default:
        return defval;
    }
}
