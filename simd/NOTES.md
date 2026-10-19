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
  |           | Mean Wall Time [s] | Change  | Factor        |
  |:----------|-------------------:|--------:|:--------------|
  | reference | 11.949 ± 0.060     |         |               |
  | riffles   | 12.044 ± 0.018     | +0.8%   |               |
  | unpacked  | 6.697 ± 0.009      | -44.0%  | (1.8x faster) |
  | sincos    | 5.996 ± 0.019      | -49.8%  | (2.0x faster) |
  | sincos2   | 5.114 ± 0.004      | -57.2%  | (2.3x faster) |
  | sincos2A  | 5.167 ± 0.005      | -56.8%  | (2.3x faster) |
**| float4A   | 2.236 ± 0.002      | -81.3%  | (5.3x faster) |**
  | float4B   | 3.402 ± 0.004      | -71.5%  | (3.5x faster) |
  | float4C   | 2.409 ± 0.003      | -79.8%  | (5.0x faster) |
  | float4D   | 2.580 ± 0.006      | -78.4%  | (4.6x faster) |

  |           | Mean Wall Time [s] | Change  | Factor        |
  |:----------|-------------------:|--------:|:--------------|
  | float4A   | 2.187 ± 0.008      |         |               |
  | float4B   | 3.381 ± 0.006      | +54.6%  | (1.5x slower) |
  | float4C   | 2.387 ± 0.004      | +9.1%   |               |
  | float4D   | 2.561 ± 0.003      | +17.1%  |               |

  |           | Mean Wall Time [s] | Change  | Factor        |
  |:----------|-------------------:|--------:|:--------------|
  | float4A   | 2.158 ± 0.008      |         |               |
  | float4AX  | 1.793 ± 0.003      | -16.9%  |               |
**| float4AY  | 1.740 ± 0.002      | -19.4%  |               |**
  | float4B   | 3.362 ± 0.009      | +55.8%  | (1.6x slower) |
  | float4C   | 2.370 ± 0.002      | +9.8%   |               |
  | float4D   | 2.540 ± 0.002      | +17.7%  |               |

Benchmark 1: ./bin/float4A (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    2.158 s   ±   0.008 s      2.145 s   …   2.168 s
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB
 
Benchmark 2: ./bin/float4AX (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    1.793 s   ±   0.003 s      1.790 s   …   1.800 s            -16.9%  
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB           -1.0%  
 
Benchmark 3: ./bin/float4AY (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    1.740 s   ±   0.002 s      1.738 s   …   1.742 s            -19.4%  
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB           -1.0%  
 
Benchmark 4: ./bin/float4B (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    3.362 s   ±   0.009 s      3.348 s   …   3.374 s            +55.8%  (1.6x slower)
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB           -0.1%  
 
Benchmark 5: ./bin/float4C (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    2.370 s   ±   0.002 s      2.368 s   …   2.374 s             +9.8%  
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB           -1.0%  
 
Benchmark 6: ./bin/float4D (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    2.540 s   ±   0.002 s      2.537 s   …   2.542 s            +17.7%  
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB           -0.1%  


## validation suite

float
=====
    max(mean)    0.000011
    max(stddev)  0.00022
    max(max)     0.002228


sincos
======
    max(mean)    0.000002
    max(stddev)  0.00014
    max(max)     0.002930


sincos2
=======
    max(mean)    0.000002
    max(stddev)  0.00014
    max(max)     0.002930


float4A
=======
    max(mean)    0.000002
    max(stddev)  0.00015
    max(max)     0.002930

===============================================================================
[doctest] test cases:     6 |     6 passed | 0 failed | 0 skipped
[doctest] assertions: 15302 | 15302 passed | 0 failed |
[doctest] Status: SUCCESS!


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

