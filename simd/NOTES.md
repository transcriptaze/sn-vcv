# NOTES

1. `float υ(const SN &sn, float α)`

- standalone
    - 5 runs
    - 16 channels
    - 48 kHz
    - 4x oversampling
    - 10m
    - 1_843_200_000 samples

- VCV
    - 3x6 banks of VCOs
    - 16 channels
    - 48kHz


|           | standalone | VCV, no antialias    | VCV, 4x oversampling |
|-----------|------------|----------------------|----------------------|
| baseline  | 0m23.384s  | 27%, 0.8% per module | N/A                  |
| reference | 0m59.011s  | 47%, 1.6% per module | 61%, 2.7% per module |


