#include "../plugin.hpp"
#include "../sn.hpp"

#include "DCF.hpp"

const std::map<int, double> BETA = {
    {11025, 0.995},
    {12000, 0.995},
    {22050, 0.995},
    {24000, 0.995},
    {44100, 0.995},

    {48000, 0.995},
    {88200, 0.995},
    {96000, 0.997},
    {176400, 0.998},
    {192000, 0.999},
    {352800, 0.999},
    {384000, 0.999},
    {705600, 0.999},
    {768000, 0.999},
};

/* DC blocking filter *
 *
 * Ref. https://www.dsprelated.com/freebooks/filters/DC_Blocker.html
 * Ref. https://kleinembedded.com/quick-and-simple-digital-filters-for-smoothing-and-dc-removal/
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

        auto it = BETA.find(f);
        if (it != BETA.end()) {
            β = it->second;
        } else {
            β = 0.995;
        }

        G = (1.0 + β) / 2.0;
    }
}

// NB: in and out must NOT be the same array (compiler aliases variables)
void DCF::process(const double *in, double *out, size_t channels) {
    for (size_t ch = 0; ch < channels; ch++) {
        out[ch] = G * (in[ch] - x[ch]) + β * y[ch];
        x[ch] = in[ch];
        y[ch] = out[ch];
    }
}
