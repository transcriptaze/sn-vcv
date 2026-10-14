import numpy as np
import scipy.signal as signal
import matplotlib.pyplot as plt

FS = 48000
F0 = 18_000
OVERSAMPLING = 4
TAPS = 100 # 25x4

f0 = F0 / (OVERSAMPLING * FS / 2.0)  # normalized cutoff frequency
taps = signal.firwin(TAPS, f0, window='hamming')
w, h = signal.freqz(taps, fs=FS * OVERSAMPLING)

polyphase = taps.reshape(-1, 4).T

print ()
print ("constexpr struct {")
print (f"    const double taps[{len(polyphase)}][{len(polyphase[0])}];")
print (f"}} POLYPHASE_{int(F0/1000)}kHz_{OVERSAMPLING}x{{")
print ("    // clang-format off")
print ("    .taps = {")

for phase in polyphase:
    print("        {")
    for ix in range(0, len(phase), 4):
        values = ", ".join(f"{x: .8e}" for x in phase[ix:ix+4])
        print(f"            {values},")
    print("        },")

print ("    },")
print ("    // clang-format on")
print ("};")
print ()


plt.figure(figsize=(12, 6))
plt.semilogx(w, 20 * np.log10(abs(h)))
plt.title("polyphase filter")
plt.xlabel("Frequency [Hz]")
plt.ylabel("Amplitude [dB]")
plt.margins(0, 0.1)
plt.grid(which="both", axis="both")
plt.axvline(F0, color="green")  # cutoff frequency
plt.axhline(-3, color="red")  # -3dB
plt.show()
