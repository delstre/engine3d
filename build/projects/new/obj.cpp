#include "obj.hpp"
#include <iostream>

MyClass::MyClass() {
    // Конструктор
}

MyClass::~MyClass() {

} 

void MyClass::Update() {
}

void MyClass::InterfaceUpdate() {

}

// Функции для создания и уничтожения объектов MyClass
extern "C" MyClass* create() {
    return new MyClass();
}

extern "C" void destroy(MyClass* object) {
    delete object;
}
