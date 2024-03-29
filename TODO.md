# TODO

- [ ] Optimisation
      - [ ] Baseline
      - https://wordsandbuttons.online/challenge_your_performance_intuition_with_cpp_sine.html
      - https://github.com/grafana/pyroscope
      - https://stackoverflow.com/questions/11445619/profiling-c-on-mac-os-x
      - https://stackoverflow.com/questions/3764978/why-hypot-function-is-so-slow
      - https://mattkretz.github.io/2019/01/18/optimizing-hypot-for-simd.html


- [ ] [Antialiasing](https://github.com/transcriptaze/sn-vcv/issues/3)
      - [x] Context menu
      - [x] sn-vco
      - [x] sn-vcox
      - [ ] ALIASING.md
      - (?) PID
      - (?) FFT/wavetable

- [ ] Anti-aliasing indicator
      - [x] FFT
      - [x] logarithmic indicator brightness
      - [ ] apply LPF transfer function
            - [x] use whole=True to avoids having to reflect freq
            - [x] None
            - [x] x1f1
            - [x] x1f2
            - [ ] x2f1
            - [ ] x2f2
            - [ ] x4f1
            - [ ] x4f2
      - [ ] aliasing for 16 channels
      - [ ] scale by relative velocity
      - [ ] quick rise, slow decay
      - [ ] enable/disable
      - [ ] AAF indicator
      - [ ] label
      - [ ] bar graph

- [x] DC blocking
      - [x] slow   0.999
      - [x] medium 0.995
      - [x] fast   0.991

- [ ] Set k-rate in ms

- [x] Clamp input frequency
- [x] Better blurbs in plugin.json
- [x] Fix tags in plugin.json
- [x] Initialise module in sn-vco-channels
- [x] Invoke parent step() in sn-vc-channels

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

