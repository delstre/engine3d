CXX = g++
CC = gcc
AR = ar 

DEINTERFACE_DEBUG = 1
CXXFLAGS = -fPIC -g -std=c++20 -Wall -Wformat
ifeq ($(MAKECMDGOALS), all)
	CXXFLAGS += -DINTERFACE_DEBUG
endif

CFLAGS = -fPIC -g -ggdb -O0 -Wall -Ilib/nfd/src/include

INCLUDES = -Iinclude -Ilib/imgui -Ilib/nfd/src/include -Ilib/refl-cpp/include
LIBS = `pkg-config --libs glfw3 gtk+-3.0 gdk-3.0` -lglfw -lGL -lGLU -lGLEW -lIL -lILU -lpthread

SRCS_CPP := $(wildcard src/*.cpp)
ifeq ($(MAKECMDGOALS), lib)
	SRCS_CPP := $(filter-out src/main.cpp src/interface.cpp,  $(SRCS_CPP))
else
	SRCS_CPP += $(wildcard lib/imgui/*.cpp) lib/imgui/backends/imgui_impl_glfw.cpp lib/imgui/backends/imgui_impl_opengl3.cpp
endif

SRCS_C = lib/nfd/src/nfd_common.c lib/nfd/src/nfd_gtk.c

OBJS_CPP = $(SRCS_CPP:.cpp=.o)
OBJS_C = $(SRCS_C:.c=.o)

TARGET = build/proj
TARGET_LIB = build/libengine.so

NFD_CFLAGS = `pkg-config --libs --cflags gtk+-3.0`

all: $(TARGET)

lib : clean $(TARGET_LIB)

$(TARGET) : $(OBJS_CPP) $(OBJS_C)
	$(CXX) $(OBJS_CPP) $(OBJS_C) -o $(TARGET) $(LIBS)

$(TARGET_LIB) : $(OBJS_CPP)
	@echo "Creating static library: $(TARGET_LIB)"
	$(CXX) -shared -o $@ $^

%.o : %.cpp
	@echo "Compiling C++ file: $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	@echo "Compiling C file: $<"
	$(CC) $(CFLAGS) $(NFD_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_C) $(OBJS_CPP)
