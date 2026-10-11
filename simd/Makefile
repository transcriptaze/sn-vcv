SRC = *.cpp sn/*.cpp 
INCLUDE = *.h sn/*.hpp
TARGET = bin/test

# CXXFLAGS = -std=c++11 -stdlib=libc++ -fPIC -MMD -MP -g -O3 -funsafe-math-optimizations -fno-omit-frame-pointer -Wall -Wextra -Wno-unused-parameter -Wno-vla-extension -march=armv8-a+fp+simd -mmacosx-version-min=11.0
CXXFLAGS = -std=c++17 -stdlib=libc++ -fPIC -MMD -MP -g -O3 -funsafe-math-optimizations -fno-omit-frame-pointer -Wall -Wextra -Wno-unused-parameter -Wno-vla-extension -march=armv8-a+fp+simd -mmacosx-version-min=11.0

RUNS ?= 5

.PHONY: baseline


all: $(TARGET)

format: $(SRC) $(INCLUDE)
	clang-format -i $(SRC) -i $(INCLUDE)

build: format
	mkdir -p bin
	clang++ $(CXXFLAGS) $(SRC) -o bin/test 
	        
test: build
	./bin/test

baseline: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/baseline.cpp sn/*.cpp -o bin/baseline
	@time for i in $$(seq 1 $(RUNS)); do ./bin/baseline; done

reference: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/reference.cpp sn/*.cpp -o bin/reference
	@time for i in $$(seq 1 $(RUNS)); do ./bin/reference; done

baseline2: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/baseline2.cpp sn/*.cpp -o bin/baseline2
	@time for i in $$(seq 1 $(RUNS)); do ./bin/baseline2; done

riffles: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/riffles.cpp sn/*.cpp -o bin/riffles
	@time for i in $$(seq 1 $(RUNS)); do ./bin/riffles; done

unpacked: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/unpacked.cpp sn/*.cpp -o bin/unpacked
	@time for i in $$(seq 1 $(RUNS)); do ./bin/unpacked; done

sincos: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/sincos.cpp sn/*.cpp -o bin/sincos
	@time for i in $$(seq 1 $(RUNS)); do ./bin/sincos; done

sincos2: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/sincos2.cpp sn/*.cpp -o bin/sincos2
	@time for i in $$(seq 1 $(RUNS)); do ./bin/sincos2; done

sincos2A: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/sincos2A.cpp sn/*.cpp -o bin/sincos2A
	@time for i in $$(seq 1 $(RUNS)); do ./bin/sincos2A; done

float4A: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4A.cpp sn/*.cpp -o bin/float4A
	@time for i in $$(seq 1 $(RUNS)); do ./bin/float4A; done

float4B: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4B.cpp sn/*.cpp -o bin/float4B
	@time for i in $$(seq 1 $(RUNS)); do ./bin/float4B; done

float4C: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4C.cpp sn/*.cpp -o bin/float4C
	@time for i in $$(seq 1 $(RUNS)); do ./bin/float4C; done

float4D: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs benchmarks/float4D.cpp sn/*.cpp -o bin/float4D
	@time for i in $$(seq 1 $(RUNS)); do ./bin/float4D; done
