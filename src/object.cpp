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
    Engine::Component* component = pComponentManager->CreateComponent(this, name);
    if (component) {
        components.push_back(component);
    } else {
        std::cerr << "Failed to create component: " << name << std::endl;
    }
}

void Object::RemoveComponent(const std::string& name) {
    for (auto it = components.begin(); it != components.end(); ++it) {
        if ((*it)->GetTypeName() == name) {
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
    for (const auto& component: components) {
        if (!component) {
            continue;
        }

        component->Update();
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
    void* GetTransform(void* obj) {
        return obj;
    }
}
