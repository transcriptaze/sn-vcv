#include <cmath>
#include <iostream>

#include "benchmarks.hpp"

using std::cout;

int main(int argc, char *argv[]) {
    // clang-format off
    double notes[16] = {441.7, 491.8, 556.2, 584.9, 661.5, 737.4, 833.1, 877.6, 657.8, 589.1, 742.3, 495.2, 552.7, 827.9, 442.8, 663.1};
    double φ[16]     = {  0.0, 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0  };
    double vco[16]   = {  0.0, 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0  };
    // clang-format on

    const double δt = 1.0 / FS;

    double checksum = 0.0;
    for (size_t sample = 0; sample < SAMPLES; sample++) {
        for (size_t ch = 0; ch < 16; ch++) {
            const double δφ = notes[ch] * δt;

            double φʼ = φ[ch] + δφ;
            while (φʼ >= 1.0) {
                φʼ -= 1.0;
            }

            φ[ch] = φʼ;

            double cosα = φʼ;

            cosα -= 0.25 + std::floor(cosα + 0.25);
            cosα *= 16.0 * (std::abs(cosα) - 0.5);
            cosα += 0.224 * cosα * (std::abs(cosα) - 1.0);

#ifdef VALIDATE
            double reference = std::cos(2.0 * M_PI * φʼ);
            double err = std::abs(cosα - reference);

            if (err > 1e-3) {
                std::cout << φʼ << "  "
                          << cosα << "  "
                          << reference << "  "
                          << err << "\n";
            }
#endif

            vco[ch] = 5.0 * cosα;
            checksum += std::abs(vco[ch]);
        }
    }

    for (size_t ch = 0; ch < 16; ch++) {
        cout << ">> POLYNOMIAL: " << ch + 1 << "  " << vco[ch] << "\n";
    }

    cout << ">> CHECKSUM: " << checksum << "\n";

    return 0;
}
