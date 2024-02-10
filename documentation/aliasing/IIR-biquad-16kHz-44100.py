"""
Frequency response using VCV biquad filter coefficients
"""

from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

fs = 44100  # sampling frequency (Hz)

a = [1.000000, 0.870693, 0.329865]
b = [0.550139, 1.100279, 0.550139]
w, h = signal.freqz(b, a, fs=fs)

print('a:', a)
print('b:', b)

plt.semilogx(w, 20 * np.log10(abs(h)))
plt.title('VCV biquad filter frequency response')
plt.xlabel('Frequency [Hz]')
plt.ylabel('Amplitude [dB]')
plt.margins(0, 0.1)
plt.grid(which='both', axis='both')
plt.axvline(100, color='green')  # cutoff frequency
plt.axhline(-3, color='red')  # -3dB
plt.show()
