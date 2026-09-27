#include <cmath>
#include <iostream>
#include <vector>

#include "doctest.h"
#include "sn/reference.hpp"
#include "sn/riffles.hpp"
#include "sn/sn.hpp"

using std::cout;
using std::vector;

TEST_CASE("validating SN::recompute() reference implementation") {
    float ε = 0.0;
    float θ = 0.0;
    float A = 1.0;
    float δx = 0.0;
    float δy = 0.0;
    float Φ = 0.0;
    float m = 1.0;

    struct SN sn(ε, θ, A, δx, δy, Φ, m);

    reference::recompute(sn);

    CHECK(sn.ζ.pʼ == 1.0);
    CHECK(sn.ζ.qʼ == 0.0);
    CHECK(sn.ζ.rʼ == 0.0);
    CHECK(sn.ζ.sʼ == 0.0);
    CHECK(sn.ζ.tʼ == 1.0);
    CHECK(sn.ζ.uʼ == 0.0);
    CHECK(sn.ζ.φ == 0.0);
}

TEST_CASE("validating SN::υ() reference implementation") {
    float ε = std::tanh(1.0 * 0.2);
    float θ = 0.0;
    float A = 1.0;
    float δx = 0.4;
    float δy = -0.1;
    float Φ = 120.0 * M_PI / 180.0;
    float m = 1.0;

    struct SN sn(ε, θ, A, δx, δy, Φ, m);

    reference::recompute(sn);

    CHECK(sn.ζ.pʼ == 1.0);
    CHECK(sn.ζ.qʼ == 0.0);
    CHECK(sn.ζ.rʼ == doctest::Approx(0.4));
    CHECK(sn.ζ.sʼ == 0.0);
    CHECK(sn.ζ.tʼ == doctest::Approx(0.980328));
    CHECK(sn.ζ.uʼ == doctest::Approx(-0.0980328));
    CHECK(sn.ζ.φ == doctest::Approx(-2.08584));

    float α[] = {0.0, 0.785398, 1.570796, 2.356194, 3.141592, 3.926991, 4.712389, 5.497787, 6.283185};
    float expected[] = {0.99257, 0.279035, -0.777237, -0.991973, -0.729219, -0.255153, 0.289949, 0.785495, 0.99257};
    size_t N = sizeof(α) / sizeof(float);

    for (size_t i = 0; i < N; i++) {
        CHECK(reference::υ(sn, α[i]) == doctest::Approx(expected[i]));
    }
}

TEST_CASE("validating SN::recompute() riffles implementation") {
    struct {
        vector<float> ε;
        vector<float> θ;
        vector<float> A;
        vector<float> δx;
        vector<float> δy;
        vector<float> Φ;
        vector<float> m;
    } tests = {
        .ε = {-0.99999, -0.5, 0.0, 0.5, 0.99999},
        .θ = {-1.570796, -1.047196, 0.0, 1.047196, 1.570796},
        .A = {0.0, 0.25, 0.5, 0.75, 1.0},
        .δx = {-1.0, -0.5, 0.0, 0.5, 1.0},
        .δy = {-1.0, -0.5, 0.0, 0.5, 1.0},
        .Φ = {-1.570796, -1.047196, 0.0, 1.047196, 1.570796},
        .m = {1.0, 2.0, 3.0, 4.0, 5.0},
    };

    for (auto εʼ = tests.ε.begin(); εʼ != tests.ε.end(); εʼ++) {
        for (auto θʼ = tests.θ.begin(); θʼ != tests.θ.end(); θʼ++) {
            for (auto Aʼ = tests.A.begin(); Aʼ != tests.A.end(); Aʼ++) {
                for (auto δxʼ = tests.δx.begin(); δxʼ != tests.δx.end(); δxʼ++) {
                    for (auto δyʼ = tests.δy.begin(); δyʼ != tests.δy.end(); δyʼ++) {
                        for (auto Φʼ = tests.Φ.begin(); Φʼ != tests.Φ.end(); Φʼ++) {
                            for (auto mʼ = tests.m.begin(); mʼ != tests.m.end(); mʼ++) {
                                float ε = *εʼ;
                                float θ = *θʼ;
                                float A = *Aʼ;
                                float δx = *δxʼ;
                                float δy = *δyʼ;
                                float Φ = *Φʼ;
                                float m = *mʼ;

                                struct SN sn1(ε, θ, A, δx, δy, Φ, m);
                                struct SN sn2(ε, θ, A, δx, δy, Φ, m);

                                reference::recompute(sn1);
                                riffles::recompute(sn2);

                                CHECK(sn2.ζ.pʼ == sn1.ζ.pʼ);
                                CHECK(sn2.ζ.qʼ == sn1.ζ.qʼ);
                                CHECK(sn2.ζ.rʼ == doctest::Approx(sn1.ζ.rʼ));
                                CHECK(sn2.ζ.sʼ == sn1.ζ.sʼ);
                                CHECK(sn2.ζ.tʼ == sn1.ζ.tʼ);

                                // if ((std::abs(sn2.ζ.uʼ - sn1.ζ.uʼ) > 0.01) && (std::abs(sn2.ζ.uʼ + sn1.ζ.uʼ) > 0.01)) {
                                //     cout << "ε:" << ε << " θ:" << θ << " A:" << A << " δx:" << δx << "  δy:" << δy << "  Φ:" << Φ << "  m:" << m << "  sn1.ζ.uʼ:" << sn1.ζ.uʼ << "  sn2.ζ.uʼ" << sn2.ζ.uʼ << "\n";
                                // }

                                // riffles fixes the θ sign flip/jump
                                CAPTURE(sn2.ζ.uʼ);
                                CAPTURE(sn1.ζ.uʼ);
                                CHECK((sn2.ζ.uʼ == doctest::Approx(sn1.ζ.uʼ) || sn2.ζ.uʼ == doctest::Approx(-sn1.ζ.uʼ)));

                                CHECK(sn2.ζ.φ == doctest::Approx(sn1.ζ.φ)); // ±0.000001
                            }
                        }
                    }
                }
            }
        }
    }
}

