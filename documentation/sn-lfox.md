# **sn-lfox**

_sn-lfox_ is an expander module for the _sn-lfo_ module that adds a harmonic wave to the output of _sn-lfo_.
Multiple expanders can be stacked to create a waveform using harmonics and/or variations.

## Controls

| Control    | Range | Name           | Description                                                    |
|------------|-------|----------------|----------------------------------------------------------------|
| _ε_        | ±1    | _eccentricity_ | Sets the _eccentricity_ of the _sn_ generator function         |
| _s_        | 0-5   | _sensitivity_  | Fine adjust for the _eccentricity_ set by the _ε_ control      |
| _θ_        | ±90°  | _rotation_     | Sets the rotation angle of he _sn_ generator function          |
| _a_        | 0-1   | _amplitude_    | Sets the amplitude of the harmonic component                   |
| _δx_       | ±1    | _shift-x_      | Sets the horizontal shift of the _sn_ generator function       |
| _δy_       | ±1    | _shift-y_      | Sets the vertical shift of the _sn_ generator function         |
|            |       |                |                                                                |
| _ATT_      | 0-1   | _gain_         | Attenuates the summed output to bring it back into range       |


## Outputs

| Output      | Range | Name          | Description                                                    |
|------------|-------|----------------|----------------------------------------------------------------|
| _LFO_      | ±5V   | _LFO_          | Polyphonal LFO signal output                                   |
| _LFO-Σ_    | ±5V   | _LFO-Σ_        | Summed LFOx + LFO audio output                                 |
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


## Factory Presets

The module includes a set of factory presets:

| Preset         |  _ε_   | _s_ | _θ_ | _a_ | _δx_  | _δy_ | _Φ_ |
|----------------|--------|-----|-----|-----|-------|------|-----|
| sine           |  0.0   | 2.5 | 0°  | 1.0 |  0.0  | 0.0  | 0°  |
| square         | -1.0   | 5.0 | 0°  | 1.0 |  0.0  | 0.0  | 0°  |
| trianglular    |  0.175 | 5.0 | 0°  | 1.0 |  0.0  | 0.0  | 0°  |
| sawtooth-left  | -0.175 | 5.0 | 90° | 1.0 | -0.99 | 0.0  | 0°  |
| sawtooth-right | -0.175 | 5.0 | 90° | 1.0 | +0.99 | 0.0  | 0°  |


## Patches

### [*sn-vcv-lfox*](/documentation/patches/sn-vcv-lfox.vcv)

Basic patch to demonstrate the operation of stacked _sn-lfox_ modules. 

Replaces the independent LFOs in the basic _sn-lfo_ patch with an _sn-lfo_ module and two stacked 
_sn-lfox_ expanders for a slightly different effect.Oddly ambient'ish in a way .. maybe with some
reverb ..

_Requires_:
1. _Fundamental_ modules
