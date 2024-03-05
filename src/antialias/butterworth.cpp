#include <limits>

#include "butterworth.h"
#include "plugin.hpp"

IIR coefficients(const std::map<int, IIR> &table, float fs) {
    int f = static_cast<int>(std::round(fs));

    auto it = table.find(f);
    if (it != table.end()) {
        return it->second;
    }

    // ... fallback: pick nearest (assumes table is sorted low-to-high)
    float delta = std::numeric_limits<float>::max();
    int ix = 0;
    float d;
    for (const auto &it : table) {
        if ((d = abs(it.first - fs)) < delta) {
            ix = it.first;
            delta = d;
        } else {
            return table.at(ix);
        }
    }

    return table.at(44100);
}
