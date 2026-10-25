#include <cmath>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "doctest.h"
#include "validate/validate.hpp"

using std::string;
using std::vector;

struct test {
    double ε;
    double θ;
    double A;
    double δx;
    double δy;
    double Φ;
    double m;
};

vector<test> debug();
vector<test> generate();

TEST_CASE("validating reference implementation against std::sin") {
    double ε = 0.0;
    double θ = radians(0.0);
    double A = 1.0;
    double δx = 0.0;
    double δy = 0.0;
    double Φ = radians(0.0);
    double m = 1.0;

    struct reference::sn sn(ε, θ, A, δx, δy, Φ, m);

    for (size_t i = 0; i <= 360; i++) {
        double α = radians(static_cast<double>(i));

        CHECK(sn.υ(α) == doctest::Approx(std::sin(α)));
    }
}

TEST_CASE("validating reference implementation against 'standard ripple'") {
    double e = 0.2;
    double s = 1.0;

    double ε = std::tanh(s * e);
    double θ = radians(15.0);
    double A = 1.0;
    double δx = 0.4;
    double δy = -0.1;
    double Φ = radians(120.0);
    double m = 1.0;

    struct reference::sn sn(ε, θ, A, δx, δy, Φ, m);

    std::ifstream f("./validate/reference.dat");
    string line;
    double α;
    double v;

    while (std::getline(f, line)) {
        std::istringstream iss(line);

        iss >> α >> v;

        CHECK(sn.υ(radians(α)) == doctest::Approx(v));
    }

    f.close();
}

