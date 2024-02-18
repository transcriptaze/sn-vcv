# TODO

- [ ] Antialiasing
      - https://github.com/transcriptaze/sn-vcv/issues/3
      - [x] Indicator
      - [x] Context menu
      - [ ] Clamp input frequency
      - [x] _None_
      - [x] 1X1F: 12.5kHz LPF
      - [ ] 1X2F: 2x16kHz LPF
      - [ ] 2X1F
      - [ ] 4X1F
      - [ ] 2X2F
      - [ ] 4X2F
      - [ ] PI(d)
      - [ ] FFT/wavetable
      - [ ] Anti-aliased AUX
      - [ ] Anti-aliasing indicator
           - [ ] enable/disable
           - [ ] show option + aliasing
      - [ ] ALIASING.md
      - [ ] sn-vcox
      - [ ] DC blocking

- [x] Better blurbs in plugin.json
- [x] Fix tags in plugin.json
- [x] Initialise module in sn-vco-channels
- [x] Invoke parent step() in sn-vc-channels


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
