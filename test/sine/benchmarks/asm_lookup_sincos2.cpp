#include <cmath>
#include <cstdio>
#include <iostream>

#include "asm_lut.hpp"
#include "asm_sn.hpp"
#include "benchmarks.hpp"

using std::cout;

int main(int argc, char *argv[]) {
    // init_tab(K);

    // clang-format off
    double notes[16] = {441.7, 491.8, 556.2, 584.9, 661.5, 737.4, 833.1, 877.6, 657.8, 589.1, 742.3, 495.2, 552.7, 827.9, 442.8, 663.1};
    uint32_t φ[16]   = {  0,   0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,     0  };
    double vco[16]   = {  0.0, 0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0,   0.0  };
    // clang-format on

    struct asm_lookup_sincos2::SN sn = asm_lookup_sincos2::SN(0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1);

    asm_lookup_sincos2::recompute(sn);

    const double δt = 1.0 / FS;

    double checksum = 0.0;
    for (size_t sample = 0; sample < SAMPLES; sample++) {
        for (size_t ch = 0; ch < 16; ch++) {
            const uint32_t δφ = static_cast<uint32_t>(std::llround(notes[ch] * δt * 4294967296.0));
            φ[ch] += δφ;

            const double out = asm_lookup_sincos2::υ(sn, φ[ch]);

#ifdef VALIDATE
            const double φʼ = φ[ch] / 4294967296.0;
            double reference = std::sin(2.0 * M_PI * φʼ);
            double err = std::abs(out - reference);

            if (err > 1e-6) {
                std::cout << φʼ << "  "
                          << out << "  "
                          << reference << "  "
                          << err << "\n";
            }
#endif

            vco[ch] = 5.0 * out;
            checksum += std::abs(vco[ch]);
        }
    }

    for (size_t ch = 0; ch < 16; ch++) {
        cout << ">> ASM(lookup,mixed): " << ch + 1 << "  " << vco[ch] << "\n";
    }

    cout << ">> CHECKSUM: " << checksum << "\n";

    return 0;
}
