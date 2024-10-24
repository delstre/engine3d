CXX = g++
CXXFLAGS = -Iinclude -L. -lGL -lIL -lGLEW -lglfw -lengine -Wl,-rpath=. -fPIC
SRCS = $(wildcard *.cpp)
SOFILES = $(SRCS:.cpp=.so)
all: $(SOFILES)
%.so: %.cpp
	$(CXX) -shared -o $@ $^ $(CXXFLAGS)
clean:
	rm -f $(SOFILES)
