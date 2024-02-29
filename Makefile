.DEFAULT_GOAL = build

RACK_DIR ?= ../..
EXE = "/Applications/VCV Rack 2 Free.app/Contents/MacOS/Rack"

FLAGS +=
CFLAGS +=
CXXFLAGS +=

LDFLAGS +=

SOURCES += $(wildcard src/*.cpp src/filters/*.cpp)
INCLUDES += $(wildcard src/*.hpp src/filters/*.hpp)

DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)
DISTRIBUTABLES += $(wildcard presets)

format: 
	clang-format -i $(SOURCES) -i $(INCLUDES) -i src/filters/*.hpp

include $(RACK_DIR)/plugin.mk

build: format dist

run: format install
	/Applications/VCV\ Rack\ 2\ Free.app/Contents/MacOS/Rack

headless: format install
	/Applications/VCV\ Rack\ 2\ Free.app/Contents/MacOS/Rack --headless

debug: build
	mkdir -p runtime/plugins-mac-x64
	cp -r dist/*.vcvplugin ./runtime/plugins-mac-x64
	$(EXE) --user ~/Development/shapestry/sn-vcv/runtime
