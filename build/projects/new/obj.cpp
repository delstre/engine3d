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
    std::cout << parent << std::endl;
    //Engine::Transform* transform = GetComponent<Engine::Transform>();
    //if (transform != nullptr) {
        //std::cout << transform->GetPosition().x << std::endl;
        //transform->GetPosition().x = sin(ticks) * 2;
    //} else {
        //std::cout << "err" << std::endl;
    //}
    //ticks += 0.1;
}

void MyClass::InterfaceUpdate() {

}

// Функции для создания и уничтожения объектов MyClass
extern "C" MyClass* Create() {
    return new MyClass();
}

extern "C" void Destroy(MyClass* object) {
    delete object;
}

extern "C" void Update(MyClass* object) {
    object->Update();
}

extern "C" void SetParent(MyClass* object, Renderer::Object* parent) {
    static_cast<Engine::Component*>(object)->SetParent(parent);
}
