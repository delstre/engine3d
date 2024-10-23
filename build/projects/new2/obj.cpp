#include "obj.hpp"
#include <iostream>

#include <cmath>
#include <transform.hpp>

MyClass::MyClass() : Engine::Component() {
    Init();
}

MyClass::~MyClass() {

} 

void MyClass::Update() {
    auto transform = GetComponent<Engine::Transform>();
    if (transform == nullptr) {
        return;
    }

    transform->GetPosition() = glm::vec3(cos(ticks) * a, sin(ticks) * b, sin(ticks) * c);

    ticks += 0.05;
}

void MyClass::Start() {}
void MyClass::End() {}

extern "C" {
    MyClass* Create() {
        return new MyClass();
    }

    void Destroy(MyClass* obj) {
        delete obj;
    }

    void Update(MyClass* obj) {
        obj->Update();
    }

    void Start(MyClass* obj) {
        obj->Start();
    }

    void End(MyClass* obj) {
        obj->End();
    }
}
