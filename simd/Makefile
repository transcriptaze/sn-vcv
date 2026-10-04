SRC = *.cpp sn/*.cpp 
INCLUDE = *.h sn/*.hpp
TARGET = bin/test

CXXFLAGS = -std=c++11 -stdlib=libc++ -fPIC -MMD -MP -g -O3 -funsafe-math-optimizations -fno-omit-frame-pointer -Wall -Wextra -Wno-unused-parameter -Wno-vla-extension -march=armv8-a+fp+simd -mmacosx-version-min=11.0

RUNS ?= 5

.PHONY: baseline


all: $(TARGET)

format: $(SRC) $(INCLUDE)
	clang-format -i $(SRC) -i $(INCLUDE)

build: format
	mkdir -p bin
	clang++ $(CXXFLAGS) $(SRC) -o bin/test 
	        
test: all
	./bin/test

baseline: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/baseline.cpp sn/*.cpp -o bin/baseline
	@time for i in $$(seq 1 $(RUNS)); do ./bin/baseline; done

reference: 
	clang-format -i benchmarks/*.cpp
	clang++ $(CXXFLAGS) benchmarks/reference.cpp sn/*.cpp -o bin/reference
	@time for i in $$(seq 1 $(RUNS)); do ./bin/reference; done

