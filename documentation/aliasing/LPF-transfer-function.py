"""
sn-vco antialiasing filters - frequency response
"""

from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

def main():
    worN = np.linspace(0,131072, 256)
    X1F1(worN)
    X1F2(worN)
    X2F1(worN)
    X2F2(worN)
    X4F1(worN)
    X4F2(worN)
    pass

# 1x oversampling, 12.5kHz 4th order Butterworth LPF
def X1F1(worN):
    legend = '4th order 12.5kHz Butterworth LPF (44.1kHz)'
    N = 4  # order of filter
    f0 = 12500  # cutoff frequency (Hz)
    fs = 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN, whole=True)

    export('X1F1', w,h, legend)
    plot(legend,w,h,'1x1f')

# 1x oversampling, 2x16kHz 4th order Butterworth LPF
def X1F2(worN):
    legend = '2x16kHz, 4 pole Butterworth LPF transfer function (44.1kHz)'
    N = 4  # order of filter
    f0 = 16000  # cutoff frequency (Hz)
    fs = 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN, whole=True)
    h = h*h

    export('X1F2', w,h, legend)
    plot(legend,w,h,'1x2f')

# 2x oversampling, 16kHz 4th order Butterworth LPF
def X2F1(worN):
    legend = '2x oversampling, 4th order 16kHz Butterworth LPF (44.1kHz)'
    N = 4  # order of filter
    f0 = 16000  # cutoff frequency (Hz)
    fs = 2 * 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN, whole=True)

    export('X2F1',w,h,legend)
    plot(legend,w,h,'2x1f')

# 2x oversampling, 2x16kHz 4th order Butterworth LPF
def X2F2(worN):
    legend = '2x oversampling, 2x 4th order 16kHz Butterworth LPF (44.1kHz)'
    N = 4  # order of filter
    f0 = 16000  # cutoff frequency (Hz)
    fs = 2 * 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN, whole=True)
    h = h * h

    export('X1F2', w,h, legend)
    plot(legend,w,h,'1x2f')

# 4x oversampling, 16kHz 4th order Butterworth LPF
def X4F1(worN):
    legend = '4x oversampling, 4th order 16kHz Butterworth LPF (44.1kHz)'
    N = 4  # order of filter
    f0 = 16000  # cutoff frequency (Hz)
    fs = 4 * 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN, whole=True)

    export('X4F1', w,h, legend)
    plot(legend,w,h,'4x1f')

# 4x oversampling, 2x16kHz 4th order Butterworth LPF
def X4F2(worN):
    legend = '4x oversampling, 2x4th order 16kHz Butterworth LPF (44.1kHz)'
    N = 4  # order of filter
    f0 = 16000  # cutoff frequency (Hz)
    fs = 4 * 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN, whole=True)
    h = h * h

    export('X4F2', w,h, legend)
    plot(legend,w,h,'4x2f')

# Export transfer function to C++
def export(name, w, h, comment):
    ix = 0

    print(f'// {comment}')
    print(f'const std::vector<std::tuple<double, double>> {name} = {{')        
    while ix < len(w):
        print(f'    ', end='')
        for j in range(4):
            print(f'{{ {w[ix]:11.5f}, {abs(h[ix]):7.5f} }},', end='')
            ix = ix+1
        print()
    print('};')
    print()

# Display graphical transfer function
def plot(title, w, h, filename):
    plt.figure(filename)
    plt.semilogx(w, 20 * np.log10(abs(h)))
    plt.title(title)
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Amplitude [dB]')
    plt.xlim(left=1, right=131072)
    plt.ylim(top=10, bottom=-100)
    plt.margins(0, 0.1)
    plt.grid(which='both', axis='both')
    plt.show()

if __name__ == "__main__":
    main()            