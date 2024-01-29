.DEFAULT_GOAL = build

RACK_DIR ?= ../..

FLAGS +=
CFLAGS +=
CXXFLAGS +=

LDFLAGS +=

SOURCES += $(wildcard src/*.cpp)
INCLUDES += $(wildcard src/*.hpp)

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

debug: format dist
	mkdir -p runtime/plugins-mac-x64
	cp -r dist/*.vcvplugin ./runtime/plugins-mac-x64
#	cd runtime && /Applications/VCV\ Rack\ 2\ Free.app/Contents/MacOS/Rack --dev
	/Applications/VCV\ Rack\ 2\ Free.app/Contents/MacOS/Rack --user ~/Development/shapestry/sn-vcv/runtime

