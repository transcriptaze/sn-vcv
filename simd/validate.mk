SRC = main.cpp validate.cpp validate/*.cpp 
INCLUDE = validate/*.hpp

E ?= 0.2
S ?= 1.0

CXXFLAGS = -std=c++17 -stdlib=libc++ -fPIC -MMD -MP -g -O3 -funsafe-math-optimizations -fno-omit-frame-pointer -Wall -Wextra -Wno-unused-parameter -Wno-vla-extension -march=armv8-a+fp+simd -mmacosx-version-min=11.0

format: $(SRC)
	clang-format -i $(SRC) -i $(INCLUDE)

build: format
	mkdir -p bin
	clang++ $(CXXFLAGS) -I ./sdk -I ./sdk/simd -I ./libs $(SRC) -o bin/validate
	        
validate: build
	./bin/validate

svg:
	gnuplot -e "e=$(E); s=$(S); infile='./gnuplot/reference.dat'; outfile='./workdir/reference.svg'" gnuplot/reference.gp
