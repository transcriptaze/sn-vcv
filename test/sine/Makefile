SRC = benchmarks/*.cpp
INCLUDE = benchmarks/*.hpp
TARGET = bin/test

CXXFLAGS = -std=c++17 -stdlib=libc++ -fPIC -MMD -MP -g -O3 -funsafe-math-optimizations -fno-omit-frame-pointer -Wall -Wextra -Wno-unused-parameter -Wno-vla-extension -march=armv8-a+fp+simd -mmacosx-version-min=11.0

format: $(SRC) $(INCLUDE)
	clang-format -i $(SRC) -i $(INCLUDE)
	clang-format -i benchmarks/*.cpp

baseline:  format
	clang++ $(CXXFLAGS) benchmarks/baseline.cpp -o bin/baseline
	./bin/baseline

sine:  format
	clang++ $(CXXFLAGS) -DVALIDATE benchmarks/sine.cpp -o bin/sine
	./bin/sine

sinef:  format
	clang++ $(CXXFLAGS) -DVALIDATE benchmarks/sinef.cpp -o bin/sinef
	./bin/sinef

sine4:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sine4.cpp -o bin/sine4
	./bin/sine4

polynomial:  format
	clang++ $(CXXFLAGS) -DVALIDATE benchmarks/polynomial.cpp -o bin/polynomial
	./bin/polynomial

lookup:  format
	clang++ $(CXXFLAGS) -DVALIDATE benchmarks/lookup.cpp -o bin/lookup
	./bin/lookup

sn:  format
	clang++ $(CXXFLAGS) -DVALIDATE benchmarks/sn.cpp -o bin/sn
	./bin/sn

sn4:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn4.cpp -o bin/sn4
	./bin/sn4

sn4d:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn4d.cpp -o bin/sn4d
	./bin/sn4d

sn-lookup:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup.cpp -o bin/sn_lookup
	./bin/sn_lookup

sn-lookup-sqrt:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sqrt.cpp -o bin/sn_lookup_sqrt
	./bin/sn_lookup_sqrt

sn-lookup-rsqrt:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_rsqrt.cpp -o bin/sn_lookup_rsqrt
	./bin/sn_lookup_rsqrt

sn-lookup-sincos:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos.cpp -o bin/sn_lookup_sincos
	./bin/sn_lookup_sincos

sn-lookup-sincos2:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2.cpp -o bin/sn_lookup_sincos2
	./bin/sn_lookup_sincos2

sn-lookup-sincos2x:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2x.cpp -o bin/sn_lookup_sincos2x
	./bin/sn_lookup_sincos2x

sn-lookup-sincos2f:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2f.cpp -o bin/sn_lookup_sincos2f
	./bin/sn_lookup_sincos2f

sn-lookup-sincos2fx:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2fx.cpp -o bin/sn_lookup_sincos2fx
	./bin/sn_lookup_sincos2fx

sn-lookup-sincos2d:  format
	clang++ $(CXXFLAGS) -DVALIDATE -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2d.cpp -o bin/sn_lookup_sincos2d
	./bin/sn_lookup_sincos2d

benchmark: format
	clang++ $(CXXFLAGS) benchmarks/baseline.cpp -o bin/baseline
	clang++ $(CXXFLAGS) benchmarks/sine.cpp -o bin/sine
	clang++ $(CXXFLAGS) benchmarks/sinef.cpp -o bin/sinef
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sine4.cpp -o bin/sine4
	clang++ $(CXXFLAGS) benchmarks/polynomial.cpp -o bin/polynomial
	clang++ $(CXXFLAGS) benchmarks/lookup.cpp -o bin/lookup
	clang++ $(CXXFLAGS) benchmarks/sn.cpp -o bin/sn
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn4.cpp  -o bin/sn4
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn4d.cpp -o bin/sn4d
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup.cpp           -o bin/sn_lookup
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sqrt.cpp      -o bin/sn_lookup_sqrt
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_rsqrt.cpp     -o bin/sn_lookup_rsqrt
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos.cpp    -o bin/sn_lookup_sincos
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2.cpp   -o bin/sn_lookup_sincos2
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2x.cpp  -o bin/sn_lookup_sincos2x
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2f.cpp  -o bin/sn_lookup_sincos2f
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2fx.cpp -o bin/sn_lookup_sincos2fx
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2d.cpp  -o bin/sn_lookup_sincos2d

	mkdir -p workdir
	hyperfine --warmup 2  --export-markdown ./workdir/sine.md \
              ./bin/baseline ./bin/sine ./bin/sinef ./bin/sine4 ./bin/polynomial ./bin/lookup \
              ./bin/sn ./bin/sn4 ./bin/sn4d \
              ./bin/sn_lookup ./bin/sn_lookup_sqrt ./bin/sn_lookup_rsqrt \
              ./bin/sn_lookup_sincos ./bin/sn_lookup_sincos2 ./bin/sn_lookup_sincos2x ./bin/sn_lookup_sincos2f ./bin/sn_lookup_sincos2fx ./bin/sn_lookup_sincos2d

benchmark2: format
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup.cpp           -o bin/sn_lookup
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sqrt.cpp      -o bin/sn_lookup_sqrt
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_rsqrt.cpp     -o bin/sn_lookup_rsqrt
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos.cpp    -o bin/sn_lookup_sincos
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2.cpp   -o bin/sn_lookup_sincos2
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2x.cpp  -o bin/sn_lookup_sincos2x
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2f.cpp  -o bin/sn_lookup_sincos2f
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2fx.cpp -o bin/sn_lookup_sincos2fx
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2d.cpp  -o bin/sn_lookup_sincos2d

	mkdir -p workdir
	hyperfine --warmup 2  --export-markdown ./workdir/debug.md \
              ./bin/sn_lookup ./bin/sn_lookup_sqrt ./bin/sn_lookup_rsqrt \
              ./bin/sn_lookup_sincos ./bin/sn_lookup_sincos2 ./bin/sn_lookup_sincos2x ./bin/sn_lookup_sincos2f ./bin/sn_lookup_sincos2fx ./bin/sn_lookup_sincos2d

asm: format
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/asm_lookup_sincos2.cpp  benchmarks/asm_sn.cpp benchmarks/asm_snv.cpp  benchmarks/asm_lut.cpp  benchmarks/asm_table.cpp -o bin/asm_lookup_sincos2
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/asm_lookup_sincos2f.cpp benchmarks/asm_sn.cpp benchmarks/asm_snvf.cpp benchmarks/asm_lut.cpp  benchmarks/asm_table.cpp -o bin/asm_lookup_sincos2f
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/asm_lookup_sincos2d.cpp benchmarks/asm_sn.cpp benchmarks/asm_snvd.cpp benchmarks/asm_lutd.cpp benchmarks/asm_table.cpp -o bin/asm_lookup_sincos2d
	./bin/asm_lookup_sincos2
	./bin/asm_lookup_sincos2f
	./bin/asm_lookup_sincos2d
	clang++ -S $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/asm_lut.cpp  -o asm_lut.s  | c++filt
	clang++ -S $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/asm_lutd.cpp -o asm_lutd.s | c++filt
	clang++ -S $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/asm_snv.cpp -o asm_snv.s   | c++filt
	clang++ -S $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/asm_snvf.cpp -o asm_snvf.s | c++filt
	clang++ -S $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/asm_snvd.cpp -o asm_snvd.s | c++filt

debug: format
	clang++ $(CXXFLAGS) benchmarks/sn.cpp -o bin/sn
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup.cpp           -o bin/sn_lookup
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sqrt.cpp      -o bin/sn_lookup_sqrt
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_rsqrt.cpp     -o bin/sn_lookup_rsqrt
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos.cpp    -o bin/sn_lookup_sincos
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2.cpp   -o bin/sn_lookup_sincos2
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2x.cpp  -o bin/sn_lookup_sincos2x
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2f.cpp  -o bin/sn_lookup_sincos2f
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2fx.cpp -o bin/sn_lookup_sincos2fx
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn_lookup_sincos2d.cpp  -o bin/sn_lookup_sincos2d

	mkdir -p workdir
	hyperfine --warmup 2  --export-markdown ./workdir/debug.md \
              ./bin/sn \
              ./bin/sn_lookup ./bin/sn_lookup_sqrt ./bin/sn_lookup_rsqrt \
              ./bin/sn_lookup_sincos ./bin/sn_lookup_sincos2 ./bin/sn_lookup_sincos2x ./bin/sn_lookup_sincos2f ./bin/sn_lookup_sincos2fx ./bin/sn_lookup_sincosd

