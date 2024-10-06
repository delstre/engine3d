# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -fPIC -I../../../include # Убедитесь, что путь к заголовкам правильный

# Цели
TARGET = obj.so

# Исходные файлы
SRCS = obj.cpp ../../../src/object.cpp # Укажите правильный путь к object.cpp

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
