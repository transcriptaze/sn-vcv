![build](https://github.com/transcriptaze/sn-vcv/workflows/build/badge.svg)

# sn-vcv

A set of VCV modules for the _sn_ experimental additive synthesizer which uses the _Jacobi ellipse_ as the 
generator function.

- [Background](#background)
- [Modules](#modules)
   - [sn vco](#sn-vco)
   - [sn vcox](#sn-vcox)
   - [sn lfo](#sn-lfo)
   - [sn lfox](#sn-lfox)
- [Patches](#patches)
- [License](#license)

## Background

Mathematically a _sine_ function is a special case of the Jacobi elliptic _sn_ function (in the same way that
a circle is a special case of an ellipse). The _sn-vcv_ VCV modules use the _sn_ function rather than the
_sine_ function to generate harmonically rich waveforms that are also continuous and differentiable.

By modifying the parameters of the _sn_ function it is possible to smoothly morph between a sine wave,
a square (well, square'ish) wave, a triangular wave and a sawtooth. There is a [web app](https://snyth.pages.dev)
which may make this a bit clearer than this umm, very extremely brief explanation, and there is a little 
more detail [here](https://github.com/transcriptaze/snyth) if you're really curious.

### Parameters

The modules use the following parameters to determine the wave shape:

| Parameter | Range | Name           | Description                                                     |
|-----------|-------|----------------|-----------------------------------------------------------------|
| _ε_       | ±1    | _eccentricity_ | Sets the eccentricity of the generating ellipse                 |
| _s_       | 0-5   | _sensitivity_  | 'Fine' adjustment for the _eccentricity_                        |
| _θ_       | ±90°  | _rotation_     | Rotates the ellipse around the origin                           |
| _a_       | 0-1   | _amplitude_    | Sets the amplitude of the _sn_ function                         |
| _δx_      | ±1    | _shift-x_      | Sets the horizontal displacement                                |
| _δy_      | ±1    | _shift-y_      | Sets the vertical displacement                                  |
| _Φ_       | ±90°  | _phase_        | Adjust a harmonic phase offset                                  |
| _m_       | 1-5   | _multiplier_   | Base frequency multiplier                                       |


## Modules

### _sn-vco_

_sn-vco_ is a more or less standard VCO module except that it produces a _sn_ output rather than a _sine_:

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

#### Context Menu

1. _k-rate_   

Sets the rate at which the parameter values are used to recalculate the waveform.

2. _aux-mode_

Sets the AUX output mode:
- _OSC_ outputs a 25Hz 'monitor' waveform
- _SUM_ outputs the summed 25Hz 'monitor' waveform
- _POLY_ outputs the monitor wave form on channel 1 and the summed monitor waveform on channel 2 |


### _sn-vcox_

_sn-vcox_ is an expander module for _sn-vco_ that adds its own waveform to the waveform generated by 
_sn-vco_. Multiple expanders can be stacked to create a waveform using harmonics and/or variations.

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
| _VCO-Σ_    | ±5V   | _VCO-Σ_        | Summed VCOx + VCO audio output                                 |
| _AUX_      | ±5V   | _AUX_          | 2 channel 25Hz signal 'monitor' output for use with the scope  |

#### Context Menu

1. _k-rate_   

Sets the rate at which the parameter values are used to recalculate the waveform.

2. _aux-mode_

Sets the AUX output mode:
- _OSC_ outputs a 25Hz 'monitor' waveform
- _SUM_ outputs the summed 25Hz 'monitor' waveform
- _POLY_ outputs the monitor wave form on channel 1 and the summed monitor waveform on channel 2 |


### _sn-lfo_

_sn-lfo_ is a more or less standard LFO module except that it produces a _sn_ output rather than a _sine_:

| Input       | Range  | Name         | Description                                                    |
|-------------|--------|--------------|----------------------------------------------------------------|
| _frequency_ | 0-10V  | _frequency_  | Polyphonal 1V/octave pitch input, normalized for C4 at 0V      |
| _sync_      | 0-10V  | _sync_       | Trigger input to reset the waveform to start                   |

| Output      | Range  | Name         | Description                                                    |
|-------------|--------|--------------|----------------------------------------------------------------|
| _LFO_       | ±5V    | _LFO_        | Polyphonal LFO signal output                                   |
| _AUX_       | ±5V    | _AUX_        | 2 channel 25Hz signal 'monitor' output for use with the scope  |
| _TRIG_      | 0-10V  | _trigger_    | Trigger output for the AUX output                              |

#### Context Menu

1. _k-rate_   

Sets the rate at which the parameter values are used to recalculate the waveform.

2. _aux-mode_

Sets the AUX output mode:
- _OSC_ outputs a 25Hz 'monitor' waveform
- _SUM_ outputs the summed 25Hz 'monitor' waveform
- _POLY_ outputs the monitor wave form on channel 1 and the summed monitor waveform on channel 2 |

3. _range_ 

Sets the frequency range mapped to the input level:
- 0.1-10Hz (logarithmic)
- 0.2-2Hz (logarithmic)
- 0.5-20Hz (linear)


### _sn-lfox_

_sn-lfox_ is an expander module for _SN LFO_ that adds its own waveform to the waveform generated by 
_sn-lfo_. Multiple expanders can be stacked to create a waveform using harmonics and/or variations.

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
| _LFO_      | ±5V   | _LFO_          | Polyphonal LFO signal output                                   |
| _LFO-Σ_    | ±5V   | _LFO-Σ_        | Summed LFOx + LFO audio output                                 |
| _AUX_      | ±5V   | _AUX_          | 2 channel 25Hz signal 'monitor' output for use with the scope  |

#### Context Menu

1. _k-rate_   

Sets the rate at which the parameter values are used to recalculate the waveform.

2. _aux-mode_

Sets the AUX output mode:
- _OSC_ outputs a 25Hz 'monitor' waveform
- _SUM_ outputs the summed 25Hz 'monitor' waveform
- _POLY_ outputs the monitor wave form on channel 1 and the summed monitor waveform on channel 2 |


## Patches

### [*sn-vcv-vco*](https://github.com/transcriptaze/sn-vcv/blob/main/documentation/patches/sn-vcv-vco.vcv)

Basic patch to demonstrate the operation of the _sn-vco_ module.

_Requires_:
1. _Fundamental_ modules

### [*sn-vcv-vcox*](https://github.com/transcriptaze/sn-vcv/blob/main/documentation/patches/sn-vcv-vcox.vcv)

Basic patch to demonstrate stacking of harmonics with the the _sn-vcox_ expander module to create more complex VCO 
wave shapes.

_Requires_:
1. _Fundamental_ modules

### [*sn-vcv-lfo*](https://github.com/transcriptaze/sn-vcv/blob/main/documentation/patches/sn-vcv-lfo.vcv)

Basic patch to demonstrate the operation of the _sn-lfo_ module.

_Requires_:
1. _Fundamental_ modules

### [*sn-vcv-lfox*](https://github.com/transcriptaze/sn-vcv/blob/main/documentation/patches/sn-vcv-lfox.vcv)

Basic patch to demonstrate stacking of harmonics with the the _sn-lfox_ expander module to create more complex LFO
wave shapes.

_Requires_:
1. _Fundamental_ modules

## License

[GPL-3.0](https://github.com/transcriptaze/sn-vcv/blob/master/LICENSE). 