TEST_CASE("validating single precision implementation against reference") {
    vector<test> tests = generate();

    struct {
        std::vector<double> mean;
        std::vector<double> stddev;
        std::vector<double> max;
    } cumulative;

    for (auto ix = tests.begin(); ix != tests.end(); ix++) {
        double ε = ix->ε;
        double θ = radians(ix->θ);
        double A = ix->A;
        double δx = ix->δx;
        double δy = ix->δy;
        double Φ = radians(ix->Φ);
        double m = ix->m;

        float _ε = static_cast<float>(ε);
        float _θ = static_cast<float>(θ);
        float _A = static_cast<float>(A);
        float _δx = static_cast<float>(δx);
        float _δy = static_cast<float>(δy);
        float _Φ = static_cast<float>(Φ);
        float _m = static_cast<float>(m);

        struct reference::sn ref(ε, θ, A, δx, δy, Φ, m);
        struct reference::sn fp(_ε, _θ, _A, _δx, _δy, _Φ, _m);

        struct {
            double mean;
            double variance;
            double stddev;
            double max;
        } error;

        std::vector<double> errors;

        for (size_t i = 0; i <= 360; i++) {
            double α = static_cast<double>(i);
            double αʼ = radians(α);

            double p = fp.υ(static_cast<float>(αʼ));
            double q = ref.υ(αʼ);

            errors.push_back(p - q);

            if (std::abs(p - q) > 0.00275) {
                CHECK_MESSAGE(p == doctest::Approx(q), " -- ", p - q);
            }
        }

        // ... mean
        size_t N = errors.size();
        double sum = std::accumulate(errors.begin(), errors.end(), 0.0);
        error.mean = sum / static_cast<double>(N);

        error.variance = std::inner_product(errors.begin(), errors.end(), errors.begin(), 0.0,
                                            std::plus<>(),
                                            [mean = error.mean](double val, double) {
                                                double diff = val - mean;
                                                return diff * diff;
                                            });

        error.stddev = std::sqrt(error.variance / static_cast<double>(N));

        auto it = std::max_element(errors.begin(), errors.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });

        error.max = (it != errors.end()) ? std::abs(*it) : 0.0;

        if (std::abs(error.mean) > 0.0) {
            CHECK_MESSAGE(error.mean == doctest::Approx(0.0), " -- ", error.mean);
        }

        if (std::abs(error.stddev) > 0.00025) {
            CHECK_MESSAGE(error.stddev == doctest::Approx(0.0), " -- ", error.stddev);
        }

        if (std::abs(error.max) > 0.00275) {
            CHECK_MESSAGE(error.max == doctest::Approx(0.0), " -- ", error.max);
        }

        cumulative.mean.push_back(std::abs(error.mean));
        cumulative.stddev.push_back(std::abs(error.stddev));
        cumulative.max.push_back(std::abs(error.max));
    }

    // ... summarise
    std::printf("\n");
    std::printf("float\n");
    std::printf("=====\n");
    {
        auto it = std::max_element(cumulative.mean.begin(), cumulative.mean.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double mean = it != cumulative.mean.end() ? std::abs(*it) : 0.0;

        std::printf("    max(mean)    %0.6f\n", mean);
    }

    {
        auto it = std::max_element(cumulative.stddev.begin(), cumulative.stddev.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double stddev = it != cumulative.stddev.end() ? std::abs(*it) : 0.0;

        std::printf("    max(stddev)  %0.5f\n", stddev);
    }

    {
        auto it = std::max_element(cumulative.max.begin(), cumulative.max.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double max = it != cumulative.max.end() ? std::abs(*it) : 0.0;

        std::printf("    max(max)     %0.6f\n", max);
    }

    std::printf("\n");
}

TEST_CASE("validating sincos implementation against reference") {
    vector<test> tests = generate();

    struct {
        std::vector<double> mean;
        std::vector<double> stddev;
        std::vector<double> max;
    } cumulative;

    for (auto ix = tests.begin(); ix != tests.end(); ix++) {
        double ε = ix->ε;
        double θ = radians(ix->θ);
        double A = ix->A;
        double δx = ix->δx;
        double δy = ix->δy;
        double Φ = radians(ix->Φ);
        double m = ix->m;

        float _ε = static_cast<float>(ε);
        float _θ = static_cast<float>(θ);
        float _A = static_cast<float>(A);
        float _δx = static_cast<float>(δx);
        float _δy = static_cast<float>(δy);
        float _Φ = static_cast<float>(Φ);
        float _m = static_cast<float>(m);

        struct reference::sn ref(ε, θ, A, δx, δy, Φ, m);
        struct sincos::sn fp(_ε, _θ, _A, _δx, _δy, _Φ, _m);

        struct {
            double mean;
            double variance;
            double stddev;
            double max;
        } error;

        std::vector<double> errors;

        const float δα = 1.0;

        for (size_t i = 0; i <= 360; i++) {
            float α = static_cast<float>(i);
            double αʼ = radians(α);
            double δαʼ = radians(δα);

            float p[4];
            double q[4];

            fp.υ(static_cast<float>(m * αʼ), static_cast<float>(m * δαʼ), p);

            q[0] = ref.υ(αʼ);
            q[1] = ref.υ(αʼ + 0.25 * δαʼ);
            q[2] = ref.υ(αʼ + 0.5 * δαʼ);
            q[3] = ref.υ(αʼ + 0.75 * δαʼ);

            for (int k = 0; k < 4; k++) {
                double pʼ = p[k];
                double qʼ = q[k];

                errors.push_back(pʼ - qʼ);

                if (std::abs(pʼ - qʼ) > 2.0) {
                    CHECK_MESSAGE(pʼ == doctest::Approx(qʼ), " -- ", pʼ - qʼ);
                }
            }
        }

        // ... mean
        size_t N = errors.size();
        double sum = std::accumulate(errors.begin(), errors.end(), 0.0);
        error.mean = sum / static_cast<double>(N);

        error.variance = std::inner_product(errors.begin(), errors.end(), errors.begin(), 0.0,
                                            std::plus<>(),
                                            [mean = error.mean](double val, double) {
                                                double diff = val - mean;
                                                return diff * diff;
                                            });

        error.stddev = std::sqrt(error.variance / static_cast<double>(N));

        auto it = std::max_element(errors.begin(), errors.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });

        error.max = (it != errors.end()) ? std::abs(*it) : 0.0;

        CHECK_MESSAGE(error.mean == doctest::Approx(0.0), " -- ", error.mean);

        if (std::abs(error.stddev) > 0.0002) {
            CHECK_MESSAGE(error.stddev == doctest::Approx(0.0), " -- ", error.stddev);
        }

        if (std::abs(error.max) > 0.003) {
            CHECK_MESSAGE(error.max == doctest::Approx(0.0), " -- ", error.max);
        }

        cumulative.mean.push_back(std::abs(error.mean));
        cumulative.stddev.push_back(std::abs(error.stddev));
        cumulative.max.push_back(std::abs(error.max));
    }

    // ... summarise
    std::printf("\n");
    std::printf("sincos\n");
    std::printf("======\n");

    {
        auto it = std::max_element(cumulative.mean.begin(), cumulative.mean.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double mean = it != cumulative.mean.end() ? std::abs(*it) : 0.0;

        std::printf("    max(mean)    %0.6f\n", mean);
    }

    {
        auto it = std::max_element(cumulative.stddev.begin(), cumulative.stddev.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double stddev = it != cumulative.stddev.end() ? std::abs(*it) : 0.0;

        std::printf("    max(stddev)  %0.5f\n", stddev);
    }

    {
        auto it = std::max_element(cumulative.max.begin(), cumulative.max.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double max = it != cumulative.max.end() ? std::abs(*it) : 0.0;

        std::printf("    max(max)     %0.6f\n", max);
    }

    std::printf("\n");
}

TEST_CASE("validating sincos2 implementation against reference") {
    vector<test> tests = generate();

    struct {
        std::vector<double> mean;
        std::vector<double> stddev;
        std::vector<double> max;
    } cumulative;

    for (auto ix = tests.begin(); ix != tests.end(); ix++) {
        double ε = ix->ε;
        double θ = radians(ix->θ);
        double A = ix->A;
        double δx = ix->δx;
        double δy = ix->δy;
        double Φ = radians(ix->Φ);
        double m = ix->m;

        float _ε = static_cast<float>(ε);
        float _θ = static_cast<float>(θ);
        float _A = static_cast<float>(A);
        float _δx = static_cast<float>(δx);
        float _δy = static_cast<float>(δy);
        float _Φ = static_cast<float>(Φ);
        float _m = static_cast<float>(m);

        struct reference::sn ref(ε, θ, A, δx, δy, Φ, m);
        struct sincos2::sn fp(_ε, _θ, _A, _δx, _δy, _Φ, _m);

        struct {
            double mean;
            double variance;
            double stddev;
            double max;
        } error;

        std::vector<double> errors;

        const float δα = 1.0;

        for (size_t i = 0; i <= 360; i++) {
            float α = static_cast<float>(i);
            double αʼ = radians(α);
            double δαʼ = radians(δα);

            float p[4];
            double q[4];

            fp.υ(static_cast<float>(m * αʼ), static_cast<float>(m * δαʼ), p);

            q[0] = ref.υ(αʼ);
            q[1] = ref.υ(αʼ + 0.25 * δαʼ);
            q[2] = ref.υ(αʼ + 0.5 * δαʼ);
            q[3] = ref.υ(αʼ + 0.75 * δαʼ);

            for (int k = 0; k < 4; k++) {
                double pʼ = p[k];
                double qʼ = q[k];

                errors.push_back(pʼ - qʼ);

                if (std::abs(pʼ - qʼ) > 2.0) {
                    CHECK_MESSAGE(pʼ == doctest::Approx(qʼ), " -- ", pʼ - qʼ);
                }
            }
        }

        // ... mean
        size_t N = errors.size();
        double sum = std::accumulate(errors.begin(), errors.end(), 0.0);
        error.mean = sum / static_cast<double>(N);

        error.variance = std::inner_product(errors.begin(), errors.end(), errors.begin(), 0.0,
                                            std::plus<>(),
                                            [mean = error.mean](double val, double) {
                                                double diff = val - mean;
                                                return diff * diff;
                                            });

        error.stddev = std::sqrt(error.variance / static_cast<double>(N));

        auto it = std::max_element(errors.begin(), errors.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });

        error.max = (it != errors.end()) ? std::abs(*it) : 0.0;

        CHECK_MESSAGE(error.mean == doctest::Approx(0.0), " -- ", error.mean);

        if (std::abs(error.stddev) > 0.0002) {
            CHECK_MESSAGE(error.stddev == doctest::Approx(0.0), " -- ", error.stddev);
        }

        if (std::abs(error.max) > 0.003) {
            CHECK_MESSAGE(error.max == doctest::Approx(0.0), " -- ", error.max);
        }

        cumulative.mean.push_back(std::abs(error.mean));
        cumulative.stddev.push_back(std::abs(error.stddev));
        cumulative.max.push_back(std::abs(error.max));
    }

    // ... summarise
    std::printf("\n");
    std::printf("sincos2\n");
    std::printf("=======\n");

    {
        auto it = std::max_element(cumulative.mean.begin(), cumulative.mean.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double mean = it != cumulative.mean.end() ? std::abs(*it) : 0.0;

        std::printf("    max(mean)    %0.6f\n", mean);
    }

    {
        auto it = std::max_element(cumulative.stddev.begin(), cumulative.stddev.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double stddev = it != cumulative.stddev.end() ? std::abs(*it) : 0.0;

        std::printf("    max(stddev)  %0.5f\n", stddev);
    }

    {
        auto it = std::max_element(cumulative.max.begin(), cumulative.max.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double max = it != cumulative.max.end() ? std::abs(*it) : 0.0;

        std::printf("    max(max)     %0.6f\n", max);
    }

    std::printf("\n");
}

TEST_CASE("validating float4A implementation against reference") {
    vector<test> tests = generate();

    struct {
        std::vector<double> mean;
        std::vector<double> stddev;
        std::vector<double> max;
    } cumulative;

    for (auto ix = tests.begin(); ix != tests.end(); ix++) {
        double ε = ix->ε;
        double θ = radians(ix->θ);
        double A = ix->A;
        double δx = ix->δx;
        double δy = ix->δy;
        double Φ = radians(ix->Φ);
        double m = ix->m;

        float _ε = static_cast<float>(ε);
        float _θ = static_cast<float>(θ);
        float _A = static_cast<float>(A);
        float _δx = static_cast<float>(δx);
        float _δy = static_cast<float>(δy);
        float _Φ = static_cast<float>(Φ);
        float _m = static_cast<float>(m);

        struct reference::sn ref(ε, θ, A, δx, δy, Φ, m);
        struct float4A::sn fp(_ε, _θ, _A, _δx, _δy, _Φ, _m);

        struct {
            double mean;
            double variance;
            double stddev;
            double max;
        } error;

        std::vector<double> errors;

        const float δα = 1.0;

        for (size_t i = 0; i <= 360; i++) {
            float α = static_cast<float>(i);
            double αʼ = radians(α);
            double δαʼ = radians(δα);

            float p[4];
            double q[4];

            fp.υ(static_cast<float>(m * αʼ), static_cast<float>(m * δαʼ), p);

            q[0] = ref.υ(αʼ);
            q[1] = ref.υ(αʼ + 0.25 * δαʼ);
            q[2] = ref.υ(αʼ + 0.5 * δαʼ);
            q[3] = ref.υ(αʼ + 0.75 * δαʼ);

            for (int k = 0; k < 4; k++) {
                double pʼ = p[k];
                double qʼ = q[k];

                errors.push_back(pʼ - qʼ);

                if (std::abs(pʼ - qʼ) > 2.0) {
                    CHECK_MESSAGE(pʼ == doctest::Approx(qʼ), " -- ", pʼ - qʼ);
                }
            }
        }

        // ... mean
        size_t N = errors.size();
        double sum = std::accumulate(errors.begin(), errors.end(), 0.0);
        error.mean = sum / static_cast<double>(N);

        error.variance = std::inner_product(errors.begin(), errors.end(), errors.begin(), 0.0,
                                            std::plus<>(),
                                            [mean = error.mean](double val, double) {
                                                double diff = val - mean;
                                                return diff * diff;
                                            });

        error.stddev = std::sqrt(error.variance / static_cast<double>(N));

        auto it = std::max_element(errors.begin(), errors.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });

        error.max = (it != errors.end()) ? std::abs(*it) : 0.0;

        CHECK_MESSAGE(error.mean == doctest::Approx(0.0), " -- ", error.mean);

        if (std::abs(error.stddev) > 0.0002) {
            CHECK_MESSAGE(error.stddev == doctest::Approx(0.0), " -- ", error.stddev);
        }

        if (std::abs(error.max) > 0.003) {
            CHECK_MESSAGE(error.max == doctest::Approx(0.0), " -- ", error.max);
        }

        cumulative.mean.push_back(std::abs(error.mean));
        cumulative.stddev.push_back(std::abs(error.stddev));
        cumulative.max.push_back(std::abs(error.max));
    }

    // ... summarise
    std::printf("\n");
    std::printf("float4A\n");
    std::printf("=======\n");

    {
        auto it = std::max_element(cumulative.mean.begin(), cumulative.mean.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double mean = it != cumulative.mean.end() ? std::abs(*it) : 0.0;

        std::printf("    max(mean)    %0.6f\n", mean);
    }

    {
        auto it = std::max_element(cumulative.stddev.begin(), cumulative.stddev.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double stddev = it != cumulative.stddev.end() ? std::abs(*it) : 0.0;

        std::printf("    max(stddev)  %0.5f\n", stddev);
    }

    {
        auto it = std::max_element(cumulative.max.begin(), cumulative.max.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double max = it != cumulative.max.end() ? std::abs(*it) : 0.0;

        std::printf("    max(max)     %0.6f\n", max);
    }

    std::printf("\n");
}

TEST_CASE("validating float4XX implementation against reference") {
    vector<test> tests = generate();

    struct {
        std::vector<double> mean;
        std::vector<double> stddev;
        std::vector<double> max;
    } cumulative;

    for (auto ix = tests.begin(); ix != tests.end(); ix++) {
        double ε = ix->ε;
        double θ = radians(ix->θ);
        double A = ix->A;
        double δx = ix->δx;
        double δy = ix->δy;
        double Φ = radians(ix->Φ);
        double m = ix->m;

        struct reference::sn ref(ε, θ, A, δx, δy, Φ, m);
        struct float4XX::sn fp(ε, θ, A, δx, δy, Φ, m);

        struct {
            double mean;
            double variance;
            double stddev;
            double max;
        } error;

        std::vector<double> errors;

        const double δα = 1.0;

        for (size_t i = 0; i <= 360; i++) {
            double α = static_cast<double>(i);
            double αʼ = radians(α);
            double δαʼ = radians(δα);

            float p[4];
            double q[4];

            fp.υ(static_cast<float>(m * αʼ), static_cast<float>(m * δαʼ), p);

            q[0] = ref.υ(αʼ);
            q[1] = ref.υ(αʼ + 0.25 * δαʼ);
            q[2] = ref.υ(αʼ + 0.5 * δαʼ);
            q[3] = ref.υ(αʼ + 0.75 * δαʼ);

            for (int k = 0; k < 4; k++) {
                double pʼ = p[k];
                double qʼ = q[k];

                errors.push_back(pʼ - qʼ);

                if (std::abs(pʼ - qʼ) > 2.0) {
                    CHECK_MESSAGE(pʼ == doctest::Approx(qʼ), " -- ", pʼ - qʼ);
                }
            }
        }

        // ... mean
        size_t N = errors.size();
        double sum = std::accumulate(errors.begin(), errors.end(), 0.0);
        error.mean = sum / static_cast<double>(N);

        error.variance = std::inner_product(errors.begin(), errors.end(), errors.begin(), 0.0,
                                            std::plus<>(),
                                            [mean = error.mean](double val, double) {
                                                double diff = val - mean;
                                                return diff * diff;
                                            });

        error.stddev = std::sqrt(error.variance / static_cast<double>(N));

        auto it = std::max_element(errors.begin(), errors.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });

        error.max = (it != errors.end()) ? std::abs(*it) : 0.0;

        CHECK_MESSAGE(error.mean == doctest::Approx(0.0), " -- ", error.mean);

        if (std::abs(error.stddev) > 0.0002) {
            CHECK_MESSAGE(error.stddev == doctest::Approx(0.0), " -- ", error.stddev);
        }

        if (std::abs(error.max) > 0.003) {
            CHECK_MESSAGE(error.max == doctest::Approx(0.0), " -- ", error.max);
        }

        cumulative.mean.push_back(std::abs(error.mean));
        cumulative.stddev.push_back(std::abs(error.stddev));
        cumulative.max.push_back(std::abs(error.max));
    }

    // ... summarise
    std::printf("\n");
    std::printf("float4X\n");
    std::printf("=======\n");

    {
        auto it = std::max_element(cumulative.mean.begin(), cumulative.mean.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double mean = it != cumulative.mean.end() ? std::abs(*it) : 0.0;

        std::printf("    max(mean)    %0.6f\n", mean);
    }

    {
        auto it = std::max_element(cumulative.stddev.begin(), cumulative.stddev.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double stddev = it != cumulative.stddev.end() ? std::abs(*it) : 0.0;

        std::printf("    max(stddev)  %0.5f\n", stddev);
    }

    {
        auto it = std::max_element(cumulative.max.begin(), cumulative.max.end(), [](double a, double b) { return std::abs(a) < std::abs(b); });
        double max = it != cumulative.max.end() ? std::abs(*it) : 0.0;

        std::printf("    max(max)     %0.6f\n", max);
    }

    std::printf("\n");
}

vector<test> debug() {
    vector<test> tests;

    test t = {
        .ε = std::tanh(1.0 * 0.2),
        .θ = 15.0,
        .A = 1.0,
        .δx = 0.4,
        .δy = -0.1,
        .Φ = 120.0,
        .m = 1.0,
    };

    tests.push_back(t);

    return tests;
}

vector<test> generate() {
    vector<double> e({-0.99, -0.2, 0.0, 0.2, 0.99});
    vector<double> s({1.0, 2.0, 5.0});
    vector<double> θ({-45.0, 0.0, 45.0});
    vector<double> A({1.0});
    vector<double> δx({-0.4, 0.0, 0.4});
    vector<double> δy({-0.4, 0.0, 0.4});
    vector<double> Φ({-85.0, 0.0, 85.0});
    vector<double> m({1.0, 2.0, 3.0});

    vector<test> tests;

    for (auto ix = e.begin(); ix != e.end(); ix++) {
        for (auto jx = s.begin(); jx != s.end(); jx++) {
            for (auto kx = θ.begin(); kx != θ.end(); kx++) {
                for (auto lx = A.begin(); lx != A.end(); lx++) {
                    for (auto mx = δx.begin(); mx != δx.end(); mx++) {
                        for (auto nx = δy.begin(); nx != δy.end(); nx++) {
                            for (auto ox = Φ.begin(); ox != Φ.end(); ox++) {
                                for (auto px = m.begin(); px != m.end(); px++) {
                                    double e = *ix;
                                    double s = *jx;
                                    test t = {
                                        .ε = std::tanh(s * e),
                                        .θ = *kx,
                                        .A = *lx,
                                        .δx = *mx,
                                        .δy = *nx,
                                        .Φ = *ox,
                                        .m = *px,
                                    };

                                    tests.push_back(t);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return tests;
}