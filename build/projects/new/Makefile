CXX = g++
CXXFLAGS = -Iinclude -L. -lGL -lIL -lGLEW -lglfw -lengine -lboost_serialization -Wl,-rpath=. -fPIC -rdynamic
SRCS = $(wildcard *.cpp)
SOFILES = $(SRCS:.cpp=.so)
all: $(SOFILES)
%.so: %.cpp
	$(CXX) -shared -o $@ $^ $(CXXFLAGS)
clean:
	rm -f $(SOFILES)
