#pragma once

#include <component.hpp>

class MyClass : protected Engine::Component {
    public:
        MyClass();
        ~MyClass();
        void Update() override;
        void InterfaceUpdate() override;
};

extern "C" MyClass* create();
extern "C" void destroy(MyClass*);
