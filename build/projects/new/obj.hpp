#pragma once

#include <component.hpp>

class MyClass : public Engine::Component {
    public:
        MyClass();
        ~MyClass();
        void Update();
        void Start();
        void End();

        double a = 10;
};
