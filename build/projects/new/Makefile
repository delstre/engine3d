# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -fPIC -I../../../include -I../../../lib/imgui

# Цели
TARGET = obj.so

# Исходные файлы
SRCS = obj.cpp ../../../src/component.cpp 

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
