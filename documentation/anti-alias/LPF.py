from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

N = 4       # order of filter
f0 = 100    # cutoff frequency (Hz)
fs = 500  # sampling frequency (Hz)

# Butterworth filter (analog)
b, a = signal.butter(N, f0, 'lowpass', analog=True)
# w, h = signal.freqz(b, a,worN=8000)
w, h = signal.freqs(b, a)

# # Plot frequency response
plt.semilogx(w, 20 * np.log10(abs(h)))
plt.title('Butterworth filter frequency response')
plt.xlabel('Frequency [radians / second]')
plt.ylabel('Amplitude [dB]')
plt.margins(0, 0.1)
plt.grid(which='both', axis='both')
plt.axvline(100, color='green') # cutoff frequency
plt.show()


# Butterworth filter (digital)
b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
w, h = signal.freqz(b, a, fs=fs)

plt.semilogx(w, 20 * np.log10(abs(h)))
plt.title('Butterworth filter frequency response')
plt.xlabel('Frequency [Hz]')
plt.ylabel('Amplitude [dB]')
plt.margins(0, 0.1)
plt.grid(which='both', axis='both')
plt.axvline(100, color='green') # cutoff frequency
plt.axhline(-3, color='red')    # -3dB
plt.show()




# # https://stackoverflow.com/questions/25191620/creating-lowpass-filter-in-scipy-understanding-methods-and-units
# def butter_lowpass(cutoff, fs, order=5):
#     return signal.butter(order, cutoff, fs=fs, btype='low', analog=False)
# 
# def butter_lowpass_filter(data, cutoff, fs, order=5):
#     b, a = butter_lowpass(cutoff, fs, order=order)
#     y = signal.lfilter(b, a, data)
#     return y
# 
# 
# # Plot frequency response
# 
# plt.subplot(2, 1, 1)
# plt.plot(w, np.abs(h), 'b')
# plt.plot(f0, 0.5*np.sqrt(2), 'ko')
# plt.axvline(f0, color='k')
# plt.xlim(0, 0.5*fs)
# plt.title("Lowpass Filter Frequency Response")
# plt.xlabel('Frequency [Hz]')
# plt.grid()
# 
# # Demonstrate the use of the filter.
# # First make some data to be filtered.
# T = 5.0         # seconds
# n = int(T * fs) # total number of samples
# t = np.linspace(0, T, n, endpoint=False)
# # "Noisy" data.  We want to recover the 1.2 Hz signal from this.
# data = np.sin(1.2*2*np.pi*t) + 1.5*np.cos(9*2*np.pi*t) + 0.5*np.sin(12.0*2*np.pi*t)
# 
# # Filter the data, and plot both the original and filtered signals.
# y = butter_lowpass_filter(data, f0, fs, N)
# 
# plt.subplot(2, 1, 2)
# plt.plot(t, data, 'b-', label='data')
# plt.plot(t, y, 'g-', linewidth=2, label='filtered data')
# plt.xlabel('Time [sec]')
# plt.grid()
# plt.legend()
# 
# plt.subplots_adjust(hspace=0.35)
# plt.show()
# 
