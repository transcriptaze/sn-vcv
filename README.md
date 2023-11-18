![build](https://github.com/transcriptaze/sn-vcv/workflows/build/badge.svg)

# sn-vcv

A set of VCV modules for the _sn_ experimental additive synthesizer which uses the _Jacobi ellipse_ as the 
generator function.

## Background

Mathematically a sine can be seen as a special case of a Jacobi elliptic _sn_ function (in the same way that
a circle is a special case of an ellipse). The _sn_ modules uses the _sn_ function rather than the _sine_
function to generate harmonically rich waveforms that are continuous and differentiable.

More usefully, by modifying the parameters of the _sn_ function it is possible to smoothly morph between a sine wave,
a square (well, square'ish) wave, a triangular wave and a sawtooth.

## Modules

### _sn_vco_

The VCO module has seven parameters that affect the wave shape:

- _ε_
- _s_
- _θ_
- _a_
- _δx_
- _δy_
- _m_

The VCO output is a ±5V waveform with a frequency set by the pitch input multiplied by the _m_ parameter.

The AUX output is a ±5V waveform at a fixed 25Hz for displaying the waveform on a scope - the AUX trigger output
can be used to synchronize the scope output to the start of the waveform.

### _sn_vcox_

### _sn_lfo_

### _sn_lfox_

## Acknowledgements

