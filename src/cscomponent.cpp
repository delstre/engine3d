#include <cscomponent.hpp>

#include <dlfcn.h>
#include <iostream>

using namespace Engine;

void CSComponent::SetHandler(void* pHandler, void* pComponent) {
    this->pHandle = pHandler;
    this->pComponent = pComponent;

    UpdateFunc = (func_t) dlsym(pHandler, "Update");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol create: " << dlsym_error << std::endl;
        dlclose(pHandler);
    }

    SetParentFunc = (setparent_t) dlsym(pHandler, "SetParent");
    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol create: " << dlsym_error << std::endl;
        dlclose(pHandler);
    }
}

void CSComponent::SetParent(Renderer::Object* obj) {
    if (SetParentFunc) {
        SetParentFunc(pComponent, obj);
    }
}

void CSComponent::Update() {
    if (UpdateFunc) {
        UpdateFunc(pComponent);
    }
}

void CSComponent::InterfaceUpdate() {

}

std::string CSComponent::GetTypeName() const {
    return "Comp";
}
