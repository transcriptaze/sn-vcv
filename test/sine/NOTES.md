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

Benchmark 2A: ./bin/sn_lookup+sqrt (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    0.865 s   ±   0.002 s      0.863 s   …   0.868 s            -76.7%  (4.3x faster)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  


|                     | Mean Wall Time [ms] | Change   | Factor         |
|:--------------------|--------------------:|---------:|:---------------|
| baseline            | 164.4 ± 2.0         |          |                |
| sine                | 2451.3 ± 15.1       | +1391.5% | (14.9x slower) |
| sinef               | 2691.0 ± 20.3       | +1537.3% | (16.4x slower) |
| sine4               | 2707.7 ± 16.8       | +1547.5% | (16.5x slower) |
| polynomial          | 484.2 ± 2.8         | +194.6%  | (2.9x slower)  |
| lookup              | 473.5 ± 3.6         | +188.1%  | (2.9x slower)  |
| sn                  | 3678.3 ± 10.7       | +2138.1% | (22.4x slower) |
| sn4                 | 2164.3 ± 6.0        | +1216.9% | (13.2x slower) |
| sn4d                | 1761.2 ± 6.6        | +971.6%  | (10.7x slower) |
| sn_lookup           | 1148.8 ± 8.2        | +599.0%  | (7.0x slower)  |
| sn_lookup_sqrt      | 861.8 ± 1.2         | +424.3%  | (5.2x slower)  |
| sn_lookup_rsqrt     | 871.7 ± 4.4         | +430.4%  | (5.3x slower)  |
| sn_lookup_sincos    | 941.3 ± 4.5         | +472.7%  | (5.7x slower)  |
| sn_lookup_sincos2   | 731.0 ± 2.2         | +344.8%  | (4.4x slower)  |
| sn_lookup_sincos2x  | 763.2 ± 1.4         | +364.3%  | (4.6x slower)  |
| sn_lookup_sincos2f  | 617.8 ± 5.4         | +275.9%  | (3.8x slower)  |
| sn_lookup_sincos2fx | 616.4 ± 2.0         | +275.0%  | (3.8x slower)  |
| sn_lookup_sincos2d  | 645.3 ± 2.8         | +292.7%  | (3.9x slower)  |

Benchmark 1: ./bin/baseline (18 runs)
                mean     ±       σ          min     …     max
  Wall Time    164.4 ms  ±     2.0 ms     162.4 ms  …   171.8 ms
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB
 
Benchmark 2: ./bin/sine (10 runs)
                mean     ±       σ          min     …     max
  Wall Time   2451.3 ms  ±    15.1 ms    2429.0 ms  …  2471.0 ms         +1391.5%  (14.9x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 3: ./bin/sinef (10 runs)
                mean     ±       σ          min     …     max
  Wall Time   2691.0 ms  ±    20.3 ms    2671.5 ms  …  2730.4 ms         +1537.3%  (16.4x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB            0.0%
 
Benchmark 4: ./bin/sine4 (10 runs)
                mean     ±       σ          min     …     max
  Wall Time   2707.7 ms  ±    16.8 ms    2691.0 ms  …  2743.0 ms         +1547.5%  (16.5x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB            0.0%
 
Benchmark 5: ./bin/polynomial (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    484.2 ms  ±     2.8 ms     481.0 ms  …   490.3 ms          +194.6%  (2.9x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 6: ./bin/lookup (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    473.5 ms  ±     3.6 ms     471.2 ms  …   483.5 ms          +188.1%  (2.9x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 7: ./bin/sn (10 runs)
                mean     ±       σ          min     …     max
  Wall Time   3678.3 ms  ±    10.7 ms    3663.7 ms  …  3693.4 ms         +2138.1%  (22.4x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 8: ./bin/sn4 (10 runs)
                mean     ±       σ          min     …     max
  Wall Time   2164.3 ms  ±     6.0 ms    2158.0 ms  …  2178.2 ms         +1216.9%  (13.2x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.8%  
 
Benchmark 9: ./bin/sn4d (10 runs)
                mean     ±       σ          min     …     max
  Wall Time   1761.2 ms  ±     6.6 ms    1755.3 ms  …  1778.9 ms          +971.6%  (10.7x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 10: ./bin/sn_lookup (10 runs)
                mean     ±       σ          min     …     max
  Wall Time   1148.8 ms  ±     8.2 ms    1137.7 ms  …  1159.4 ms          +599.0%  (7.0x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 11: ./bin/sn_lookup_sqrt (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    861.8 ms  ±     1.2 ms     860.2 ms  …   864.0 ms          +424.3%  (5.2x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 12: ./bin/sn_lookup_rsqrt (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    871.7 ms  ±     4.4 ms     868.4 ms  …   883.5 ms          +430.4%  (5.3x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 13: ./bin/sn_lookup_sincos (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    941.3 ms  ±     4.5 ms     936.1 ms  …   951.5 ms          +472.7%  (5.7x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           -0.1%  
 
Benchmark 14: ./bin/sn_lookup_sincos2 (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    731.0 ms  ±     2.2 ms     727.9 ms  …   734.1 ms          +344.8%  (4.4x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 15: ./bin/sn_lookup_sincos2x (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    763.2 ms  ±     1.4 ms     760.5 ms  …   765.6 ms          +364.3%  (4.6x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 16: ./bin/sn_lookup_sincos2f (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    617.8 ms  ±     5.4 ms     613.9 ms  …   632.2 ms          +275.9%  (3.8x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 17: ./bin/sn_lookup_sincos2fx (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    616.4 ms  ±     2.0 ms     613.5 ms  …   620.4 ms          +275.0%  (3.8x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 18: ./bin/sn_lookup_sincos2d (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    645.3 ms  ±     2.8 ms     642.8 ms  …   652.4 ms          +292.7%  (3.9x slower)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +1.9%  
 


### lookup benchmarks

|                     | Mean Wall Time [ms] | Change   | Factor         |
|:--------------------|--------------------:|---------:|:---------------|
| sn_lookup           | 1.130 ± 0.007       |          |                |
| sn_lookup_sqrt      | 0.853 ± 0.005       | -24.5%   |                |
| sn_lookup_rsqrt     | 0.863 ± 0.005       | -23.6%   |                |
| sn_lookup_sincos    | 0.921 ± 0.001       | -18.5%   |                |
| sn_lookup_sincos2   | 0.720 ± 0.002       | -36.3%   | (1.6x faster)  |
| sn_lookup_sincos2x  | 0.755 ± 0.004       | -33.2%   | (1.5x faster)  |
| sn_lookup_sincos2f  | 0.611 ± 0.001       | -46.0%   | (1.9x faster)  |
| sn_lookup_sincos2fx | 0.613 ± 0.004       | -45.7%   | (1.8x faster)  |
| sn_lookup_sincos2d  | 0.640 ± 0.002       | -43.4%   | (1.8x faster)  |

Benchmark 1: ./bin/sn_lookup (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    1.130 s   ±   0.007 s      1.116 s   …   1.138 s
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB
 
Benchmark 2: ./bin/sn_lookup_sqrt (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    0.853 s   ±   0.005 s      0.848 s   …   0.865 s            -24.5%  
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +1.0%  
 
Benchmark 3: ./bin/sn_lookup_rsqrt (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    0.863 s   ±   0.005 s      0.857 s   …   0.874 s            -23.6%  
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.1%  
 
Benchmark 4: ./bin/sn_lookup_sincos (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    0.921 s   ±   0.001 s      0.919 s   …   0.923 s            -18.5%  
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +0.9%  
 
Benchmark 5: ./bin/sn_lookup_sincos2 (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    0.720 s   ±   0.002 s      0.718 s   …   0.722 s            -36.3%  (1.6x faster)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +1.9%  
 
Benchmark 6: ./bin/sn_lookup_sincos2x (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    0.755 s   ±   0.004 s      0.752 s   …   0.765 s            -33.2%  (1.5x faster)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +1.9%  
 
Benchmark 7: ./bin/sn_lookup_sincos2f (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    0.611 s   ±   0.001 s      0.608 s   …   0.612 s            -46.0%  (1.9x faster)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +1.9%  
 
Benchmark 8: ./bin/sn_lookup_sincos2fx (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    0.613 s   ±   0.004 s      0.610 s   …   0.625 s            -45.7%  (1.8x faster)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +1.0%  
 
Benchmark 9: ./bin/sn_lookup_sincos2d (10 runs)
                mean     ±       σ          min     …     max
  Wall Time    0.640 s   ±   0.002 s      0.636 s   …   0.643 s            -43.4%  (1.8x faster)
  Memory         1.7 MiB ±     0.0 MiB      1.7 MiB …     1.7 MiB           +2.9%  
 
