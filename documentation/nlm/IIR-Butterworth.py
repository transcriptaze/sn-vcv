"""
Generates 4th order Butterworth IIR filter coefficients with a cutoff of around 18kHz @48kHz
"""

from scipy import signal
import matplotlib.pyplot as plt
import numpy as np

FS = 48000
F0 = 0.09375  # normalized cutoff frequency
N = 4  # order of filter

b, a = signal.butter(N, F0, "low", analog=False, output="ba")

dc_gain = np.sum(b) / np.sum(a)

v = { 'f0': F0*FS*4/1000, # kHz, 4x oversampling
      'gain': dc_gain,
      'a1': a[1],
      'a2': a[2],
      'a3': a[3],
      'a4': a[4],
      'b0': b[0],
      'b1': b[1],
      'b2': b[2],
      'b3': b[3],
      'b4': b[4] }

print()
print('// f0 %(f0).1fkHz @48kHz, gain %(gain).3f' % v)
print('struct {')
print('    const double a[4];')
print('    const double b[5];')
print('} IIR {')
print('    .a = {%(a1).6f, %(a2).6f, %(a3).6f, %(a4).6f},' % v)
print('    .b = {%(b0).6f, %(b1).6f, %(b2).6f, %(b3).6f, %(b4).6f},' % v)
print('};')
print()

