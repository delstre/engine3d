#pragma once

#include <object.hpp>

class MyClass : public Renderer::Object {
    public:
        MyClass();
        void doSomething();
};

extern "C" MyClass* create();
extern "C" void destroy(MyClass*);
