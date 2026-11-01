# NOTES

Working notes for optimising _sn-vco_ infrastructure.

## patches

| patch         | module   | load                           | configuration                                         |
|---------------|----------|--------------------------------|-------------------------------------------------------|
| baseline      | sn-debug | 18 modules, 16 channels/module | -                                                     |
| vcv           | VCV VCO  | 18 modules, 16 channels/module | sine, no FM                                           |
| sn-vco        | sn-vco   | 18 modules, 16 channels/module | k-rate 1/256, no anti-aliasing, no DC blocking        |
| sn-vco_4x1    | sn-vco   | 18 modules, 16 channels/module | k-rate 1/256, 4x oversampling, 1x IIR, no DC blocking |
| sn-vco_4x2    | sn-vco   | 18 modules, 16 channels/module | k-rate 1/256, 4x oversampling, 2x IIR, no DC blocking |


### performance

  |            | sn-debug | vcv | squinky | sn-vco | sn-vco_4x1 | sn-vco_4x2 |
  |:-----------|----------|-----|---------|--------|------------|------------|
  | baseline   | 8%       | 25% |         | 34%    | 48.75%     | 49.5%      |
  | υ4         |          |     |         | 30%    | 45%        | 48.75%     |

