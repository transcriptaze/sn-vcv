import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

FS = 48000
OVERSAMPLING = 4
F0 = 5000 / (OVERSAMPLING * FS / 2.0)  # normalized cutoff frequency
NUMTAPS = 101

taps = signal.firwin(NUMTAPS, F0, window='hamming')
w, h = signal.freqz(taps, fs=FS * OVERSAMPLING)

ix = 0
while ix < 101:
    jx = 0
    print("      ", end="")
    while ix < 101 and jx<4:
        print(f" {taps[ix]: .8e},", end="")
        ix += 1
        jx += 1
    print("")


plt.semilogx(w, 20 * np.log10(abs(h)))
plt.title("FIR filter")
plt.xlabel("Frequency [Hz]")
plt.ylabel("Amplitude [dB]")
plt.margins(0, 0.1)
plt.grid(which="both", axis="both")
plt.axvline(F0, color="green")  # cutoff frequency
plt.axhline(-3, color="red")  # -3dB
plt.show()
