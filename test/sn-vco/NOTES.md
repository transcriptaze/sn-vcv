# NOTES

Working notes for optimising _sn-vco_ infrastructure.

### benchmarks

| patch         | module   | load                    | configuration                                         |
|---------------|----------|-------------------------|-------------------------------------------------------|
| baseline      | sn-debug | 18 modules, 16 channels | -                                                     |
| vcv           | VCV VCO  | 18 modules, 16 channels | sine, no FM                                           |
| sn-vco        | sn-vco   | 18 modules, 16 channels | k-rate 1/256, no anti-aliasing, no DC blocking        |
| sn-vco_4x1    | sn-vco   | 18 modules, 16 channels | k-rate 1/256, 4x oversampling, 1x IIR, no DC blocking |
| sn-vco_4x2    | sn-vco   | 18 modules, 16 channels | k-rate 1/256, 4x oversampling, 2x IIR, no DC blocking |


|             | sn-debug | vcv | squinky | sn-vco | sn-vco_4x1 | sn-vco_4x2 |
|:------------|----------|-----|---------|--------|------------|------------|
| baseline    | 8%       | 25% |         | 34%    | 48.75%     | 49.5%      |
| υ4          |          |     |         | 30%    | 45%        | 48.75%     |


### sn-debug

|                             | load   | notes                                                             |
|:----------------------------|--------|-------------------------------------------------------------------|
| baseline                    | 8.5%   | triangular, fixed frequency                                       |
| per-channel α               | 9.0%   | per channel triangular, fixed frequency                           |
| per-channel α (std::array)  | 8.75%  | per channel triangular, fixed frequency                           |
| per-channel sine            | 15.0%  | per channel sine, fixed frequency                                 |
| per-channel sine4           | 15.5%  | per channel simd::sine, fixed frequency                           |
| per-channel sine (fast)     | 10.25% | per channel simd::sine, fixed frequency, polynomial approximation |
| per-channel sine (lookup)   | 12.25% | per channel simd::sine, fixed frequency, 1024 entry lookup table  |
| per-channel sn.υ            | 22.5%  | per channel sn, fixed frequency, precomputed ζ                    |
| per-channel sn.υ4  (float)  | 24.25% | per channel float_4 sn, fixed frequency, precomputed ζ (float)    |
| per-channel sn.υ4  (double) | 25.0%  | per channel float_4 sn, fixed frequency, precomputed ζ (float)    |
| per-channel sn.υ4d (double) | 24.0%  | per channel float_4 sn, fixed frequency, precomputed ζ (double)   |

## R&D

- [sine wave interpolation](https://habr.com/en/articles/1096724)
- [fast polynomials](https://publik-void.github.io/sin-cos-approximations)
- [Bhāskara I's sine approximation formula](https://en.wikipedia.org/wiki/Bh%C4%81skara_I%27s_sine_approximation_formula)
- [Fastest implementation of sine, cosine and square root in C++ (doesn't need to be much accurate)](https://stackoverflow.com/questions/18662261/fastest-implementation-of-sine-cosine-and-square-root-in-c-doesnt-need-to-b)