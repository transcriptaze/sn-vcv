"""
Frequency response scipy 4th order IIR Butterworth filter

cutoff: 16kHz
fs:     44.1kHz 
"""

from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

N = 4  # order of filter
f0 = 16000  # cutoff frequency (Hz)
fs = 44100  # sampling frequency (Hz)

# Butterworth filter (analog)
b, a = signal.butter(N, f0, 'lowpass', analog=True)
w, h = signal.freqs(b, a)

# # Plot frequency response
plt.semilogx(w, 20 * np.log10(abs(h)))
plt.title('Butterworth analog filter frequency response')
plt.xlabel('Frequency [radians / second]')
plt.ylabel('Amplitude [dB]')
plt.margins(0, 0.1)
plt.grid(which='both', axis='both')
plt.axvline(100, color='green')  # cutoff frequency
plt.show()

# Butterworth filter (digital)
b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
w, h = signal.freqz(b, a, fs=fs)

print('a:', a)
print('b:', b)

# scipy
# ----
# a: [1.         1.77400852 1.49725809 0.59777634 0.09650769]
# b: [0.31034692 1.24138766 1.86208149 1.24138766 0.31034692]
#
# Julia
# ----

plt.semilogx(w, 20 * np.log10(abs(h)))
plt.title('Butterworth digital filter frequency response')
plt.xlabel('Frequency [Hz]')
plt.ylabel('Amplitude [dB]')
plt.margins(0, 0.1)
plt.grid(which='both', axis='both')
plt.axvline(100, color='green')  # cutoff frequency
plt.axhline(-3, color='red')  # -3dB
plt.show()
