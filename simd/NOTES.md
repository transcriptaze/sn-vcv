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

  | Command   | Mean Wall Time [s] | Change | Factor         |
  |:----------|-------------------:|-------:|:---------------|
  | float4A   | 2.232 ± 0.003      |        |                |
  | float4B   | 3.400 ± 0.003      | +52.3% | (1.5x slower)  |
  | float4C   | 2.406 ± 0.002      | +7.8%  |                |
  | float4D   | 2.576 ± 0.003      | +15.4% |                |

### c++17
  | Command   | Mean Wall Time [s] | Change | Factor         |
  |:----------|-------------------:|-------:|:---------------|
  | reference | 11.928 ± 0.028     |        |                |
  | riffles   | 11.974 ± 0.010     | +0.4%  |                |
  | unpacked  | 6.695 ± 0.006      | -43.9% | (1.8x faster)  |
  | sincos    | 6.013 ± 0.050      | -49.6% | (2.0x faster)  |
  | sincos2   | 5.105 ± 0.005      | -57.2% | (2.3x faster)  |
  | sincos2A  | 5.157 ± 0.008      | -56.8% | (2.3x faster)  |
  | float4A   | 2.223 ± 0.003      | -81.4% | (5.4x faster)  |
  | float4AX  | 1.840 ± 0.002      | -84.6% | (6.5x faster)  |
**| float4AY  | 1.777 ± 0.004      | -85.1% | (6.7x faster)  |**
  | float4B   | 3.390 ± 0.004      | -71.6% | (3.5x faster)  |
  | float4C   | 2.390 ± 0.003      | -80.0% | (5.0x faster)  |
  | float4D   | 2.553 ± 0.002      | -78.6% | (4.7x faster)  |
  | float4X   | 1.842 ± 0.002      | -84.6% | (6.5x faster)  |
**| float4XX  | 1.920 ± 0.001      | -83.9% | (6.2x faster)  |**
  | float4XY  | 1.857 ± 0.001      | -84.4% | (6.4x faster)  |

  | Command   | Mean Wall Time [s] | Change | Factor         |
  |:----------|-------------------:|-------:|:---------------|
  | float4A   | 2.154 ± 0.007      |        |                |
  | float4AX  | 1.789 ± 0.003      | -17.0% |                |
**| float4AY  | 1.734 ± 0.002      | -19.5% |                |**
  | float4B   | 3.374 ± 0.076      | +56.7% | (1.6x slower)  |
  | float4C   | 2.364 ± 0.002      | +9.8%  |                |
  | float4D   | 2.529 ± 0.003      | +17.4% |                |
  | float4X   | 1.824 ± 0.004      | -15.3% |                |
**| float4XX  | 1.906 ± 0.004      | -11.5% |                |**
  | float4XY  | 1.904 ± 0.004      | -11.6% |                |


Benchmark 1: ./bin/float4A (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    2.154 s   ±   0.007 s      2.141 s   …   2.162 s
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB
 
Benchmark 2: ./bin/float4AX (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    1.789 s   ±   0.003 s      1.785 s   …   1.793 s            -17.0%  
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB            0.0%
 
Benchmark 3: ./bin/float4AY (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    1.734 s   ±   0.002 s      1.731 s   …   1.738 s            -19.5%  
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB            0.0%
 
Benchmark 4: ./bin/float4B (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    3.374 s   ±   0.076 s      3.338 s   …   3.590 s            +56.7%  (1.6x slower)
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB           -1.0%  
 
Benchmark 5: ./bin/float4C (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    2.364 s   ±   0.002 s      2.359 s   …   2.367 s             +9.8%  
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB           -0.2%  
 
Benchmark 6: ./bin/float4D (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    2.529 s   ±   0.003 s      2.526 s   …   2.533 s            +17.4%  
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB           -1.0%  
 
Benchmark 7: ./bin/float4X (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    1.824 s   ±   0.004 s      1.819 s   …   1.829 s            -15.3%  
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB           -1.0%  
 
Benchmark 8: ./bin/float4XX (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    1.906 s   ±   0.004 s      1.901 s   …   1.911 s            -11.5%  
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB           -0.2%  
 
Benchmark 9: ./bin/float4XY (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    1.904 s   ±   0.004 s      1.898 s   …   1.911 s            -11.6%  
  Memory         1.6 MiB ±     0.0 MiB      1.6 MiB …     1.6 MiB           -1.0%  


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


float4X
=======
    max(mean)    0.000000
    max(stddev)  0.00001
    max(max)     0.000165

===============================================================================
[doctest] test cases:     7 |     7 passed | 0 failed | 0 skipped
[doctest] assertions: 18947 | 18947 passed | 0 failed |
[doctest] Status: SUCCESS!


## sn-vco

### k-rate 1/256, no DC blocking

  |            | vcv | squinky | sn-vco | sn-vco_4x1 | sn-vco_4x2 |
  |:-----------|-----|---------|--------|------------|------------|
  | baseline   | 25% |         | 34%    | 48.75%     | 49.5%      |
  | υ4         |     |         | 30%    | 45%        | 48.75%     |


**Notes:**

1. `vcv`:        18x VCV fundamental VCOs
2. `sn-vco`:     18x sn-vco, no antialiasing
3. `sn-vco_4x1`: 18x sn-vco, 4x oversampling
4. `sn-vco_4x2`: 18x sn-vco, 4x oversampling



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


