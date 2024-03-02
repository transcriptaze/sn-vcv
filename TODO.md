# TODO

- [ ] [Antialiasing](https://github.com/transcriptaze/sn-vcv/issues/3)
      - [x] Context menu
      - [ ] sn-vco
            - [x] Use unfiltered VCO in sn message
            - [x] _None_
            - [x] 1X1F: 12.5kHz LPF
            - [x] 1X2F: 2x16kHz LPF
            - [x] 2X1F: 2x oversampling, 16kHz LPF
            - [ ] 2X2F: 2x oversampling, 2x16kHz LPF
            - [ ] 4X1F: 2x oversampling, 16kHz LPF
            - [ ] 4X2F: 2x oversampling, 2x16kHz LPF
      - [ ] sn-vcox
            - [ ] Use unfiltered VCO in sn message
            - [x] _None_
            - [x] 1X1F
            - [x] 1X2F
            - [ ] 2x1F

      - [ ] Anti-aliasing indicator
           - [ ] enable/disable
           - [ ] show option + aliasing
      - (?) PI(d)
      - (?) FFT/wavetable
      - [ ] ALIASING.md
      - [ ] DC blocking
      - [ ] Clamp input frequency

- [ ] e directions are switched between sn-vco and sn-vcox
- [x] Better blurbs in plugin.json
- [x] Fix tags in plugin.json
- [x] Initialise module in sn-vco-channels
- [x] Invoke parent step() in sn-vc-channels
- [ ] Benchmarks
- [ ] Unit tests


## Notes and References

1. https://github.com/qno/vcv-plugin-github-actions-example/blob/main/.github/workflows/build-plugin.yml
2. https://community.vcvrack.com/t/help-needed-for-beta-testing/20711/30
3. https://community.vcvrack.com/t/automated-building-and-releasing-plugins-on-github-with-github-actions/11364
4. https://community.vcvrack.com/t/rack-development-blog/5864/84
5. https://herbie.uwplse.org
6. `convert *.png -crop 392x331+281+90 cropped/screenshot%d.png`
7. https://github.com/nullJaX/vcvrack-patchable-devices
8. https://www.youtube.com/watch?v=yV52TMdGkng

9. Antialiasing
      - https://github.com/squinkylabs/Demo/blob/main/docs/aliasing2.md
      - https://dsp.stackexchange.com/questions/91854/benefit-of-anti-aliasing-for-wideband-finite-time-duration-signals-prior-to-samp
      - https://dsp.stackexchange.com/questions/15974/digital-anti-aliasing-filter-for-waveform-band-limiting/15975#15975
      - https://dsp.stackexchange.com/questions/22793/does-msaa-make-sense-in-audio-1d-data-streams/22828#22828
      - https://dsp.stackexchange.com/questions/9205/can-we-have-a-digital-anti-aliasing-filter/9239#9239
      - https://dsp.stackexchange.com/questions/2608/is-there-such-a-thing-as-band-limited-non-linear-distortion/28787#28787
      - https://dsp.stackexchange.com/questions/50716/are-oversampling-and-downsampling-the-same-thing/50725#50725
      - https://observablehq.com/d/a51954c61a72e1ef
      - http://jaggedplanet.com/iir/iir-explorer.asp
      - https://docs.juliadsp.org/stable/filters/#DSP.Filters.PolynomialRatio
      - https://github.com/JunoLab/Weave.jl

10. https://www.researchgate.net/publication/333688079_Oversampling_for_Nonlinear_Waveshaping_Choosing_the_Right_Filters