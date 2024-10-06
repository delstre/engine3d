#include "obj.hpp"
#include <iostream>

MyClass::MyClass() {
    // Конструктор
}

void MyClass::doSomething() {
    std::cout << "My render!" << std::endl;
    
}

// Функции для создания и уничтожения объектов MyClass
extern "C" MyClass* create() {
    return new MyClass();
}

extern "C" void destroy(MyClass* object) {
    delete object;
}
