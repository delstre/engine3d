#pragma once

#include <component.hpp>

class MyClass : public Engine::Component {
    public:
        MyClass();
        void Update() override;
};

extern "C" MyClass* create();
extern "C" void destroy(MyClass*);
