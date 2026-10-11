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


| c++11     | sandboxed  | VCV, no antialias    | VCV, 4x oversampling |
|-----------|------------|----------------------|----------------------|
| baseline  | 0m23.384s  | 27%, 0.8% per module | N/A                  |
| reference | 0m59.011s  | 47%, 1.6% per module | 61%, 2.7% per module |
| baseline2 | 0m8.496s   | -                    | -                    |
| riffles   | 0m59.111s  | -                    | -                    |
| unpacked  | 0m33.433s  | -                    | -                    |
| sincos    | 0m29.225s  | -                    | -                    |
| sincos2   | 0m24.946s  | -                    | -                    |
| sincos2A  | 0m25.207s  | -                    | -                    |
| float4A   | 0m10.848s  | -                    | -                    |
| float4B   | 0m16.602s  | -                    | -                    |
| float4C   | 0m11.717s  | -                    | -                    |
| float4D   | 0m12.545s  | -                    | -                    |

## hyperfine

### c++11

| Command         | Mean Wall Time [s] | Change | Factor        |
|:----------------|-------------------:|-------:|:--------------|
| `./bin/float4A` | 2.232 ± 0.003      |        |               |
| `./bin/float4B` | 3.400 ± 0.003      | +52.3% | (1.5x slower) |
| `./bin/float4C` | 2.406 ± 0.002      | +7.8%  |               |
| `./bin/float4D` | 2.576 ± 0.003      | +15.4% |               |

### c++17
| Command         | Mean Wall Time [s] | Change | Factor        |
|:----------------|-------------------:|-------:|:--------------|
| `./bin/float4A` | 2.183 ± 0.010      |        |               |
| `./bin/float4B` | 3.379 ± 0.010      | +54.8% | (1.5x slower) |
| `./bin/float4C` | 2.389 ± 0.003      | +9.4%  |               |
| `./bin/float4D` | 2.563 ± 0.005      | +17.4% |               |


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

