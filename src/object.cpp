#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <IL/il.h>

#include <object.hpp>
#include <model.hpp>
#include <transform.hpp>
#include <iostream>
#include <fstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

using namespace Renderer;

Object::Object(const std::string& name) {
    this->name = name;
};

void Object::AddComponent(const std::string& name) {
    Engine::Component* component = static_cast<Engine::Component*>(Engine::ComponentManager::CreateComponent(name));
    if (component) {
        component->Start();
        component->SetParent(this);
        components.push_back(component);
    } else {
        std::cerr << "Failed to create component: " << name << std::endl;
    }
}

void Object::AddComponent(Engine::Component& component) {
    component.Start();
    component.SetParent(this);
    components.push_back(&component);
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

void Object::RemoveComponent(Engine::Component* component) {
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


void Object::SaveAsPrefab(const std::string& path) {
    std::ofstream obj_file(path + "/myobject.bin", std::ios::binary);
    if (!obj_file) {
        std::cerr << "Failed to load object: " << path + "/myobject.bin" << std::endl;
        return;
    }

    boost::archive::binary_oarchive oa(obj_file);
    oa.register_type<Engine::Transform>();
    oa.register_type<Engine::Model>();
    oa << *this;
    obj_file.close();
}

void Object::LoadFromPrefab(const std::string& path) {
    std::ifstream obj_file(path, std::ios::binary);
    if (!obj_file) {
        std::cerr << "Failed to load object: " << path + "/myobject.bin" << std::endl;
        return;
    }

    boost::archive::binary_iarchive ia(obj_file);
    ia.register_type<Engine::Transform>();
    ia.register_type<Engine::Model>();
    ia >> *this;
    obj_file.close();
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
template Engine::Model* Object::GetComponent<Engine::Model>();
