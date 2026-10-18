# TODO

- [x] benchmark baseline implementation
- [x] benchmark riffles implementation
- [x] measure CPU usage _in situ_
- [ ] merge benchmark and sn code (i.e. make benchmarks standalone)
- [ ] validation suite

- [ ] loop optimisation
       - only the last value of a 4x oversample is used. Can approximate the others
       - polyphase interpolation ???


```
    for (size_t i = 0; i <= 360; i++) {
        double α = radians(static_cast<double>(i));
    
        // clang-format off
        std::cout << std::fixed
          << std::setw(5) << std::setprecision(1) << static_cast<double>(i)
          << "  "
          << std::setw(7) << std::setprecision(5) << sn.υ(α)
          << "\n";
        // clang-format on
    }
```
