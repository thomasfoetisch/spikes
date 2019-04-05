CXX = clang++
DEPS_BIN = clang++
CXXFLAGS = -O2 -std=c++11
LDFLAGS = -O2
LDLIB = -lpthread
AR = ar
ARFLAGS = rc
MKDIR = mkdir
MKDIRFLAGS = -p

PREFIX = ~/.local/
BIN_DIR = bin/
INCLUDE_DIR = include/
LIB_DIR = lib/


SOURCES = test/array.cpp test/image.cpp test/thread_pool.cpp

HEADERS = include/spikes/array.hpp include/spikes/thread_pool.hpp include/spikes/meta.hpp

BIN = bin/test_array bin/test_image bin/test_thread_pool

bin/test_array: build/test/array.o
bin/test_image: build/test/image.o
bin/test_thread_pool: build/test/thread_pool.o
#bin/...: ...

LIB = 

#lib/...: ...
