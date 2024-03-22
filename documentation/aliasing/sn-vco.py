"""
sn-vco antialiasing filters - frequency response
"""

from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

# 1x oversampling, 12.5kHz 4th order Butterworth LPF
N = 4  # order of filter
f0 = 12500  # cutoff frequency (Hz)
fs = 44100  # sampling frequency (Hz)

b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
w, h = signal.freqz(b, a, fs=fs, worN=128, whole=True)

ix = 0
print('// 12.5kHz, 4 pole Butterworth LPF transfer function (44.1kHz)')
print('const std::vector<std::tuple<double, double>> TF_12500Hz = {')
for i in range(32):
	print(f'    ', end='')
	for j in range(4):
		print(f'{{ {w[ix]:11.5f}, {abs(h[ix]):7.5f} }},', end='')
		ix = ix+1
	print()
print('};')
print()

# plt.figure('12.5kHz')
# plt.semilogx(w, 20 * np.log10(abs(h)))
# plt.title('4th order 12.5kHz Butterworth LPF (44.1kHz)')
# plt.xlabel('Frequency [Hz]')
# plt.ylabel('Amplitude [dB]')
# plt.xlim(left=1, right=44100)
# plt.ylim(top=10, bottom=-100)
# plt.margins(0, 0.1)
# plt.grid(which='both', axis='both')
# plt.show()

# 1x oversampling, 16kHz 4th order Butterworth LPF
N = 4  # order of filter
f0 = 16000  # cutoff frequency (Hz)
fs = 44100  # sampling frequency (Hz)

b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
w, h = signal.freqz(b, a, fs=fs, worN=128, whole=True)

ix = 0
print('// 16kHz, 4 pole Butterworth LPF transfer function (44.1kHz)')
print('const std::vector<std::tuple<double, double>> TF_16kHz = {')
for i in range(32):
	print(f'    ', end='')
	for j in range(4):
		print(f'{{ {w[ix]:11.5f}, {abs(h[ix]):7.5f} }},', end='')
		ix = ix+1
	print()
print('};')
print()

# plt.figure('16kHz')
# plt.semilogx(w, 20 * np.log10(abs(h)))
# plt.title('4th order 16kHz Butterworth LPF (44.1kHz)')
# plt.xlabel('Frequency [Hz]')
# plt.ylabel('Amplitude [dB]')
# plt.xlim(left=1, right=44100)
# plt.ylim(top=10, bottom=-100)
# plt.margins(0, 0.1)
# plt.grid(which='both', axis='both')
# plt.show()

# # 1x oversampling, 2x16kHz 4th order Butterworth LPF
# N = 4  # order of filter
# f0 = 16000  # cutoff frequency (Hz)
# fs = 44100  # sampling frequency (Hz)

# b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
# w, h = signal.freqz(b, a, fs=fs, whole=True)
# h = h * h

# plt.figure('2x16kHz')
# plt.semilogx(w, 20 * np.log10(abs(h)))
# plt.title('2x4th order 16kHz Butterworth LPF (44.1kHz)')
# plt.xlabel('Frequency [Hz]')
# plt.ylabel('Amplitude [dB]')
# plt.xlim(left=1, right=44100)
# plt.ylim(top=10, bottom=-100)
# plt.margins(0, 0.1)
# plt.grid(which='both', axis='both')
# plt.show()

# # 2x oversampling, 16kHz 4th order Butterworth LPF
# N = 4  # order of filter
# f0 = 16000  # cutoff frequency (Hz)
# fs = 2 * 44100  # sampling frequency (Hz)

# b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
# w, h = signal.freqz(b, a, fs=fs, whole=True)

# plt.figure('2x1f')
# plt.semilogx(w, 20 * np.log10(abs(h)))
# plt.title('2x oversampling, 4th order 16kHz Butterworth LPF (44.1kHz)')
# plt.xlabel('Frequency [Hz]')
# plt.ylabel('Amplitude [dB]')
# plt.xlim(left=1, right=44100)
# plt.ylim(top=10, bottom=-100)
# plt.margins(0, 0.1)
# plt.grid(which='both', axis='both')
# plt.show()

# # 2x oversampling, 2x16kHz 4th order Butterworth LPF
# N = 4  # order of filter
# f0 = 16000  # cutoff frequency (Hz)
# fs = 2 * 44100  # sampling frequency (Hz)

# b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
# w, h = signal.freqz(b, a, fs=fs, whole=True)
# h = h * h

# plt.figure('2x2f')
# plt.semilogx(w, 20 * np.log10(abs(h)))
# plt.title('2x oversampling, 2x4th order 16kHz Butterworth LPF (44.1kHz)')
# plt.xlabel('Frequency [Hz]')
# plt.ylabel('Amplitude [dB]')
# plt.xlim(left=1, right=44100)
# plt.ylim(top=10, bottom=-100)
# plt.margins(0, 0.1)
# plt.grid(which='both', axis='both')
# plt.show()

# # 4x oversampling, 16kHz 4th order Butterworth LPF
# N = 4  # order of filter
# f0 = 16000  # cutoff frequency (Hz)
# fs = 4 * 44100  # sampling frequency (Hz)

# b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
# w, h = signal.freqz(b, a, fs=fs, whole=True)

# plt.figure('4x1f')
# plt.semilogx(w, 20 * np.log10(abs(h)))
# plt.title('4x oversampling, 4th order 16kHz Butterworth LPF (44.1kHz)')
# plt.xlabel('Frequency [Hz]')
# plt.ylabel('Amplitude [dB]')
# plt.xlim(left=1, right=44100)
# plt.ylim(top=10, bottom=-100)
# plt.margins(0, 0.1)
# plt.grid(which='both', axis='both')
# plt.show()

# # 4x oversampling, 2x16kHz 4th order Butterworth LPF
# N = 4  # order of filter
# f0 = 16000  # cutoff frequency (Hz)
# fs = 4 * 44100  # sampling frequency (Hz)

# b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
# w, h = signal.freqz(b, a, fs=fs, whole=True)
# h = h * h

# plt.figure('4x2f')
# plt.semilogx(w, 20 * np.log10(abs(h)))
# plt.title('4x oversampling, 2x4th order 16kHz Butterworth LPF (44.1kHz)')
# plt.xlabel('Frequency [Hz]')
# plt.ylabel('Amplitude [dB]')
# plt.xlim(left=1, right=44100)
# plt.ylim(top=10, bottom=-100)
# plt.margins(0, 0.1)
# plt.grid(which='both', axis='both')
# plt.show()
