SRC = benchmarks/*.cpp
INCLUDE = benchmarks/*.hpp
TARGET = bin/test

CXXFLAGS = -std=c++17 -stdlib=libc++ -fPIC -MMD -MP -g -O3 -funsafe-math-optimizations -fno-omit-frame-pointer -Wall -Wextra -Wno-unused-parameter -Wno-vla-extension -march=armv8-a+fp+simd -mmacosx-version-min=11.0

format: $(SRC) $(INCLUDE)
	clang-format -i $(SRC) -i $(INCLUDE)
	clang-format -i benchmarks/*.cpp

build: format
	mkdir -p bin
	clang++ $(CXXFLAGS) $(SRC) -o bin/test 
	        
baseline:  format
	clang++ $(CXXFLAGS) benchmarks/baseline.cpp -o bin/baseline
	./bin/baseline

sine:  format
	clang++ $(CXXFLAGS) benchmarks/sine.cpp -o bin/sine
	./bin/sine

sinef:  format
	clang++ $(CXXFLAGS) benchmarks/sinef.cpp -o bin/sinef
	./bin/sinef

sine4:  format
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sine4.cpp -o bin/sine4
	./bin/sine4

polynomial:  format
	clang++ $(CXXFLAGS) benchmarks/polynomial.cpp -o bin/polynomial
	./bin/polynomial

lookup:  format
	clang++ $(CXXFLAGS) benchmarks/lookup.cpp -o bin/lookup
	./bin/lookup

sn:  format
	clang++ $(CXXFLAGS) benchmarks/sn.cpp -o bin/sn
	./bin/sn

sn4:  format
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn4.cpp -o bin/sn4
	./bin/sn4

sn4d:  format
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn4d.cpp -o bin/sn4d
	./bin/sn4d

benchmark: format
	clang++ $(CXXFLAGS) benchmarks/baseline.cpp -o bin/baseline
	clang++ $(CXXFLAGS) benchmarks/sine.cpp -o bin/sine
	clang++ $(CXXFLAGS) benchmarks/sinef.cpp -o bin/sinef
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sine4.cpp -o bin/sine4
	clang++ $(CXXFLAGS) benchmarks/polynomial.cpp -o bin/polynomial
	clang++ $(CXXFLAGS) benchmarks/lookup.cpp -o bin/lookup
	clang++ $(CXXFLAGS) benchmarks/sn.cpp -o bin/sn
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn4.cpp -o bin/sn4
	clang++ $(CXXFLAGS) -I ../sdk -I ../sdk/simd -I ../libs benchmarks/sn4d.cpp -o bin/sn4d

	mkdir -p workdir
	hyperfine --warmup 2  --export-markdown ./workdir/sine.md \
              ./bin/baseline ./bin/sine ./bin/sinef ./bin/sine4 ./bin/polynomial ./bin/lookup \
              ./bin/sn ./bin/sn4 ./bin/sn4d

