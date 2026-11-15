# NOTES

## Baselines

|            |                                 | error | Mean Wall Time [ms] | Change   | Factor         |
|:-----------|---------------------------------|-------|--------------------:|---------:|:---------------|
| baseline   | triangular ramp                 | -     | 167.5 ± 1.0         |          |                |
| sine       | std::sin                        | -     | 2498.4 ± 10.8       | +1391.8% | (14.9x slower) |
| sinef      | std::sinf                       | 1e-7  | 2732.6 ± 15.7       | +1531.6% | (16.3x slower) |
| sine4      | rack::simd::sin                 | 1e-6  | 2748.6 ± 7.2        | +1541.2% | (16.4x slower) |
| polynomial | polynomial sin approximation    | 1e-3  | 489.2 ± 2.7         | +192.1%  | (2.9x slower)  |
| lookup     | 1024 entry lookup table         | 1e-5  | 474.7 ± 2.9         | +183.4%  | (2.8x slower)  |
| sn         | sn                              | 1e-6  | 3728.3 ± 4.5        | +2126.2% | (22.3x slower) |
| sn4        | sn, 4x oversampling,SIMD,float  | 1e-6  | 2194.5 ± 4.1        | +1210.3% | (13.1x slower) |
| sn4d       | sn, 4x oversampling,SIMD,double | 1e-6  | 1788.1 ± 6.5        | +967.7%  | (10.7x slower) |

__Notes:__
1. _sin4_ is somewhat bogus (it uses rack::simd::sin to calculate only a single value)
2. _sn_ has a call to `std::sin` and `std::cos`
3. _sn4_ has call to `rack::simd:sin` and `rack::simd_cos`
4. _sn4d_ has call to `rack::simd:sin` and `rack::simd_cos`

## sn-lookup

Benchmark 1: ./bin/sn (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    3.627 s   ±   0.055 s      3.563 s   …   3.762 s
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB
 
Benchmark 2: ./bin/sn_lookup (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    1.145 s   ±   0.009 s      1.130 s   …   1.156 s            -68.4%  (3.2x faster)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
