from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

N = 4       # order of filter
f0 = 100    # cutoff frequency (Hz)
fs = 500  # sampling frequency (Hz)

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
plt.axvline(100, color='green') # cutoff frequency
plt.show()


# Butterworth filter (digital)
b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
w, h = signal.freqz(b, a, fs=fs)

print('a:',a)
print('b:',b)

# scipy
# ----
# a: [ 1.         -0.7820952   0.67997853 -0.1826757   0.03011888]
# b: [0.04658291 0.18633163 0.27949744 0.18633163 0.04658291]
#
# Julia
# ----
# a: [1.0, -0.7820951980233379, 0.6799785269162996, -0.1826756977530324, 0.030118875043169253]
# b: [0.04658290663644366, 0.18633162654577465, 0.279497439818662, 0.18633162654577465, 0.04658290663644366]


plt.semilogx(w, 20 * np.log10(abs(h)))
plt.title('Butterworth digital filter frequency response')
plt.xlabel('Frequency [Hz]')
plt.ylabel('Amplitude [dB]')
plt.margins(0, 0.1)
plt.grid(which='both', axis='both')
plt.axvline(100, color='green') # cutoff frequency
plt.axhline(-3, color='red')    # -3dB
plt.show()

