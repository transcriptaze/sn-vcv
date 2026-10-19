SRC = *.cpp sn/*.cpp 
INCLUDE = *.h sn/*.hpp
TARGET = bin/test

CXXFLAGS = -std=c++17 -stdlib=libc++ -fPIC -MMD -MP -g -O3 -funsafe-math-optimizations -fno-omit-frame-pointer -Wall -Wextra -Wno-unused-parameter -Wno-vla-extension -march=armv8-a+fp+simd -mmacosx-version-min=11.0

RUNS ?= 5

.PHONY: baseline


all: $(TARGET)

format: $(SRC) $(INCLUDE)
	clang-format -i $(SRC) -i $(INCLUDE)
	clang-format -i benchmarks/*.cpp

build: format
	mkdir -p bin
	clang++ $(CXXFLAGS) $(SRC) -o bin/test 
	        
test: build
	./bin/test

baseline:  format
	clang++ $(CXXFLAGS) benchmarks/baseline.cpp sn/*.cpp -o bin/baseline
	hyperfine --warmup 2 ./bin/baseline

baseline2: format
	clang++ $(CXXFLAGS) benchmarks/baseline2.cpp sn/*.cpp -o bin/baseline2
	hyperfine --warmup 2 ./bin/baseline2
/reference: format
	clang++ $(CXXFLAGS) benchmarks/reference.cpp sn/*.cpp -o bin/reference
	hyperfine --warmup 2 ./bin/reference

riffles: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/riffles.cpp sn/*.cpp -o bin/riffles
	hyperfine --warmup 2 ./bin/riffles

unpacked: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/unpacked.cpp sn/*.cpp -o bin/unpacked
	hyperfine --warmup 2 ./bin/unpacked

sincos: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/sincos.cpp sn/*.cpp -o bin/sincos
	hyperfine --warmup 2 ./bin/sincos

sincos2: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/sincos2.cpp sn/*.cpp -o bin/sincos2
	hyperfine --warmup 2 ./bin/sincos2

sincos2A: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/sincos2A.cpp sn/*.cpp -o bin/sincos2A
	hyperfine --warmup 2 ./bin/sincos2A

float4A: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4A.cpp sn/*.cpp -o bin/float4A
	hyperfine --warmup 2 ./bin/float4A

float4AX: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4AX.cpp sn/*.cpp -o bin/float4AX
	hyperfine --warmup 2 ./bin/float4AX

float4AY: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4AY.cpp sn/*.cpp -o bin/float4AY
	hyperfine --warmup 2 ./bin/float4AY

float4B: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4B.cpp sn/*.cpp -o bin/float4B
	hyperfine --warmup 2 ./bin/float4B

float4C: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4C.cpp sn/*.cpp -o bin/float4C
	hyperfine --warmup 2 ./bin/float4C

float4D: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4D.cpp sn/*.cpp -o bin/float4D
	hyperfine --warmup 2 ./bin/float4D

hyperfine:
	clang++ $(CXXFLAGS) benchmarks/reference.cpp sn/*.cpp -o bin/reference
	clang++ $(CXXFLAGS) benchmarks/riffles.cpp  sn/*.cpp -o bin/riffles
	clang++ $(CXXFLAGS) benchmarks/unpacked.cpp sn/*.cpp -o bin/unpacked
	clang++ $(CXXFLAGS) benchmarks/sincos.cpp   sn/*.cpp -o bin/sincos
	clang++ $(CXXFLAGS) benchmarks/sincos2.cpp  sn/*.cpp -o bin/sincos2
	clang++ $(CXXFLAGS) benchmarks/sincos2A.cpp sn/*.cpp -o bin/sincos2A
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4A.cpp  sn/*.cpp -o bin/float4A
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4AX.cpp sn/*.cpp -o bin/float4AX
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4AY.cpp sn/*.cpp -o bin/float4AY
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4B.cpp  sn/*.cpp -o bin/float4B
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4C.cpp  sn/*.cpp -o bin/float4C
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4D.cpp  sn/*.cpp -o bin/float4D

# 	hyperfine --warmup 1  --export-markdown ./workdir/sn.md \
# 	          ./bin/reference ./bin/riffles  ./bin/unpacked \
# 	          ./bin/sincos    ./bin/sincos2  ./bin/sincos2A \
# 	          ./bin/float4A   ./bin.float4AX \
# 	          ./bin/float4B   ./bin/float4C ./bin/float4D

	hyperfine --warmup 1  --export-markdown ./workdir/sn.md \
	          ./bin/float4A ./bin/float4AX ./bin/float4AY ./bin/float4B ./bin/float4C ./bin/float4D
