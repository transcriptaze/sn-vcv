# TODO

## sn-vco
- [ ] [Antialiasing](https://github.com/transcriptaze/sn-vcv/issues/3)
      - [x] Context menu
      - [x] sn-vco
      - [x] sn-vcox
      - [ ] ALIASING.md
      - (?) PID

- [ ] Optimisation
      - [ ] Baseline
      - https://community.vcvrack.com/t/anyone-has-done-profiling-on-a-module-code/7537/
      - https://wordsandbuttons.online/challenge_your_performance_intuition_with_cpp_sine.html
      - https://github.com/grafana/pyroscope
      - https://stackoverflow.com/questions/11445619/profiling-c-on-mac-os-x
      - https://stackoverflow.com/questions/3764978/why-hypot-function-is-so-slow
      - https://mattkretz.github.io/2019/01/18/optimizing-hypot-for-simd.html


- [ ] Set k-rate in ms
- [ ] Move sn_vco::dcblocking and sn_vc::antialias to  DCB and AA

- [x] DC blocking
- [x] Clamp input frequency
- [x] Better blurbs in plugin.json
- [x] Fix tags in plugin.json
- [x] Initialise module in sn-vco-channels
- [x] Invoke parent step() in sn-vc-channels

- [ ] Unit tests

## sn-nlm
- [x] AUX xy
- [x] gain
- [ ] attenuation
- [ ] bypass
- (?) gain input

### antialiasing: input
- [x] 4th order Butterworth
- [x] 4th order Bessel
- [ ] 4th order Elliptic
- [x] FIR
- [ ] 8th order Butterworth
- [ ] 8th order Bessel
- [ ] 8th order Elliptic
- [ ] polyphase
- [ ] SOS

```
#include <vector>
#include <iostream>

class PolyphaseInterpolator4x {
public:
    /// @brief Constructor for a 4x polyphase interpolator
    /// @param taps_per_phase Number of filter taps allocated to each of the 4 sub-filters
    /// @param full_coeffs The complete low-pass filter impulse response (length = 4 * taps_per_phase)
    PolyphaseInterpolator4x(int taps_per_phase, const std::vector<double>& full_coeffs)
        : M(4), P(taps_per_phase), state(P, 0.0), phase_coeffs(M, std::vector<double>(P)) 
    {
        if (full_coeffs.size() != static_cast<size_t>(M * P)) {
            throw std::invalid_argument("Coefficient size must equal 4 * taps_per_phase.");
        }

        // De-interleave the full filter coefficients into 4 polyphase branches
        // h[m*M + k] maps to phase k, tap index m
        for (size_t i = 0; i < full_coeffs.size(); ++i) {
            int phase = i % M;
            int tap_idx = i / M;
            phase_coeffs[phase][tap_idx] = full_coeffs[i];
        }
    }

    /// @brief Process a single input sample and produce 4 interpolated output samples
    /// @param input The new sample at the lower input sample rate
    /// @return A vector containing the 4 output samples at the higher rate
    std::vector<double> process(double input) {
        // 1. Shift the input delay line (stores history of input samples)
        for (int i = P - 1; i > 0; --i) {
            state[i] = state[i - 1];
        }
        state[0] = input;

        // 2. Compute output for each of the 4 phases
        std::vector<double> outputs(M);
        for (int p = 0; p < M; ++p) {
            double acc = 0.0;
            for (int i = 0; i < P; ++i) {
                acc += state[i] * phase_coeffs[p][i];
            }
            outputs[p] = acc;
        }

        return outputs;
    }

    /// @brief Reset the internal delay line/states to zero
    void reset() {
        std::fill(state.begin(), state.end(), 0.0);
    }

private:
    int M;                                  // Interpolation factor (4)
    int P;                                  // Number of taps per phase
    std::vector<double> state;              // Shared input delay line of length P
    std::vector<std::vector<double>> phase_coeffs; // 4 sub-filter coefficient arrays
};
```

```
int main() {
    const int taps_per_phase = 8;
    const int total_taps = 4 * taps_per_phase; // 32 taps total

    // Dummy prototype coefficients (in a real app, generate these using windowed sinc / Parks-McClellan)
    std::vector<double> prototype_coeffs(total_taps, 0.1); 

    // Instantiate the 4x interpolator
    PolyphaseInterpolator4x interpolator(taps_per_phase, prototype_coeffs);

    // Simulate feeding input samples
    std::vector<double> input_stream = {1.0, 0.5, -0.5, 0.0};

    std::cout << "Processing input stream with 4x interpolation:\n";
    for (double x : input_stream) {
        std::vector<double> upsampled = interpolator.process(x);
        
        std::cout << "Input: " << x << " -> Outputs: [ ";
        for (double out : upsampled) {
            std::cout << out << " ";
        }
        std::cout << "]\n";
    }

    return 0;
}
```
### antialiasing: output
- [ ] 4th order Butterworth
- [ ] 4th order Bessel
- [ ] 4th order Elliptic
- [ ] 8th order Butterworth
- [ ] 8th order Bessel
- [ ] 8th order Elliptic


## R & D

1. https://www.quantamagazine.org/mathematicians-identify-the-best-versions-of-iconic-shapes-20240105/#
2. https://www.quantamagazine.org/geometers-engineer-new-tools-to-wrangle-spacecraft-orbits-20240415/
3. https://www.quantamagazine.org/elliptic-curve-murmurations-found-with-ai-take-flight-20240305/
4. https://www.youtube.com/watch?v=fxzrpfezezE
5. https://www.youtube.com/watch?v=3depB4EZT7c
6. https://www.soundonsound.com/techniques/synthesizing-strings-string-machines


## Notes and References

1. https://github.com/qno/vcv-plugin-github-actions-example/blob/main/.github/workflows/build-plugin.yml
2. https://community.vcvrack.com/t/help-needed-for-beta-testing/20711/30
3. https://community.vcvrack.com/t/automated-building-and-releasing-plugins-on-github-with-github-actions/11364
4. https://community.vcvrack.com/t/rack-development-blog/5864/84
5. https://herbie.uwplse.org
6. `convert *.png -crop 392x331+281+90 cropped/screenshot%d.png`
7. https://github.com/nullJaX/vcvrack-patchable-devices
8. https://www.youtube.com/watch?v=yV52TMdGkng
9. https://surge-synthesizer.github.io/manual-xt/#string


