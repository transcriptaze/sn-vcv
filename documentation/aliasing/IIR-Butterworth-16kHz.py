"""
Generates 4th order Butterworth IIR filter coefficients for all the VCV sampling rates.
"""

from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

FS = [11025, 12000, 22050, 24000, 44100, 48000, 88200, 96000, 176400, 192000, 352800, 384000, 705600, 768000]
F0 = 16000  # cutoff frequency (Hz)
N = 4  # order of filter

rates = set()
for fs in FS:
    rates.add(fs)
    rates.add(fs * 2)
    rates.add(fs * 4)

print('const std::map<int, IIR> COEFFICIENTS_16kHz = {')

for fs in sorted(rates):
    f0 = 0.3 * fs if fs < 44100 else 16000
    b, a = signal.butter(N, f0, 'lowpass', analog=False, fs=fs)

    v = {
        'fs': fs,
        'f0': f0,
        'a1': a[1],
        'a2': a[2],
        'a3': a[3],
        'a4': a[4],
        'b0': b[0],
        'b1': b[1],
        'b2': b[2],
        'b3': b[3],
        'b4': b[4],
    }

    print('    {')
    print('        %(fs)d,' % v)
    print('        IIR{')
    print('            .a = {%(a1).6f, %(a2).6f, %(a3).6f, %(a4).6f},' % v)
    print('            .b = {%(b0).6f, %(b1).6f, %(b2).6f, %(b3).6f, %(b4).6f}' % v)
    print('        },')
    print('    },')

print('};')
print()
