# **sn-vco**

_sn-vco_ is a more or less standard VCO module except that it produces a _sn_ output rather than a _sine_,
allowing the waveshape to be morphed between a sine, square, triangular and sawtooth wave and pretty much
everything in between. Although it can be used on its own, the module is really intended to be used as
the base for stacked _sn-vcox_ expanders. 

<img width="256" src="images/sine.png"><img width="256" src="images/square.png"><img width="256" src="images/triangle.png">
<img width="256" src="images/sawtooth.png"><img width="256" src="images/spike.png"><img width="256" src="images/bumps.png">
<img width="256" src="images/wonky.png">

## Parameters


## Inputs and outputs

| Input      | Range | Name           | Description                                                    |
|------------|-------|----------------|----------------------------------------------------------------|
| _pitch_    | ±5V   | _frequency_    | Polyphonal 1V/octave pitch input, normalized for C4 at 0V      |
| _velocity_ | 0-10V | _velocity_     | Polyphonal MIDI note velocity input. Defaults to 10V (127)     |
|            |       |                |                                                                |
| _ε_        | ±5V   | _eccentricity_ | Overrides the _eccentricity_ parameter control                 |
| _s_        | 0-10V | _sensitivity_  | Overrides the _sensitivity_ parameter control                  |
| _θ_        | ±5V   | _rotation_     | Overrides the _rotation_ parameter control                     |
| _a_        | 0-10V | _amplitude_    | Overrides the _amplitude_ parameter control                    |
| _δx_       | ±5V   | _shift-x_      | Overrides the _shift-x_ parameter control                      |
| _δy_       | ±5V   | _shift-y_      | Overrides the _shift-y_ parameter control                      |

| Output      | Range | Name          | Description                                                    |
|------------|-------|----------------|----------------------------------------------------------------|
| _VCO_      | ±5V   | _VCO_          | Polyphonal VCO audio output                                    |
| _AUX_      | ±5V   | _AUX_          | 2 channel 25Hz signal 'monitor' output for use with the scope  |
| _TRIG_     | 0-10V | _trigger_      | Trigger output for the AUX output                              |

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

### [*sn-vcv-vco*](/documentation/patches/sn-vcv-vco.vcv)

Basic patch to demonstrate the operation of the _sn-vco_ module. 

The patch basically connects three standard LFOs to _ε_, _δx_ and _δy_ inputs of a single _sn-vco_ module
to output a continuously varying waveform. Not very inspired and sounds pretty bland but should (hopefully)
give some idea of the operation of the module.

_Requires_:
1. _Fundamental_ modules
