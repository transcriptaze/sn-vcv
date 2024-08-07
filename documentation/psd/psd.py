import csv
import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import rfft, rfftfreq


def main():
    y = sn()

    # plt.figure('PSD')
    # plt.plot(y)
    # plt.title('PSD: 25Hz AUX IN')
    # plt.xlabel('t')
    # plt.ylabel('V')
    # plt.margins(0, 0.1)
    # plt.grid(which='both', axis='both')
    # plt.show()

    sp = rfft(y)
    freq = rfftfreq(1764, d=1 / 44100)

    plt.figure('PSD')
    plt.plot(freq, sp.real)
    plt.title('FFT 25Hz, 1764 samples')
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Amplitude [dB]')
    # plt.xlim(left=0, right=2500)
    plt.xlim(left=0, right=1600)
    plt.margins(0, 0.1)
    plt.grid(which='both', axis='both')
    plt.show()


def sn():
    sn = []
    with open("./workdir/sn-psd-samples.tsv") as f:
        r = csv.reader(f, delimiter="\t", quotechar='"')
        next(r)
        for row in r:
            sn.append(float(f'{row[1]}'))

    return sn


if __name__ == "__main__":
    main()
