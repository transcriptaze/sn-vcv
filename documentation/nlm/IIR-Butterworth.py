"""
Generates 4th order 4x oversampled Butterworth IIR filter

fc:
- 16.5kHz @44.1kHz, 4x oversampling
- 4.5kHz @48kHz
- 9kHz   @48kHz, 2x oversampling
- 18kHz  @48kHz, 4x oversampling
"""

from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

FS = 48000
OVERSAMPLING = 4
F0 = 18000 / (OVERSAMPLING * FS / 2.0)  # normalized cutoff frequency
N = 4  # order of filter

b, a = signal.butter(N, F0, "low", analog=False, output="ba")
w, h = signal.freqz(b, a, fs=FS * OVERSAMPLING)
fc = F0*(FS/2)*OVERSAMPLING
dc_gain = np.sum(b) / np.sum(a)

print()
print(f"// f0       {fc/1000:.1f}kHz @48kHz,4x oversampling")
print(f"// DC gain  {dc_gain:.3f}")
print("constexpr IIR4 BUTTERWORTH {")
print(f"    .a = {{{a[1]:.6f}, {a[2]:.6f}, {a[3]:.6f}, {a[4]:.6f}}},")
print(f"    .b = {{{b[0]:.6f}, {b[1]:.6f}, {b[2]:.6f}, {b[3]:.6f}, {b[4]:.6f}}},")
print("};")
print()

plt.semilogx(w, 20 * np.log10(abs(h)))
plt.title("4th order, 4x oversampled Butterworth IIR filter")
plt.xlabel("Frequency [Hz]")
plt.ylabel("Amplitude [dB]")
plt.margins(0, 0.1)
plt.grid(which="both", axis="both")
plt.axvline(F0, color="green")  # cutoff frequency
plt.axhline(-3, color="red")  # -3dB
plt.show()
