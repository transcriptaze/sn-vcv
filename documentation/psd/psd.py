import csv
import numpy as np
import matplotlib.pyplot as plt


def main():
    y = sn()

    plt.figure('PSD')
    plt.plot(y)
    plt.title('PSD: 25Hz AUX IN')
    plt.xlabel('t')
    plt.ylabel('V')
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
