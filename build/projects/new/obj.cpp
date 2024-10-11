#include "obj.hpp"
#include <iostream>

#include <cmath>
#include <transform.hpp>

MyClass::MyClass() {
    // Конструктор
}

MyClass::~MyClass() {

} 

double ticks = 0;
void MyClass::Update() {
    Engine::Transform* transform = parent->GetComponent<Engine::Transform>();
    if (transform != nullptr) {
        std::cout << transform->GetPosition().x << std::endl;
        transform->GetPosition().x = sin(ticks) * 2;
    } else {
        std::cout << "err" << std::endl;
    }
    ticks += 0.1;
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
