SRC = *.cpp sn/*.cpp 
INCLUDE = *.h sn/*.hpp
TARGET = bin/test

CXXFLAGS = -std=c++11 -stdlib=libc++ -fPIC -MMD -MP -g -O3 -funsafe-math-optimizations -fno-omit-frame-pointer -Wall -Wextra -Wno-unused-parameter -Wno-vla-extension -march=armv8-a+fp+simd -mmacosx-version-min=11.0

all: $(TARGET)

format: $(SRC) $(INCLUDE)
	clang-format -i $(SRC) -i $(INCLUDE)

build: format
	mkdir -p bin
	clang++ $(CXXFLAGS) $(SRC) -o bin/test 
	        
test: build
	./bin/test

