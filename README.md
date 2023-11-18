![build](https://github.com/transcriptaze/sn-vcv/workflows/build/badge.svg)

# sn-vcv

A set of VCV modules for the _sn_ experimental additive synthesizer which uses the _Jacobi ellipse_ as the 
generator function.

## Background

Mathematically a _sine_ function is a special case of the Jacobi elliptic _sn_ function (in the same way that
a circle is a special case of an ellipse). The _sn_ VCV modules use the _sn_ function rather than the
_sine_ function to generate harmonically rich waveforms that are also continuous and differentiable.

By modifying the parameters of the _sn_ function it is possible to smoothly morph between a sine wave,
a square (well, square'ish) wave, a triangular wave and a sawtooth.

### Parameters

The modules use the following parameters to determine the wave shape:

| Parameter | Range | Name           | Description                                                                 |
|-----------|-------|----------------|-----------------------------------------------------------------------------|
| _ε_       | ±1    | _eccentricity_ | Sets the eccentricity of the generating ellipse (0 corresponds to a circle) |
| _s_       | 0-5   | _sensitivity_  | 'Fine' adjustment for the _eccentricity_                                    |
| _θ_       | ±90°  | _rotation_     | Rotate the ellipse around the origin                                        |
| _a_       | 0-1   | _amplitude_    | Amplitude of the _sn_ function                                              |
| _δx_      | ±1    | _shift-x_      | Horizontal displacement                                                     |
| _δy_      | ±1    | _shift-y_      | Vertical displacement                                                       |
| _Φ_       | ±90°  | _phase_        | Phase offset to adjust the phase of a harmonic                              |
| _m_       | 1-5   | _multiplier_   | Base frequency multiplier                                                   |


## Modules

### _sn-vco_

_SN VCO_ is a traditional VCO module except that it produces a _sn_ output rather than a _sine_:

| Input      | Range | Name           | Description                                                    |
|------------|-------|----------------|----------------------------------------------------------------|
| _pitch_    | ±5V   | _frequency_    | Polyphonal 1V/octave pitch input, normalized for C4 at 0V      |
| _velocity_ | 0-10V | _velocity_     | Polyphonal MIDI note velocity input. Defaults to 10V (127)     |
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
| _AUX_      | ±5V   | _AUX_          | 2 channel 25Hz signal 'monitor' output for use with the scope  |
| _TRIG_     | 0-10V | _trigger_      | Trigger output for the AUX output                              |


### _sn-vcox_

### _sn-lfo_

### _sn-lfox_

## Patches

### [*sn-vcv-vco*](https://github.com/transcriptaze/sn-vcv/blob/main/documentation/patches/sn-vcv-vco.vcv)

Simple patch to show the basic operation of the _sn-vco_ module.

_Requires_:
1. _Fundamental_ modules

### [*sn-vcv-vcox*](https://github.com/transcriptaze/sn-vcv/blob/main/documentation/patches/sn-vcv-vcox.vcv)

Simple patch to show 'stacking' with the the _sn-vcox_ expander module to create more complex VCO wave shapes.

_Requires_:
1. _Fundamental_ modules

### [*sn-vcv-lfo*](https://github.com/transcriptaze/sn-vcv/blob/main/documentation/patches/sn-vcv-lfo.vcv)

Simple patch to show the basic operation of the _sn-lfo_ module.

_Requires_:
1. _Fundamental_ modules

### [*sn-vcv-lfox*](https://github.com/transcriptaze/sn-vcv/blob/main/documentation/patches/sn-vcv-lfox.vcv)

Simple patch to show 'stacking' with the the _sn-lfox_ expander module to create more complex LFO shapes.

_Requires_:
1. _Fundamental_ modules

## Acknowledgements

