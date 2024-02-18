#pragma once

#include "plugin.hpp"

template <int B_ORDER, int A_ORDER, typename T = float>
struct MultiChannelFilter {
    dsp::IIRFilter<B_ORDER, A_ORDER, T> lpf[16];
    size_t channels = 0;

    MultiChannelFilter() {
    }

    void reset() {
        if (channels > 0) {
            for (size_t i = 0; i < 16; i++) {
                lpf[i].reset();
            }

            channels = 0;
        }
    }

    void setCoefficients(const T *b, const T *a) {
        for (size_t i = 0; i < 16; i++) {
            lpf[i].setCoefficients(b, a);
        }
    }

    // void setChannels(size_t N) {
    //     if (N != channels) {
    //         for (size_t i = N; i < 16; i++) {
    //             lpf[i].reset();
    //         }
    //
    //         channels = N;
    //     }
    // }

    // T process(size_t ch, const T in) {
    //     return lpf[ch].process(in);
    // }

    void process(const T *in, T *out, size_t N) {
        for (size_t i = 0; i < N; i++) {
            out[i] = lpf[i].process(in[i]);
        }

        if (N != channels) {
            for (size_t i = N; i < 16; i++) {
                lpf[i].reset();
            }

            channels = N;
        }
    }
};