#pragma once

#include <tuple>
#include <vector>

#include "../sn.hpp"

struct TF {
    static const double Δf;
    static const double FS;
    static const std::vector<std::tuple<double, double>> X1F1;
    static const std::vector<std::tuple<double, double>> X1F2;
    static const std::vector<std::tuple<double, double>> X2F1;
    static const std::vector<std::tuple<double, double>> X2F2;
    static const std::vector<std::tuple<double, double>> X4F1;
    static const std::vector<std::tuple<double, double>> X4F2;

    static double interpolate(ANTIALIAS mode, const double f);
};
