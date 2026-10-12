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
- [x] gain
- [x] AUX xy
- [ ] bypass
- (?) gain input

### anti-aliasing
- [x] 4th order Butterworth
- [x] 4th order Bessel
- [ ] 8th order Butterworth
- [ ] 8th order Bessel
- [ ] SOS
- [ ] FIR (scipy.firwin)

```
#include <vector>
#include <iostream>

class FIRFilter {
public:
    // Constructor takes the filter coefficients (taps)
    explicit FIRFilter(const std::vector<double>& coefficients)
        : taps(coefficients), 
          num_taps(coefficients.size()), 
          delay_line(coefficients.size(), 0.0), 
          head(0) {}

    // Process a single sample
    double process(double input) {
        // Store input at the current head position (circular buffer)
        delay_line[head] = input;

        double output = 0.0;
        size_t index = head;

        // Convolution sum: multiply coefficients by delayed samples
        for (size_t i = 0; i < num_taps; ++i) {
            output += taps[i] * delay_line[index];
            
            // Move backward in the circular buffer, wrapping around if necessary
            if (index == 0) {
                index = num_taps - 1;
            } else {
                --index;
            }
        }

        // Advance the head pointer for the next sample
        head = (head + 1) % num_taps;

        return output;
    }

    // Reset filter states
    void reset() {
        std::fill(delay_line.begin(), delay_line.end(), 0.0);
        head = 0;
    }

private:
    std::vector<double> taps;          // Filter coefficients h[k]
    size_t num_taps;                   // Number of taps (e.g., 101)
    std::vector<double> delay_line;    // History buffer x[n-k]
    size_t head;                       // Current index in circular buffer
};
```

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


