# TODO

- [ ] [Antialiasing](https://github.com/transcriptaze/sn-vcv/issues/3)
      - [x] Context menu
      - [x] sn-vco
      - [x] sn-vcox
      - [ ] ALIASING.md
      - (?) PID

- [ ] Optimisation
      - [ ] Baseline
      - https://community.vcvrack.com/t/anyone-has-done-profiling-on-a-module-code/7537/
      - https://wordsandbuttons.online/challenge_your_performance_intuition_with_cpp_sine.html
      - https://github.com/grafana/pyroscope
      - https://stackoverflow.com/questions/11445619/profiling-c-on-mac-os-x
      - https://stackoverflow.com/questions/3764978/why-hypot-function-is-so-slow
      - https://mattkretz.github.io/2019/01/18/optimizing-hypot-for-simd.html


- [ ] Set k-rate in ms
- [ ] Move sn_vco::dcblocking and sn_vc::antialias to  DCB and AA

- [x] DC blocking
- [x] Clamp input frequency
- [x] Better blurbs in plugin.json
- [x] Fix tags in plugin.json
- [x] Initialise module in sn-vco-channels
- [x] Invoke parent step() in sn-vc-channels

- [ ] Unit tests

## R & D

1. https://www.quantamagazine.org/mathematicians-identify-the-best-versions-of-iconic-shapes-20240105/#
2. https://www.quantamagazine.org/geometers-engineer-new-tools-to-wrangle-spacecraft-orbits-20240415/
3. https://www.quantamagazine.org/elliptic-curve-murmurations-found-with-ai-take-flight-20240305/
4. https://www.youtube.com/watch?v=fxzrpfezezE
5. https://www.youtube.com/watch?v=3depB4EZT7c
6. https://www.soundonsound.com/techniques/synthesizing-strings-string-machines


## Notes and References

1. https://github.com/qno/vcv-plugin-github-actions-example/blob/main/.github/workflows/build-plugin.yml
2. https://community.vcvrack.com/t/help-needed-for-beta-testing/20711/30
3. https://community.vcvrack.com/t/automated-building-and-releasing-plugins-on-github-with-github-actions/11364
4. https://community.vcvrack.com/t/rack-development-blog/5864/84
5. https://herbie.uwplse.org
6. `convert *.png -crop 392x331+281+90 cropped/screenshot%d.png`
7. https://github.com/nullJaX/vcvrack-patchable-devices
8. https://www.youtube.com/watch?v=yV52TMdGkng
9. https://surge-synthesizer.github.io/manual-xt/#string


