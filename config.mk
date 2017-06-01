CXX = g++
DEPS_BIN = g++
CXXFLAGS = -g -std=c++11
LDFLAGS = -g
AR = ar
ARFLAGS = rc

PREFIX = ~/.local/
BIN_DIR = bin/
INCLUDE_DIR = include/spikes/
LIB_DIR = lib/


SOURCES = test/array.cpp test/image.cpp

HEADERS = include/array.hpp

BIN = bin/test_array bin/test_image

bin/test_array: build/test/array.o
bin/test_image: build/test/image.o
#bin/...: ...

LIB = 

#lib/...: ...
