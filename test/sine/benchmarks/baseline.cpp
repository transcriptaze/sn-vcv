#include <cmath>
#include <iostream>

#include "benchmarks.hpp"

using std::cout;

constexpr double TWO_TO_32 = 4294967296.0; // 2^32
constexpr uint32_t MAX_RAMP = 2147483647U;

int main(int argc, char *argv[]) {
    // clang-format off
    float  notes[16] = {441.7, 491.8, 556.2, 584.9, 661.5, 737.4, 833.1, 877.6, 657.8, 589.1, 742.3, 495.2, 552.7, 827.9, 442.8, 663.1};
    uint32_t  α[16]  = {  0,   0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0    };
    double vco[16]   = {  0.0, 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0  };
    // clang-format on

    double checksum = 0.0;

    for (size_t sample = 0; sample < SAMPLES; sample++) {
        for (size_t ch = 0; ch < 16; ch++) {
            uint32_t δα = static_cast<uint32_t>((notes[ch] / FS) * TWO_TO_32);
            uint32_t αʼ = α[ch] + δα;

            bool msb = (αʼ >> 31) & 1;
            uint32_t ramp = αʼ & 0x7FFFFFFF;
            if (msb) {
                ramp = MAX_RAMP - ramp;
            }

            double normalized = (static_cast<double>(ramp) / static_cast<double>(MAX_RAMP)) * 2.0 - 1.0;

            vco[ch] = normalized * 5.0f;
            α[ch] = αʼ;

            checksum += vco[ch];
        }
    }

    for (size_t ch = 0; ch < 16; ch++) {
        cout << ">> BASELINE: " << ch + 1 << "  " << vco[ch] << "\n";
    }

    cout << ">> CHECKSUM: " << checksum << "\n";

    return 0;
}
