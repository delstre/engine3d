#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <IL/il.h>

#include <object.hpp>
#include <modelinstance.hpp>
#include <transform.hpp>
#include <iostream>
#include <memory>

using namespace Renderer;

Object::Object(const std::string& name) {
    this->name = name;
};

void Object::AddComponent(const std::string& name) {
    Engine::Component* component = static_cast<Engine::Component*>(pComponentManager->CreateComponent(name));
    if (component) {
        component->Start();
        component->SetParent(this);
        components.push_back(component);
    } else {
        std::cerr << "Failed to create component: " << name << std::endl;
    }
}

void Object::RemoveComponent(const std::string& name) {
    for (auto it = components.begin(); it != components.end(); ++it) {
        Engine::Component* component = dynamic_cast<Engine::Component*>(*it);
        if (component->GetTypeName() == name) {
            component->End();
            components.erase(it);
            break;
        }
    }
}

template <typename T>
T* Object::GetComponent() {
    for (const auto& component: components) {
        T* result = dynamic_cast<T*>(component);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

template <typename T>
T* Object::GetComponent(const std::string& name) {
    for (Engine::Component* component: components) {
        if (component->GetTypeName() == name) {
            T* result = dynamic_cast<T*>(component);
            if (result) {
                return result;
            }
        }
    }
    return nullptr;
}

std::vector<Engine::Component*>& Object::GetComponents() {
    return components;
}

void Object::SetComponentManager(Engine::ComponentManager* manager) {
    pComponentManager = manager;
}

void Object::SetENV(const Envy& env) {
    this->env = env;
}

void Object::Update() {
    for (Engine::Component* pComponent: components) {
        if (!pComponent) {
            continue;
        }

        pComponent->UpdateComponent();
    }
}

//Renderer::ModelInstance* Renderer::TranslateModelsToInstance(std::vector<Renderer::Object*>& objects, int start, int end) {
    //int size = objects.size();
    //std::vector<glm::mat4> matrixes;
    //std::vector<uint> textures;
    //std::vector<glm::vec3> colors;

    //for (int i = start; i < (end < size ? end : size); i++) {
        //matrixes.push_back(objects[i]->GetMatrix());
        //textures.push_back(objects[i]->GetTexture());
        //colors.push_back(objects[i]->GetColor());
    //}

    //return new Renderer::ModelInstance(objects[start]->GetModel(), matrixes, textures, colors);
//}

template Engine::Transform* Object::GetComponent<Engine::Transform>();
template Renderer::ModelRender* Object::GetComponent<Renderer::ModelRender>();

extern "C" {
    //void* GetTransform(void* obj) {
        //return obj->GetComponent<Engine::Transform>();
    //}

    //void* GetPosition(void* transform) {
        //return static_cast<Engine::Transform*>(transform)->GetPositionPtr();
    //}
}
