# 1Hz square wave
# 2048 samples

from scipy.fft import rfft, fftfreq, fftshift
import numpy as np
import matplotlib.pyplot as plt

def main():
    y = sn()
    sp = rfft(y)
    freq = fftfreq(1025,d=1/2048)

    print(len(sp))
    plt.figure('FFT-1Hz-2048')
    plt.plot(sp.real)
    plt.title('FFT 1Hz, 2048 samples')
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Amplitude [dB]')
#   plt.xlim(left=0, right=64)
    plt.margins(0, 0.1)
    plt.grid(which='both', axis='both')
    plt.show()


def sn():
    return [  
              0.22201,  0.41444,  0.56405,  0.67337,  0.75134,  0.80695,  0.84711,  0.87663,
              0.89875,  0.91563,  0.92876,  0.93913,  0.94745,  0.95420,  0.95976,  0.96438,
              0.96827,  0.97155,  0.97436,  0.97678,  0.97888,  0.98071,  0.98231,  0.98372,
              0.98497,  0.98609,  0.98709,  0.98798,  0.98878,  0.98951,  0.99017,  0.99077,
              0.99132,  0.99182,  0.99228,  0.99270,  0.99309,  0.99345,  0.99378,  0.99409,
              0.99437,  0.99464,  0.99488,  0.99511,  0.99533,  0.99553,  0.99572,  0.99590,
              0.99607,  0.99622,  0.99637,  0.99651,  0.99664,  0.99677,  0.99689,  0.99700,
              0.99710,  0.99721,  0.99730,  0.99739,  0.99748,  0.99756,  0.99764,  0.99771,
              0.99778,  0.99785,  0.99792,  0.99798,  0.99804,  0.99810,  0.99815,  0.99820,
              0.99825,  0.99830,  0.99835,  0.99839,  0.99844,  0.99848,  0.99852,  0.99856,
              0.99859,  0.99863,  0.99866,  0.99870,  0.99873,  0.99876,  0.99879,  0.99882,
              0.99884,  0.99887,  0.99890,  0.99892,  0.99895,  0.99897,  0.99899,  0.99901,
              0.99904,  0.99906,  0.99908,  0.99910,  0.99912,  0.99913,  0.99915,  0.99917,
              0.99919,  0.99920,  0.99922,  0.99923,  0.99925,  0.99926,  0.99928,  0.99929,
              0.99931,  0.99932,  0.99933,  0.99934,  0.99936,  0.99937,  0.99938,  0.99939,
              0.99940,  0.99941,  0.99942,  0.99943,  0.99944,  0.99945,  0.99946,  0.99947,
              0.99948,  0.99949,  0.99950,  0.99951,  0.99951,  0.99952,  0.99953,  0.99954,
              0.99955,  0.99955,  0.99956,  0.99957,  0.99957,  0.99958,  0.99959,  0.99959,
              0.99960,  0.99961,  0.99961,  0.99962,  0.99962,  0.99963,  0.99964,  0.99964,
              0.99965,  0.99965,  0.99966,  0.99966,  0.99967,  0.99967,  0.99968,  0.99968,
              0.99969,  0.99969,  0.99970,  0.99970,  0.99970,  0.99971,  0.99971,  0.99972,
              0.99972,  0.99973,  0.99973,  0.99973,  0.99974,  0.99974,  0.99974,  0.99975,
              0.99975,  0.99975,  0.99976,  0.99976,  0.99976,  0.99977,  0.99977,  0.99977,
              0.99978,  0.99978,  0.99978,  0.99979,  0.99979,  0.99979,  0.99979,  0.99980,
              0.99980,  0.99980,  0.99980,  0.99981,  0.99981,  0.99981,  0.99981,  0.99982,
              0.99982,  0.99982,  0.99982,  0.99983,  0.99983,  0.99983,  0.99983,  0.99984,
              0.99984,  0.99984,  0.99984,  0.99984,  0.99985,  0.99985,  0.99985,  0.99985,
              0.99985,  0.99985,  0.99986,  0.99986,  0.99986,  0.99986,  0.99986,  0.99987,
              0.99987,  0.99987,  0.99987,  0.99987,  0.99987,  0.99987,  0.99988,  0.99988,
              0.99988,  0.99988,  0.99988,  0.99988,  0.99989,  0.99989,  0.99989,  0.99989,
              0.99989,  0.99989,  0.99989,  0.99989,  0.99990,  0.99990,  0.99990,  0.99990,
              0.99990,  0.99990,  0.99990,  0.99990,  0.99991,  0.99991,  0.99991,  0.99991,
              0.99991,  0.99991,  0.99991,  0.99991,  0.99991,  0.99992,  0.99992,  0.99992,
              0.99992,  0.99992,  0.99992,  0.99992,  0.99992,  0.99992,  0.99992,  0.99993,
              0.99993,  0.99993,  0.99993,  0.99993,  0.99993,  0.99993,  0.99993,  0.99993,
              0.99993,  0.99993,  0.99993,  0.99994,  0.99994,  0.99994,  0.99994,  0.99994,
              0.99994,  0.99994,  0.99994,  0.99994,  0.99994,  0.99994,  0.99994,  0.99994,
              0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99995,
              0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99995,
              0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99996,
              0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99996,
              0.99996,  0.99996,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,
              0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,
              0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,
              0.99997,  0.99997,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,
              0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,
              0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,
              0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,
              0.99998,  0.99998,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,  1.00000,
              1.00000,  1.00000,  1.00000,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99999,
              0.99999,  0.99999,  0.99999,  0.99999,  0.99999,  0.99998,  0.99998,  0.99998,
              0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,
              0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,
              0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99998,
              0.99998,  0.99998,  0.99998,  0.99998,  0.99998,  0.99997,  0.99997,  0.99997,
              0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,
              0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99997,
              0.99997,  0.99997,  0.99997,  0.99997,  0.99997,  0.99996,  0.99996,  0.99996,
              0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99996,
              0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99996,  0.99995,
              0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99995,
              0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99995,  0.99994,
              0.99994,  0.99994,  0.99994,  0.99994,  0.99994,  0.99994,  0.99994,  0.99994,
              0.99994,  0.99994,  0.99994,  0.99994,  0.99993,  0.99993,  0.99993,  0.99993,
              0.99993,  0.99993,  0.99993,  0.99993,  0.99993,  0.99993,  0.99993,  0.99993,
              0.99992,  0.99992,  0.99992,  0.99992,  0.99992,  0.99992,  0.99992,  0.99992,
              0.99992,  0.99992,  0.99991,  0.99991,  0.99991,  0.99991,  0.99991,  0.99991,
              0.99991,  0.99991,  0.99991,  0.99990,  0.99990,  0.99990,  0.99990,  0.99990,
              0.99990,  0.99990,  0.99990,  0.99989,  0.99989,  0.99989,  0.99989,  0.99989,
              0.99989,  0.99989,  0.99989,  0.99988,  0.99988,  0.99988,  0.99988,  0.99988,
              0.99988,  0.99987,  0.99987,  0.99987,  0.99987,  0.99987,  0.99987,  0.99987,
              0.99986,  0.99986,  0.99986,  0.99986,  0.99986,  0.99985,  0.99985,  0.99985,
              0.99985,  0.99985,  0.99985,  0.99984,  0.99984,  0.99984,  0.99984,  0.99984,
              0.99983,  0.99983,  0.99983,  0.99983,  0.99982,  0.99982,  0.99982,  0.99982,
              0.99981,  0.99981,  0.99981,  0.99981,  0.99980,  0.99980,  0.99980,  0.99980,
              0.99979,  0.99979,  0.99979,  0.99979,  0.99978,  0.99978,  0.99978,  0.99977,
              0.99977,  0.99977,  0.99976,  0.99976,  0.99976,  0.99975,  0.99975,  0.99975,
              0.99974,  0.99974,  0.99974,  0.99973,  0.99973,  0.99973,  0.99972,  0.99972,
              0.99971,  0.99971,  0.99970,  0.99970,  0.99970,  0.99969,  0.99969,  0.99968,
              0.99968,  0.99967,  0.99967,  0.99966,  0.99966,  0.99965,  0.99965,  0.99964,
              0.99964,  0.99963,  0.99962,  0.99962,  0.99961,  0.99961,  0.99960,  0.99959,
              0.99959,  0.99958,  0.99957,  0.99957,  0.99956,  0.99955,  0.99955,  0.99954,
              0.99953,  0.99952,  0.99951,  0.99951,  0.99950,  0.99949,  0.99948,  0.99947,
              0.99946,  0.99945,  0.99944,  0.99943,  0.99942,  0.99941,  0.99940,  0.99939,
              0.99938,  0.99937,  0.99936,  0.99934,  0.99933,  0.99932,  0.99931,  0.99929,
              0.99928,  0.99926,  0.99925,  0.99923,  0.99922,  0.99920,  0.99919,  0.99917,
              0.99915,  0.99913,  0.99912,  0.99910,  0.99908,  0.99906,  0.99904,  0.99901,
              0.99899,  0.99897,  0.99895,  0.99892,  0.99890,  0.99887,  0.99884,  0.99882,
              0.99879,  0.99876,  0.99873,  0.99870,  0.99866,  0.99863,  0.99859,  0.99856,
              0.99852,  0.99848,  0.99844,  0.99839,  0.99835,  0.99830,  0.99825,  0.99820,
              0.99815,  0.99810,  0.99804,  0.99798,  0.99792,  0.99785,  0.99778,  0.99771,
              0.99764,  0.99756,  0.99748,  0.99739,  0.99730,  0.99721,  0.99710,  0.99700,
              0.99689,  0.99677,  0.99664,  0.99651,  0.99637,  0.99622,  0.99607,  0.99590,
              0.99572,  0.99553,  0.99533,  0.99511,  0.99488,  0.99464,  0.99437,  0.99409,
              0.99378,  0.99345,  0.99309,  0.99270,  0.99228,  0.99182,  0.99132,  0.99077,
              0.99017,  0.98951,  0.98878,  0.98798,  0.98709,  0.98609,  0.98497,  0.98372,
              0.98231,  0.98071,  0.97888,  0.97678,  0.97436,  0.97155,  0.96827,  0.96438,
              0.95976,  0.95420,  0.94745,  0.93913,  0.92876,  0.91563,  0.89875,  0.87663,
              0.84711,  0.80695,  0.75134,  0.67337,  0.56405,  0.41443,  0.22200, -0.00001,
             -0.22202, -0.41444, -0.56406, -0.67336, -0.75134, -0.80695, -0.84711, -0.87663,
             -0.89875, -0.91563, -0.92876, -0.93913, -0.94745, -0.95420, -0.95976, -0.96438,
             -0.96827, -0.97155, -0.97436, -0.97678, -0.97888, -0.98071, -0.98231, -0.98372,
             -0.98497, -0.98609, -0.98709, -0.98798, -0.98878, -0.98951, -0.99017, -0.99077,
             -0.99132, -0.99182, -0.99228, -0.99270, -0.99309, -0.99345, -0.99378, -0.99409,
             -0.99437, -0.99464, -0.99488, -0.99511, -0.99533, -0.99553, -0.99572, -0.99590,
             -0.99607, -0.99622, -0.99637, -0.99651, -0.99664, -0.99677, -0.99689, -0.99700,
             -0.99710, -0.99721, -0.99730, -0.99739, -0.99748, -0.99756, -0.99764, -0.99771,
             -0.99778, -0.99785, -0.99792, -0.99798, -0.99804, -0.99810, -0.99815, -0.99820,
             -0.99825, -0.99830, -0.99835, -0.99839, -0.99844, -0.99848, -0.99852, -0.99856,
             -0.99859, -0.99863, -0.99866, -0.99870, -0.99873, -0.99876, -0.99879, -0.99882,
             -0.99884, -0.99887, -0.99890, -0.99892, -0.99895, -0.99897, -0.99899, -0.99901,
             -0.99904, -0.99906, -0.99908, -0.99910, -0.99912, -0.99913, -0.99915, -0.99917,
             -0.99919, -0.99920, -0.99922, -0.99923, -0.99925, -0.99926, -0.99928, -0.99929,
             -0.99931, -0.99932, -0.99933, -0.99934, -0.99936, -0.99937, -0.99938, -0.99939,
             -0.99940, -0.99941, -0.99942, -0.99943, -0.99944, -0.99945, -0.99946, -0.99947,
             -0.99948, -0.99949, -0.99950, -0.99951, -0.99951, -0.99952, -0.99953, -0.99954,
             -0.99955, -0.99955, -0.99956, -0.99957, -0.99957, -0.99958, -0.99959, -0.99959,
             -0.99960, -0.99961, -0.99961, -0.99962, -0.99962, -0.99963, -0.99964, -0.99964,
             -0.99965, -0.99965, -0.99966, -0.99966, -0.99967, -0.99967, -0.99968, -0.99968,
             -0.99969, -0.99969, -0.99970, -0.99970, -0.99970, -0.99971, -0.99971, -0.99972,
             -0.99972, -0.99973, -0.99973, -0.99973, -0.99974, -0.99974, -0.99974, -0.99975,
             -0.99975, -0.99975, -0.99976, -0.99976, -0.99976, -0.99977, -0.99977, -0.99977,
             -0.99978, -0.99978, -0.99978, -0.99979, -0.99979, -0.99979, -0.99979, -0.99980,
             -0.99980, -0.99980, -0.99980, -0.99981, -0.99981, -0.99981, -0.99981, -0.99982,
             -0.99982, -0.99982, -0.99982, -0.99983, -0.99983, -0.99983, -0.99983, -0.99984,
             -0.99984, -0.99984, -0.99984, -0.99984, -0.99985, -0.99985, -0.99985, -0.99985,
             -0.99985, -0.99985, -0.99986, -0.99986, -0.99986, -0.99986, -0.99986, -0.99987,
             -0.99987, -0.99987, -0.99987, -0.99987, -0.99987, -0.99987, -0.99988, -0.99988,
             -0.99988, -0.99988, -0.99988, -0.99988, -0.99989, -0.99989, -0.99989, -0.99989,
             -0.99989, -0.99989, -0.99989, -0.99989, -0.99990, -0.99990, -0.99990, -0.99990,
             -0.99990, -0.99990, -0.99990, -0.99990, -0.99991, -0.99991, -0.99991, -0.99991,
             -0.99991, -0.99991, -0.99991, -0.99991, -0.99991, -0.99992, -0.99992, -0.99992,
             -0.99992, -0.99992, -0.99992, -0.99992, -0.99992, -0.99992, -0.99992, -0.99993,
             -0.99993, -0.99993, -0.99993, -0.99993, -0.99993, -0.99993, -0.99993, -0.99993,
             -0.99993, -0.99993, -0.99993, -0.99994, -0.99994, -0.99994, -0.99994, -0.99994,
             -0.99994, -0.99994, -0.99994, -0.99994, -0.99994, -0.99994, -0.99994, -0.99994,
             -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99995,
             -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99995,
             -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99996,
             -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99996,
             -0.99996, -0.99996, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997,
             -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997,
             -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997,
             -0.99997, -0.99997, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998,
             -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998,
             -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998,
             -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998,
             -0.99998, -0.99998, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000, -1.00000,
             -1.00000, -1.00000, -1.00000, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99999,
             -0.99999, -0.99999, -0.99999, -0.99999, -0.99999, -0.99998, -0.99998, -0.99998,
             -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998,
             -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998,
             -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99998,
             -0.99998, -0.99998, -0.99998, -0.99998, -0.99998, -0.99997, -0.99997, -0.99997,
             -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997,
             -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99997,
             -0.99997, -0.99997, -0.99997, -0.99997, -0.99997, -0.99996, -0.99996, -0.99996,
             -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99996,
             -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99996, -0.99995,
             -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99995,
             -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99995, -0.99994,
             -0.99994, -0.99994, -0.99994, -0.99994, -0.99994, -0.99994, -0.99994, -0.99994,
             -0.99994, -0.99994, -0.99994, -0.99994, -0.99993, -0.99993, -0.99993, -0.99993,
             -0.99993, -0.99993, -0.99993, -0.99993, -0.99993, -0.99993, -0.99993, -0.99993,
             -0.99992, -0.99992, -0.99992, -0.99992, -0.99992, -0.99992, -0.99992, -0.99992,
             -0.99992, -0.99992, -0.99991, -0.99991, -0.99991, -0.99991, -0.99991, -0.99991,
             -0.99991, -0.99991, -0.99991, -0.99990, -0.99990, -0.99990, -0.99990, -0.99990,
             -0.99990, -0.99990, -0.99990, -0.99989, -0.99989, -0.99989, -0.99989, -0.99989,
             -0.99989, -0.99989, -0.99989, -0.99988, -0.99988, -0.99988, -0.99988, -0.99988,
             -0.99988, -0.99987, -0.99987, -0.99987, -0.99987, -0.99987, -0.99987, -0.99987,
             -0.99986, -0.99986, -0.99986, -0.99986, -0.99986, -0.99985, -0.99985, -0.99985,
             -0.99985, -0.99985, -0.99985, -0.99984, -0.99984, -0.99984, -0.99984, -0.99984,
             -0.99983, -0.99983, -0.99983, -0.99983, -0.99982, -0.99982, -0.99982, -0.99982,
             -0.99981, -0.99981, -0.99981, -0.99981, -0.99980, -0.99980, -0.99980, -0.99980,
             -0.99979, -0.99979, -0.99979, -0.99979, -0.99978, -0.99978, -0.99978, -0.99977,
             -0.99977, -0.99977, -0.99976, -0.99976, -0.99976, -0.99975, -0.99975, -0.99975,
             -0.99974, -0.99974, -0.99974, -0.99973, -0.99973, -0.99973, -0.99972, -0.99972,
             -0.99971, -0.99971, -0.99970, -0.99970, -0.99970, -0.99969, -0.99969, -0.99968,
             -0.99968, -0.99967, -0.99967, -0.99966, -0.99966, -0.99965, -0.99965, -0.99964,
             -0.99964, -0.99963, -0.99962, -0.99962, -0.99961, -0.99961, -0.99960, -0.99959,
             -0.99959, -0.99958, -0.99957, -0.99957, -0.99956, -0.99955, -0.99955, -0.99954,
             -0.99953, -0.99952, -0.99951, -0.99951, -0.99950, -0.99949, -0.99948, -0.99947,
             -0.99946, -0.99945, -0.99944, -0.99943, -0.99942, -0.99941, -0.99940, -0.99939,
             -0.99938, -0.99937, -0.99936, -0.99934, -0.99933, -0.99932, -0.99931, -0.99929,
             -0.99928, -0.99926, -0.99925, -0.99923, -0.99922, -0.99920, -0.99919, -0.99917,
             -0.99915, -0.99913, -0.99912, -0.99910, -0.99908, -0.99906, -0.99904, -0.99901,
             -0.99899, -0.99897, -0.99895, -0.99892, -0.99890, -0.99887, -0.99884, -0.99882,
             -0.99879, -0.99876, -0.99873, -0.99870, -0.99866, -0.99863, -0.99859, -0.99856,
             -0.99852, -0.99848, -0.99844, -0.99839, -0.99835, -0.99830, -0.99825, -0.99820,
             -0.99815, -0.99810, -0.99804, -0.99798, -0.99792, -0.99785, -0.99778, -0.99771,
             -0.99764, -0.99756, -0.99748, -0.99739, -0.99730, -0.99721, -0.99710, -0.99700,
             -0.99689, -0.99677, -0.99664, -0.99651, -0.99637, -0.99622, -0.99607, -0.99590,
             -0.99572, -0.99553, -0.99533, -0.99511, -0.99488, -0.99464, -0.99437, -0.99409,
             -0.99378, -0.99345, -0.99309, -0.99270, -0.99228, -0.99182, -0.99132, -0.99077,
             -0.99017, -0.98951, -0.98878, -0.98798, -0.98709, -0.98609, -0.98497, -0.98372,
             -0.98231, -0.98071, -0.97888, -0.97678, -0.97436, -0.97155, -0.96827, -0.96438,
             -0.95976, -0.95420, -0.94745, -0.93913, -0.92876, -0.91563, -0.89875, -0.87663,
             -0.84711, -0.80695, -0.75134, -0.67336, -0.56405, -0.41443, -0.22200,  0.00000,
          ]

if __name__ == "__main__":
    main()            