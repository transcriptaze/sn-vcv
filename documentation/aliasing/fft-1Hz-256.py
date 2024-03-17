# 1Hz square wave
# 256 samples

from scipy.fft import rfft, fftfreq, fftshift
import numpy as np
import matplotlib.pyplot as plt

def main():
    y = sn()
    sp = rfft(y)
    freq = fftfreq(129,d=1/256)

    print(len(sp))
    plt.figure('FFT-1Hz-256')
    plt.plot(sp.real)
    plt.title('FFT 1Hz, 256 samples')
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Amplitude [dB]')
#   plt.xlim(left=0, right=64)
    plt.margins(0, 0.1)
    plt.grid(which='both', axis='both')
    plt.show()


def sn():
    return [  
              0.87663,  0.96438,  0.98372,  0.99077,  0.99409,  0.99590,  0.99700,  0.99771,
              0.99820,  0.99856,  0.99882,  0.99901,  0.99917,  0.99929,  0.99939,  0.99947,
              0.99954,  0.99959,  0.99964,  0.99968,  0.99972,  0.99975,  0.99977,  0.99980,
              0.99982,  0.99984,  0.99985,  0.99987,  0.99988,  0.99989,  0.99990,  0.99991,
              0.99992,  0.99993,  0.99993,  0.99994,  0.99994,  0.99995,  0.99995,  0.99996,
              0.99996,  0.99997,  0.99997,  0.99997,  0.99998,  0.99998,  0.99998,  0.99998,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99998,
              0.99998,  0.99998,  0.99998,  0.99997,  0.99997,  0.99997,  0.99996,  0.99996,
              0.99995,  0.99995,  0.99994,  0.99994,  0.99993,  0.99993,  0.99992,  0.99991,
              0.99990,  0.99989,  0.99988,  0.99987,  0.99985,  0.99984,  0.99982,  0.99980,
              0.99977,  0.99975,  0.99972,  0.99968,  0.99964,  0.99959,  0.99954,  0.99947,
              0.99939,  0.99929,  0.99917,  0.99901,  0.99882,  0.99856,  0.99820,  0.99771,
              0.99700,  0.99590,  0.99409,  0.99077,  0.98372,  0.96438,  0.87663, -0.00001,
             -0.87663, -0.96438, -0.98372, -0.99077, -0.99409, -0.99590, -0.99700, -0.99771,
             -0.99820, -0.99856, -0.99882, -0.99901, -0.99917, -0.99929, -0.99939, -0.99947,
             -0.99954, -0.99959, -0.99964, -0.99968, -0.99972, -0.99975, -0.99977, -0.99980,
             -0.99982, -0.99984, -0.99985, -0.99987, -0.99988, -0.99989, -0.99990, -0.99991,
             -0.99992, -0.99993, -0.99993, -0.99994, -0.99994, -0.99995, -0.99995, -0.99996,
             -0.99996, -0.99997, -0.99997, -0.99997, -0.99998, -0.99998, -0.99998, -0.99998,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99998,
             -0.99998, -0.99998, -0.99998, -0.99997, -0.99997, -0.99997, -0.99996, -0.99996,
             -0.99995, -0.99995, -0.99994, -0.99994, -0.99993, -0.99993, -0.99992, -0.99991,
             -0.99990, -0.99989, -0.99988, -0.99987, -0.99985, -0.99984, -0.99982, -0.99980,
             -0.99977, -0.99975, -0.99972, -0.99968, -0.99964, -0.99959, -0.99954, -0.99947,
             -0.99939, -0.99929, -0.99917, -0.99901, -0.99882, -0.99856, -0.99820, -0.99771,
             -0.99700, -0.99590, -0.99409, -0.99077, -0.98372, -0.96438, -0.87663,  0.00000,
          ]

if __name__ == "__main__":
    main()            