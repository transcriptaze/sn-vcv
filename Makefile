.DEFAULT_GOAL = build

RACK_DIR ?= ../../tools/Rack-SDK

FLAGS +=
CFLAGS +=
CXXFLAGS += 
LDFLAGS +=

SOURCES  += $(wildcard src/*.cpp src/antialias/*.cpp src/dc-blocking/*.cpp src/widgets/*.cpp)
INCLUDES += $(wildcard src/*.hpp src/antialias/*.hpp src/dc-blocking/*.hpp src/widgets/*.hpp)

DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)
DISTRIBUTABLES += $(wildcard presets)

BIN = "/Applications/VCV Rack 2 Free.app/Contents/MacOS/Rack"

format: 
	clang-format -i $(SOURCES) -i $(INCLUDES)

include $(RACK_DIR)/plugin.mk

build: format dist

run: format install
	/Applications/VCV\ Rack\ 2\ Free.app/Contents/MacOS/Rack

headless: format install
	/Applications/VCV\ Rack\ 2\ Free.app/Contents/MacOS/Rack --headless

debug: build
	mkdir -p workdir/simd/plugins-mac-arm64
	cp -r dist/*.vcvplugin ./workdir/simd/plugins-mac-arm64
	$(BIN) --user ~/Development/sn/sn-vcv/workdir/simd

simd: build
	mkdir -p workdir/simd/plugins-mac-arm64
	cp -r dist/*.vcvplugin ./workdir/simd/plugins-mac-arm64
	$(BIN) --user ~/Development/sn/sn-vcv/workdir/simd

tail:
	tail -f workdir/simd/log.txt | grep sn-vcv

