#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <component.hpp>
BOOST_CLASS_EXPORT_IMPLEMENT(Engine::Component)
#include <transform.hpp>

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

void Component::SetParent(Renderer::Object* parent) { 
    if (this->parent == nullptr)
        this->parent = parent; 
}

Renderer::Object* Component::GetParent() const {
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
    return typeid(*this).name();
}

template Transform* Component::GetComponent<Transform>();
