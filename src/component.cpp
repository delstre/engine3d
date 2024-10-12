#include "component.hpp"
#include <transform.hpp>

using namespace Engine;

void Component::SetParent(Renderer::Object* parent) { 
    if (this->parent == nullptr)
        this->parent = parent; 
}

template <typename T>
T* Component::GetComponent() { 
    if (parent == nullptr) 
        return nullptr;

    return parent->template GetComponent<T>(); 
}

template Transform* Component::GetComponent<Transform>();

std::string Component::GetTypeName() const {
    return typeid(*this).name();
}

class ConcreteComponent : public Component {
    void Update() override {}
    void InterfaceUpdate() override {}
    void SetParent(Renderer::Object* parent) override {}
};



extern "C" {
    Component* CreateComponent() {
        return new ConcreteComponent();
    }

    void DestroyComponent(Component* component) {
        delete component;
    }

    void SetParentComponent(Component* component, void* parent) {
        component->SetParent(static_cast<Renderer::Object*>(parent));
    }
}
