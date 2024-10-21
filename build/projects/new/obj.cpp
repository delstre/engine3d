#include "obj.hpp"
#include <iostream>

#include <cmath>

MyClass::MyClass() : Engine::Component() {
    Init();
}

MyClass::~MyClass() {

} 

double ticks = 0;
void MyClass::Update() {
    if (ticks > 10) {
        std::cout << a << std::endl;
        ticks = 0;
    }

    ticks += 0.1;
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
