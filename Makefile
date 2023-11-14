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

build: format install
