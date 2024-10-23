#pragma once

#include <component.hpp>

class MyClass : public Engine::Component {
    public:
        MyClass();
        ~MyClass();
        void Update();
        void Start();
        void End();

        float ticks = 0;

        DECLARE_CLASS_VARIABLE(int, a, 10)
        DECLARE_CLASS_VARIABLE(int, b, 10)
        DECLARE_CLASS_VARIABLE(int, c, 10)
        DECLARE_CLASS_VARIABLE(bool, d, false)

        DECLARE_CLASS_VARIABLES(
            REGISTER_CLASS_VARIABLE(int, a)
            REGISTER_CLASS_VARIABLE(int, b)
            REGISTER_CLASS_VARIABLE(int, c)
            REGISTER_CLASS_VARIABLE(bool, d)
        )
};


extern "C" {
    MyClass* Create();
    void Destroy(MyClass* obj);
    void Update(MyClass* obj);
    void Start(MyClass* obj);
    void End(MyClass* obj);
}
