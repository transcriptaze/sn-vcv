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
                                CHECK(sn2.ζ.rʼ == sn1.ζ.rʼ);
                                CHECK(sn2.ζ.sʼ == sn1.ζ.sʼ);
                                CHECK(sn2.ζ.tʼ == sn1.ζ.tʼ);
                                CHECK(sn2.ζ.uʼ == sn1.ζ.uʼ);
                                CHECK(sn2.ζ.φ == sn1.ζ.φ);
                            }
                        }
                    }
                }
            }
        }
    }
}