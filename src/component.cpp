#include <component.hpp>
BOOST_CLASS_EXPORT_IMPLEMENT(Engine::Component)
#include <transform.hpp>
#include <cxxabi.h>
#include <serialize.hpp>

using namespace Engine;

void Component::UpdateComponent() {
    if (isEnabled) {
        Update();
    }
}

void Component::Start() {
    if (isEnabled) {
        Start();
    }
}

void Component::End() {
    if (isEnabled) {
        End();
    }
}

void Component::Update() {}
void Component::UpdateInterface() {}

void Component::SetParent(Engine::Object* parent) { 
    if (this->parent == nullptr)
        this->parent = parent; 
}

Engine::Object* Component::GetParent() const {
    return parent;
}

void Component::SetEnable(bool enable) {
    isEnabled = enable;
}

template <typename T>
T* Component::GetComponent() { 
    if (parent == nullptr) 
        return nullptr;

    return parent->template GetComponent<T>(); 
}

std::string Component::GetTypeName() const {
    char* demangledName = abi::__cxa_demangle(typeid(*this).name(), nullptr, nullptr, nullptr);
    return std::string(demangledName);
}


template Transform* Component::GetComponent<Transform>();
