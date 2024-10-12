#pragma once

#include <component.hpp>

class MyClass : public Engine::Component {
    public:
        MyClass();
        ~MyClass();
        void Update() override;
        void InterfaceUpdate() override;
};

extern "C" MyClass* Create();
extern "C" void Destroy(MyClass*);
extern "C" void Update(MyClass*);
extern "C" void SetParent(MyClass* object, Renderer::Object* parent);
