.DEFAULT_GOAL = build

RACK_DIR ?= ../..
EXE = "/Applications/VCV Rack 2 Free.app/Contents/MacOS/Rack"

FLAGS +=
CFLAGS +=
CXXFLAGS +=

LDFLAGS +=

SOURCES  += $(wildcard src/*.cpp src/antialias/*.cpp src/dc-blocking/*.cpp src/dsp/*.cpp src/widgets/*.cpp)
INCLUDES += $(wildcard src/*.hpp src/antialias/*.hpp src/dc-blocking/*.hpp src/dsp/*.hpp src/widgets/*.hpp)

DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)
DISTRIBUTABLES += $(wildcard presets)

format: 
	clang-format -i $(SOURCES) -i $(INCLUDES)

include $(RACK_DIR)/plugin.mk

build: format dist

run: format install
	/Applications/VCV\ Rack\ 2\ Free.app/Contents/MacOS/Rack

headless: format install
	/Applications/VCV\ Rack\ 2\ Free.app/Contents/MacOS/Rack --headless

debug: build
	mkdir -p workdir/plugins-mac-x64
	cp -r dist/*.vcvplugin ./workdir/plugins-mac-x64
	$(EXE) --user ~/Development/shapestry/sn-vcv/workdir