TEST_CASE("validating SN::recompute() riffles implementation") {
    struct {
        vector<float> ε;
        vector<float> θ;
        vector<float> A;
        vector<float> δx;
        vector<float> δy;
        vector<float> Φ;
        vector<float> m;
    } tests = {
        .ε = {std::tanh(1.f * 0.2f)},
        .θ = {0.0},
        .A = {1.0},
        .δx = {0.4},
        .δy = {-0.1},
        .Φ = {2.094395},
        .m = {1.0},
    };

    float α[] = {0.0, 0.785398, 1.570796, 2.356194, 3.141592, 3.926991, 4.712389, 5.497787, 6.283185};
    size_t N = sizeof(α) / sizeof(float);

    for (auto εʼ = tests.ε.begin(); εʼ != tests.ε.end(); εʼ++) {
        for (auto θʼ = tests.θ.begin(); θʼ != tests.θ.end(); θʼ++) {
            for (auto Aʼ = tests.A.begin(); Aʼ != tests.A.end(); Aʼ++) {
                for (auto δxʼ = tests.δx.begin(); δxʼ != tests.δx.end(); δxʼ++) {
                    for (auto δyʼ = tests.δy.begin(); δyʼ != tests.δy.end(); δyʼ++) {
                        for (auto Φʼ = tests.Φ.begin(); Φʼ != tests.Φ.end(); Φʼ++) {
                            for (auto mʼ = tests.m.begin(); mʼ != tests.m.end(); mʼ++) {
                                float ε = *εʼ;
                                float θ = *θʼ;
                                float A = *Aʼ;
                                float δx = *δxʼ;
                                float δy = *δyʼ;
                                float Φ = *Φʼ;
                                float m = *mʼ;

                                struct SN sn1(ε, θ, A, δx, δy, Φ, m);
                                struct SN sn2(ε, θ, A, δx, δy, Φ, m);

                                reference::recompute(sn1);
                                riffles::recompute(sn2);

                                CHECK(sn2.ζ.pʼ == sn1.ζ.pʼ);
                                CHECK(sn2.ζ.qʼ == sn1.ζ.qʼ);
                                CHECK(sn2.ζ.rʼ == doctest::Approx(sn1.ζ.rʼ));
                                CHECK(sn2.ζ.sʼ == sn1.ζ.sʼ);
                                CHECK(sn2.ζ.tʼ == sn1.ζ.tʼ);

                                // riffles fixes the θ sign flip/jump
                                CAPTURE(sn2.ζ.uʼ);
                                CAPTURE(sn1.ζ.uʼ);
                                CHECK((sn2.ζ.uʼ == doctest::Approx(sn1.ζ.uʼ) || sn2.ζ.uʼ == doctest::Approx(-sn1.ζ.uʼ)));

                                CHECK(sn2.ζ.φ == doctest::Approx(sn1.ζ.φ)); // ±0.000001

                                for (size_t i = 0; i < N; i++) {
                                    CHECK(riffles::υ(sn2, α[i]) == doctest::Approx(reference::υ(sn1, α[i])));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
