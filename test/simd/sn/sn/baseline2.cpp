#include <cmath>
#include <random>

#include "baseline2.hpp"

namespace baseline2 {

    // static std::mt19937 gen(54892137);
    // static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    //
    // static const std::array<float, 37> randoms = [] {
    //     std::array<float, 37> a;
    //     for (auto& v : a) v = dist(gen);
    //     return a;
    // }();

    // const float randoms[37] = {
    //     0.0745168,
    //     0.215663,
    //     0.761099,
    //     0.347939,
    //     0.384917,
    //     0.216187,
    //     0.719554,
    //     0.440048,
    //     0.642393,
    //     0.320469,
    //     0.892961,
    //     0.46639,
    //     0.814149,
    //     0.836472,
    //     0.463045,
    //     0.792535,
    //     0.138174,
    //     0.581718,
    //     0.175642,
    //     0.0243384,
    //     0.802986,
    //     0.887503,
    //     0.922249,
    //     0.502394,
    //     0.432791,
    //     0.30785,
    //     0.897449,
    // };

    void recompute(SN &sn) {
        sn.ζ.pʼ = 1.0;
        sn.ζ.qʼ = 0.0;
        sn.ζ.rʼ = 0.0;
        sn.ζ.sʼ = 0.0;
        sn.ζ.tʼ = 1.0;
        sn.ζ.uʼ = 0.0;
        sn.ζ.φ = 0.0;
    }

    __attribute__((noinline)) float υ(const SN &, float a) { return a * 0.1f; }

    // float υ(const SN &sn, float α) {
    //     static size_t ix = 0;
    //
    //     ix = (ix + 1) % 37;
    //
    //     return std::fmod(α + randoms[ix], 2.0) - 1.0;
    // }

} // namespace baseline2
