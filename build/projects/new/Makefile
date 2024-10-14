# Компилятор и флаги
CXX = g++

ENGINE_DIR = ../../../

CXXFLAGS = -Wall -fPIC -I$(ENGINE_DIR)include

# Цели
TARGET = obj.so

# Исходные файлы
SRCS = obj.cpp $(wildcard $(ENGINE_DIR)src/*.cpp)

# Объектные файлы
OBJS = $(SRCS:.cpp=.o)

# Правило по умолчанию
all: $(TARGET)

# Создание динамической библиотеки
$(TARGET): $(OBJS)
	$(CXX) -shared -o $@ $^

# Компиляция .cpp в .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка сгенерированных файлов
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
