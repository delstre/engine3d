#include "component.hpp"
#include <transform.hpp>

using namespace Engine;

void Component::SetParent(Renderer::Object* parent) { 
    if (this->parent == nullptr)
        this->parent = parent; 
}

template <typename T>
T* Component::GetComponent() { 
    return parent->template GetComponent<T>(); 
}

template Transform* Component::GetComponent<Transform>();

std::string Component::GetTypeName() {
    return typeid(*this).name();
}

