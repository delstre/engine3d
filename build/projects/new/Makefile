CXX = g++
CXXFLAGS = -Iinclude -L. -lGL -lIL -lGLEW -lglfw -lengine -Wl,-rpath=. -fPIC
TARGET = components.so
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) -shared -o $@ $^ $(CXXFLAGS)
%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
clean:
	rm -f $(TARGET) $(OBJS)
