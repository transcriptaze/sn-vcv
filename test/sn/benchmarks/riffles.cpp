#include <cmath>
#include <iostream>

#include "../sn/riffles.hpp"
#include "../sn/sn.hpp"

using std::cout;

int main(int argc, char *argv[]) {
    const float fs = 48000.0; // 48kHz
    const unsigned long samples = 48000 * 60 * 10;
    const unsigned oversampling = 4;
    const float δt = 1 / fs;

    float ε = std::tanh(1.0 * 0.2);
    float θ = 0.0;
    float A = 1.0;
    float δx = 0.4;
    float δy = -0.1;
    float Φ = 120.0 * M_PI / 180.0;
    float m = 1.0;

    struct SN sn(ε, θ, A, δx, δy, Φ, m);

    riffles::recompute(sn);

    // clang-format off
    float  notes[16] = {441.7, 491.8, 556.2, 584.9, 661.5, 737.4, 833.1, 877.6, 657.8, 589.1, 742.3, 495.2, 552.7, 827.9, 442.8, 663.1};
    float  α[16]     = {  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0 };
    double vco[16]   = {  0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0 };
    // clang-format on

    unsigned long count = 0;
    for (size_t sample = 0; sample < samples; sample++) {
        for (size_t ch = 0; ch < 16; ch++) {
            float δα = δt * notes[ch] / oversampling;
            float αʼ = α[ch];
            float υ = 0.0;

            for (size_t oversample = 0; oversample < oversampling; oversample++) {
                υ += riffles::υ(sn, αʼ);
                αʼ = std::fmod(αʼ + δα, 2 * M_PI);
                count++;
            }

            vco[ch] += υ;
            α[ch] = αʼ;
        }
    }

    for (size_t ch = 0; ch < 16; ch++) {
        cout << ">> RIFFLES: " << ch + 1 << "  " << vco[ch] << "\n";
    }

    cout << "\nCOUNT: " << count << "\n\n";

    return 0;
}
