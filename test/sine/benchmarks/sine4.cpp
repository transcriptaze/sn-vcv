#include <cmath>
#include <iostream>

#include <simd/functions.hpp>

#include "benchmarks.hpp"

using std::cout;

int main(int argc, char *argv[]) {
    // clang-format off
    float notes[16] = {441.7, 491.8, 556.2, 584.9, 661.5, 737.4, 833.1, 877.6, 657.8, 589.1, 742.3, 495.2, 552.7, 827.9, 442.8, 663.1};
    float φ[16]     = {  0.0, 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0  };
    float vco[16]   = {  0.0, 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0  };
    // clang-format on

    const float δt = 1.0 / FS;

    double checksum = 0.0;
    for (size_t sample = 0; sample < SAMPLES; sample++) {
        for (size_t ch = 0; ch < 16; ch++) {
            const float δφ = notes[ch] * δt;

            float φʼ = φ[ch] + δφ;
            while (φʼ >= 1.0f) {
                φʼ -= 1.0f;
            }

            φ[ch] = φʼ;

            const double α = 2.0 * M_PI * φʼ;
            const float out = rack::simd::sin(static_cast<float>(α));

            // double reference = std::sin(α);
            // double err = std::abs(out - reference);
            //
            // if (err > 1e-6) {
            //     std::cout << φʼ << "  "
            //               << out << "  "
            //               << reference << "  "
            //               << err << "\n";
            // }

            vco[ch] = 5.0f * out;
            checksum += std::abs(vco[ch]);
        }
    }

    for (size_t ch = 0; ch < 16; ch++) {
        cout << ">> SINE4: " << ch + 1 << "  " << vco[ch] << "\n";
    }

    cout << ">> CHECKSUM: " << checksum << "\n";

    return 0;
}
