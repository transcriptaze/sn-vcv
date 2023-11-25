# **sn-vcox**

_sn-vcox_ is an expander module for the _sn-vco_ module that adds a harmonic wave to the output of 
_sn-vco_. Multiple expanders can be stacked to create a waveform using harmonics and/or variations.

## Parameters


## Inputs and outputs

| Input      | Range | Name           | Description                                                    |
|------------|-------|----------------|----------------------------------------------------------------|
| _ε_        | ±5V   | _eccentricity_ | Overrides the _eccentricity_ parameter control                 |
| _s_        | 0-10V | _sensitivity_  | Overrides the _sensitivity_ parameter control                  |
| _θ_        | ±5V   | _rotation_     | Overrides the _rotation_ parameter control                     |
| _a_        | 0-10V | _amplitude_    | Overrides the _amplitude_ parameter control                    |
| _δx_       | ±5V   | _shift-x_      | Overrides the _shift-x_ parameter control                      |
| _δy_       | ±5V   | _shift-y_      | Overrides the _shift-y_ parameter control                      |

| Output      | Range | Name          | Description                                                    |
|------------|-------|----------------|----------------------------------------------------------------|
| _VCO_      | ±5V   | _VCO_          | Polyphonal VCO audio output                                    |
| _VCO-Σ_    | ±V    | _VCO-Σ_        | Summed VCOx + VCO audio output.                                |
| _AUX_      | ±5V   | _AUX_          | 2 channel 25Hz signal 'monitor' output for use with the scope  |


The _VCO-Σ_ is a summed voltage and can easily exceed the ±5V audio signal range if all component waveforms
are at their maximum amplitudes. The _ATT_ control can be used to attenuate the _VCO-Σ_ output to a usable
range.


### Context Menu

1. _k-rate_
   Sets the rate at which the parameter values are used to recalculate the waveform:
   - audio rate
   - ¹⁄₆₄ audio rate
   - ¹⁄₁₂₈ audio rate
   - ¹⁄₂₅₆ audio rate

2. _aux-mode_
   Sets the AUX output mode for display on the _scope_ module:
   - _OSC_ outputs a 25Hz 'monitor' waveform
   - _SUM_ outputs the summed 25Hz 'monitor' waveform
   - _POLY_ outputs the monitor wave form on channel 1 and the summed monitor waveform on channel 2 |

3. _aux-gain_
   Sets the AUX output gain for display on the _scope_ module:
   - 1 (±5V, 10V peak-to-peak)
   - ½ (±2.5V, 5V peak-to-peak)
   - ⅓ (±1.66V, 3.33V peak-to-peak)
   - ¼ (±1.25V, 2.5V peak-to-peak)
   - ⅕ (±1V, 2V peak-to-peak)

## Presets

## Patches

### [*sn-vcv-vcox*](/documentation/patches/sn-vcv-vcox.vcv)

Basic patch to demonstrate the operation of stacked _sn-vcox_ modules. 

Uses three independent LFOs to drive the parameter inputs of an _sn-vco_ module with two stacked _sn-vcox_ 
extenders to that the resulting waveform is the sum of three more or less independent harmonics, each
of which is shape-shifting in it's own particular way. A little more interesting than just the basic
_sn-vco_ patch but its not going to win any awards.

_Requires_:
1. _Fundamental_ modules
