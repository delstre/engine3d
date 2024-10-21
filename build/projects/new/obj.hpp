#pragma once

#include <component.hpp>

class MyClass : public Engine::Component {
    public:
        MyClass();
        ~MyClass();
        void Update();
        void Start();
        void End();

        DECLARE_CLASS_VARIABLE(int, a, 10)

        DECLARE_CLASS_VARIABLES(
            REGISTER_CLASS_VARIABLE(int, a)
        )
};


extern "C" {
    MyClass* Create();
    void Destroy(MyClass* obj);
    void Update(MyClass* obj);
    void Start(MyClass* obj);
    void End(MyClass* obj);
}
