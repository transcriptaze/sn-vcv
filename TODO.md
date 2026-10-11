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
- [ ] 4th order Bessel
- [ ] 8th order Butterworth
- [ ] 8th order Bessel
- [ ] FIR (scipy.firwin)

```
import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

# 1. Specification Parameters
fs = 1000.0         # Sampling frequency (Hz)
cutoff = 150.0      # Desired cutoff frequency (Hz)
numtaps = 101       # Filter length (number of coefficients/taps). Must be odd for type I linear phase.

# 2. Design the Linear-Phase Low-Pass FIR Filter
# firwin automatically creates a linear phase filter using the window method (default: 'hamming')
taps = signal.firwin(numtaps, cutoff, window='hamming', fs=fs)

# 3. Analyze Frequency and Phase Response
w, h = signal.freqz(taps, fs=fs)

# 4. Apply the Filter to a Noisy Signal
t = np.arange(0, 1.0, 1/fs)
# Signal consists of a 25 Hz tone + a 300 Hz tone (noise)
clean_signal = np.sin(2 * np.pi * 25 * t)
noisy_signal = clean_signal + 0.5 * np.sin(2 * np.pi * 300 * t)

# Use lfilter (or lfilter_zi) for FIR filtering. 
# Because the filter is linear phase, it delays the signal by (numtaps - 1) / 2 samples.
filtered_signal = signal.lfilter(taps, 1.0, noisy_signal)

# --- Visualization ---
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))

# Frequency Response Magnitude
ax1.plot(w, 20 * np.log10(np.abs(h)), color='b')
ax1.set_title("Low-Pass FIR Filter Frequency Response (Linear Phase)")
ax1.set_xlabel("Frequency (Hz)")
ax1.set_ylabel("Gain (dB)")
ax1.grid(True)

# Time Domain Filtering Result
ax2.plot(t, noisy_signal, label='Noisy Signal', alpha=0.5)
ax2.plot(t, filtered_signal, label='Filtered Signal (Linear Phase)', linewidth=2, color='r')
ax2.set_title("Time Domain Filtering")
ax2.set_xlabel("Time (s)")
ax2.set_ylabel("Amplitude")
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.show()
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


