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
    ///
    ////Engine::Transform* transform = GetComponent<Engine::Transform>();
    ////if (transform != nullptr) {
        ////std::cout << transform->GetPosition().x << std::endl;
        ////transform->GetPosition().x = sin(ticks) * 2;
    ////} else {
        ////std::cout << "err" << std::endl;
    ////}
    
    if (ticks > 10) {
        std::cout << a << std::endl;
        ticks = 0;
    }

    ticks += 0.1;
}
void MyClass::Start() {}
void MyClass::End() {}

REGCLASS(MyClass)
