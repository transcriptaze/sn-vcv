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
    - k-rate:1 (reducing k-rate to 1/256 gives about a 5% performance boost)


|           | standalone | VCV, no antialias    | VCV, 4x oversampling |
|-----------|------------|----------------------|----------------------|
| baseline  | 0m23.384s  | 27%, 0.8% per module | N/A                  |
| reference | 0m59.011s  | 47%, 1.6% per module | 61%, 2.7% per module |
| riffles   | 0m59.111s  |                      |                      |
| unpacked  | 0m33.433s  |                      |                      |
| baseline2 | 0m24.468s  |                      | N/A                  |


## `fmod`
1. `αʼ = std::fmod(αʼ + δα, 2.0 * M_PI);`
real    0m59.922s
user    0m59.754s
sys 0m0.056s

2. `αʼ = std::fmod(αʼ + δα, TAU);`
real    0m53.878s
user    0m53.605s
sys 0m0.099s

3. `αʼ += δα; if (αʼ > TAU) { αʼ -= TAU; }`
real    1m14.305s
user    1m14.002s
sys 0m0.071s

4. `αʼ += δα; ... α[ch] = std::fmod(αʼ, TAU);`
real    0m37.398s
user    0m37.159s
sys 0m0.062s

