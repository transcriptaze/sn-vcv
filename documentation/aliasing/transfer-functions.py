"""
sn-vco antialiasing filters - frequency response
"""

from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

def main():
    worN = np.linspace(0,4*48000, num=376, endpoint=False)

    w,h,name,legend = X1F1(worN)
    export(name, w,h, legend)
    # tsv(name,w,h)
    # plot(legend,w,h,name)

    w,h,name,legend = X1F2(worN)
    export(name, w,h, legend)
    # tsv(name,w,h)
    # plot(legend,w,h,name)

    w,h,name,legend = X2F1(worN)
    export(name, w,h, legend)
    # tsv(name,w,h)
    # plot(legend,w,h,name)

    export(name, w,h, legend)
    # tsv(name,w,h)
    # plot(legend,w,h,name)

    w,h,name,legend = X4F1(worN)
    export(name, w,h, legend)
    # tsv(name,w,h)
    # plot(legend,w,h,name)

    w,h,name,legend = X4F2(worN)
    export(name, w,h, legend)
    # tsv(name,w,h)
    # plot(legend,w,h,name)

# 1x oversampling, 12.5kHz 4th order Butterworth LPF
def X1F1(worN):
    name = 'X1F1'
    legend = '4th order 12.5kHz Butterworth LPF (44.1kHz)'
    N = 4  # order of filter
    f0 = 12500  # cutoff frequency (Hz)
    fs = 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN)

    return w,h, name, legend

# 1x oversampling, 2x16kHz 4th order Butterworth LPF
def X1F2(worN):
    name = 'X1F2'
    legend = '2x16kHz, 4 pole Butterworth LPF transfer function (44.1kHz)'
    N = 4  # order of filter
    f0 = 16000  # cutoff frequency (Hz)
    fs = 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN, whole=True)
    h = h*h

    return w,h, name, legend


# 2x oversampling, 16kHz 4th order Butterworth LPF
def X2F1(worN):
    name = 'X2F1'
    legend = '2x oversampling, 4th order 16kHz Butterworth LPF (44.1kHz)'
    N = 4  # order of filter
    f0 = 16000  # cutoff frequency (Hz)
    fs = 2 * 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN, whole=True)

    return w,h, name, legend

# 2x oversampling, 2x16kHz 4th order Butterworth LPF
def X2F2(worN):
    name = 'X2F2'
    legend = '2x oversampling, 2x 4th order 16kHz Butterworth LPF (44.1kHz)'
    N = 4  # order of filter
    f0 = 16000  # cutoff frequency (Hz)
    fs = 2 * 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN, whole=True)
    h = h * h

    return w,h, name, legend

# 4x oversampling, 16kHz 4th order Butterworth LPF
def X4F1(worN):
    name = 'X4F1'
    legend = '4x oversampling, 4th order 16kHz Butterworth LPF (44.1kHz)'
    N = 4  # order of filter
    f0 = 16000  # cutoff frequency (Hz)
    fs = 4 * 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN, whole=True)

    return w,h, name, legend

# 4x oversampling, 2x16kHz 4th order Butterworth LPF
def X4F2(worN):
    name = 'X4F2'
    legend = '4x oversampling, 2x4th order 16kHz Butterworth LPF (44.1kHz)'
    N = 4  # order of filter
    f0 = 16000  # cutoff frequency (Hz)
    fs = 4 * 44100  # sampling frequency (Hz)

    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)
    w, h = signal.freqz(b, a, fs=fs, worN=worN, whole=True)
    h = h * h

    return w,h, name, legend

# Export transfer function to C++
def export(name, w, h, comment):
    ix = 0

    print(f'// {comment}')
    print(f'const std::vector<std::tuple<double, double>> {name} = {{')        
    while ix < len(w):
        print(f'    ', end='')
        for j in range(4):
            print(f'{{ {w[ix]:8.1f}, {abs(h[ix]):7.5f} }},', end='')
            ix = ix+1
        print()
    print('};')
    print()

# Export transfer function to TSV
def tsv(name, w, h):
    ix = 0

    print(f'{name}')
    while ix < len(w):
        print(f'{w[ix]:8.1f}\t{abs(h[ix]):7.5f}')
        ix = ix+1
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