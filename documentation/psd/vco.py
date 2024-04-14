import csv
import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import rfft, rfftfreq


def main():
    y = sn()

    # plt.figure('VCO')
    # plt.plot(y)
    # plt.title('VCO: 1Hz generated')
    # plt.xlabel('t')
    # plt.ylabel('V')
    # plt.margins(0, 0.1)
    # plt.grid(which='both', axis='both')
    # plt.show()

    sp = rfft(y)
    freq = rfftfreq(512, d=1 / 512)

    plt.figure('VCO')
    plt.plot(freq, sp.real)
    plt.title('FFT 1Hz, 512 samples')
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Amplitude [dB]')
    plt.xlim(left=0, right=64)
    plt.margins(0, 0.1)
    plt.grid(which='both', axis='both')
    plt.show()


def sn():
    sn = []
    with open("./workdir/sn-vco-samples.tsv") as f:
        r = csv.reader(f, delimiter="\t", quotechar='"')
        next(r)
        for row in r:
            sn.append(float(f'{row[1]}'))

    return sn


if __name__ == "__main__":
    main()
