CXX = g++
FLAGS = -Iinclude -L. -lGL -lIL -lGLEW -llua -lglfw -lengine -Wl,-rpath=.
SOFLAGS = $(FLAGS) -fPIC
CXXFLAGS = $(FLAGS)

SRCS = $(wildcard *.cpp)
SOFILES = $(SRCS:.cpp=.so)
TARGET = program
all: $(SOFILES)
execute: $(TARGET)
	gdb -ex "start" -ex "c" ./$(TARGET)

%.so: %.cpp
	$(CXX) -shared -o $@ $^ $(SOFLAGS)

$(TARGET): compile_program.cpp
	$(CXX) compile_program.cpp -o $(TARGET) $(CXXFLAGS)

clean:
	rm -f $(SOFILES)
