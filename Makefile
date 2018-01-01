SLUG    = stk
VERSION = 0.5.1

SOURCES = $(wildcard src/*.cpp)

DISTRIBUTABLES += $(wildcard LICENSE*) res

FLAGS += \
	-I./stk-fork/include \
	-I./stk-fork/src/include \
	-Wno-unused-local-typedefs

SOURCES += $(wildcard stk-fork/src/*.cpp)

include ../../plugin.mk

ifeq ($(ARCH),lin)
FLAGS += -D__OS_LINUX__
else ifeq ($(ARCH),mac)
FLAGS += -D__OS_MACOSX__
else ifeq ($(ARCH),win)
FLAGS += -D__OS_WINDOWS__
endif
