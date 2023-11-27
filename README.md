![build](https://github.com/transcriptaze/sn-vcv/workflows/build/badge.svg)

# sn-vcv

A set of VCV modules for the _sn_ experimental additive synthesizer which uses the _sn_ function as the 
generator function instead of the more usual _sine_. The modules comprise:
- a _stackable_ VCO, which generates an audio signal with a smoothly adjustable shape
- a _stackable_ LFO, which generates a low frequency control voltage signal with a smoothly adjustable shape.

<img width="96" src="documentation/images/sine.png"> <img width="96" src="documentation/images/square.png"> <img width="96" src="documentation/images/triangle.png"> <img width="96" src="documentation/images/sawtooth.png"> <img width="96" src="documentation/images/spike.png"> <img width="96" src="documentation/images/bumps.png"> <img width="96" src="documentation/images/wonky.png">

The modules don't do anything particuarly radical but are an interesting way to add subtle variations to
a sound e.g. if you're already using a VCO, replacing it with _sn-vco_ is a way to add a touch more tonal
interest, e.g.:

[Greensleeves](/documentation/examples/greensleeves.mp3)

- [Background](#background)
- [Modules](#modules)
   - [sn-vco](/documentation/sn-vco.md)
   - [sn-vcox](/documentation/sn-vcox.md)
   - [sn-lfo](/documentation/sn-lfo.md)
   - [sn-lfox](/documentation/sn-lfox.md)
- [Patches](#patches)
- [License](#license)


## Background

Mathematically a _sine_ function is a special case of the Jacobi elliptic _sn_ function (in the same way that
a circle is a special case of an ellipse). The _sn-vcv_ VCV modules use the _sn_ function rather than the
_sine_ function to generate harmonically rich waveforms that are also continuous and differentiable.

By modifying the parameters of the _sn_ function it is possible to smoothly morph between a sine wave,
a square (well, square'ish) wave, a triangular'ish wave and a sawtooth. There is a [web app](https://snyth.pages.dev)
which may make this a bit clearer than this umm, very extremely brief explanation, and there is a little
more detail [here](https://github.com/transcriptaze/snyth) if you're really curious.


### Parameters

The modules share a common set of parameters that determine the wave shape:

| Parameter | Range | Name           | Description                                                     |
|-----------|-------|----------------|-----------------------------------------------------------------|
| _ε_       | ±1    | _eccentricity_ | Sets the _eccentricity_ of the _sn_ generator function          |
| _s_       | 0-5   | _sensitivity_  | Fine adjust for the _eccentricity_ set by the _ε_ control       |
| _θ_       | ±90°  | _rotation_     | Sets the rotation angle of he _sn_ generator function           |
| _a_       | 0-1   | _amplitude_    | Sets the amplitude of the harmonic component                    |
| _δx_      | ±1    | _shift-x_      | Sets the horizontal shift of the _sn_ generator function        |
| _δy_      | ±1    | _shift-y_      | Sets the vertical shift of the _sn_ generator function          |
| _Φ_       | ±90°  | _phase_        | (LFO only) Adjust the phase offset of a harmonic                |
| _m_       | 1-5   | _multiplier_   | Base frequency multiplier                                       |

## Installation

### VCV Rack Library

### Manual

### Building from source


## Modules

### [sn-vco](/documentation/sn-vco.md)

_sn-vco_ is a more-or-less standard VCO module except that it produces a _sn_ output rather than a _sine_
which (as described above), allows the waveshape to be morphed between a sine, square, triangular and sawtooth
wave. 

### [sn-vcox](/documentation/sn-vcox.md)

_sn-vcox_ is an expander module for the _sn-vco_ module that adds a harmonic component to the output of 
_sn-vco_. Multiple expanders can be stacked to create a waveform using harmonics and/or variations.

### [sn-lfo](/documentation/sn-lfo.md)

_sn-lfo_ is a more or less standard LFO module except that it produces a _sn_ output rather than a _sine_,
allowing the waveshape to be morphed between a sine, square, triangular and sawtooth wave and pretty much
everything in between. Although it can be used on its own, the module is really intended to be used as
the base for stacked _sn-lfox_ expanders. 

### [sn-lfox](/documentation/sn-lfox.md)

_sn-lfox_ is an expander module for the _sn-lfo_ module that adds a harmonic wave to the output of _sn-lfo_.
Multiple expanders can be stacked to create a waveform using harmonics and/or variations.

## Demo Patches

### Basic operation 

The basic patches have been kept as simple as possible and require only the _VCV Fundamental_ modules included
with _VCV Rack 2_ and are mostly intended just to demonstrate the basic setup and operation of the modules rather
than as shining examples.

#### [sn-vcv-vco](/documentation/patches/sn-vcv-vco.vcv)

Hooks up 3 standard LFOs to a single _sn-vco_ module to demonstrate the basic shape shifting.

#### [sn-vcv-vcox](/documentation/patches/sn-vcv-vcox.vcv)

Extends the basic _sn-vco_ patch by stacking two _sn-vcox_ modules on to the_sn-vco_ module and generating
waveforms at the 2nd and 4th harmonics. The LFOs from the _sn-vco_ demo are routed to the parameter inputs
on each of the modules so that each module shape shifts independently. Sounds just about as horrible as you
would expect but looks pretty.

#### [sn-vcv-lfo](/documentation/patches/sn-vcv-lfo.vcv)

Replaces the LFOs in the basic _sn-vco_ patch with three independent _sn-lfo_ modules each with its own
waveform. The effect is to make the _sn-vco_ output morph in some unexpected ways - still sounds weird
rather than actually good, but at least it's starting to sound a little interesting.

#### [*sn-vcv-lfox*](/documentation/patches/sn-vcv-lfox.vcv)

Replaces the independent LFOs in the basic _sn-lfo_ patch with an _sn-lfo_ module and two stacked 
_sn-lfox_ expanders for a slightly different effect. Oddly ambient'ish in a way .. maybe with some
reverb .. 

## License

The source code and panel files are licensed under [Gnu General Public License v3](/LICENSE). 



