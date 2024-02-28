#pragma once

#include "../plugin.hpp"
#include "../sn.hpp"

/* Anti-aliasing filter */

template <ANTI_ALIAS CUTOFF>
struct AAF {
	AAF() {
    }

    void process(const double *in, double *out, size_t channels) {
    	for (size_t i = 0; i < channels; i++) {
        	out[i] = in[i];
    	}
    }
};