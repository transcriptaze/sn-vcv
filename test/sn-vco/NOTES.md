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

|                             | load   | notes                                                           |
|:----------------------------|--------|-----------------------------------------------------------------|
| baseline                    | 8%     | triangular, fixed frequency                                     |
| per-channel α               | 9%     | per channel triangular, fixed frequency                         |
| per-channel α (std::array)  | 8.75%  | per channel triangular, fixed frequency                         |
| per-channel sine            | 15%    | per channel sine, fixed frequency                               |
| per-channel sn.υ            | 20.5%  | per channel sn, fixed frequency, precomputed ζ                  |
| per-channel sn.υ4  (float)  | 22.25% | per channel float_4 sn, fixed frequency, precomputed ζ (float)  |
| per-channel sn.υ4  (double) | 22.75% | per channel float_4 sn, fixed frequency, precomputed ζ (float)  |
| per-channel sn.υ4d (double) | 22.75% | per channel float_4 sn, fixed frequency, precomputed ζ (double) |

## R&D

- sine wave interpolation: https://habr.com/en/articles/1096724/