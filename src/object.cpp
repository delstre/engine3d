#include <object.hpp>
#include <model.hpp>
#include <transform.hpp>
#include <componentmanager.hpp>
#include <archive.hpp>

#include <iostream>
#include <fstream>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <IL/il.h>

using namespace Engine;

Object::Object() {
    Init();
}

Object::Object(const std::string& name) : Object() {
    this->name = name;
};

Component* Object::AddComponent(const std::string& name) {
    Engine::Component* component = static_cast<Engine::Component*>(Engine::ComponentManager::CreateComponent(name));
    if (component) {
        component->Start();
        component->SetParent(this);
        components.push_back(component);

        return component;
    } else {
        std::cerr << "Failed to create component: " << name << std::endl;
        return nullptr;
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
        if (component == nullptr) {
            continue;
        }

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
        if (!component || component == nullptr)
            continue;

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
        if (component == nullptr) {
            continue;
        }

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

    OArchive oa(obj_file);
    oa << *this;
    obj_file.close();
}

void Object::LoadFromPrefab(const std::string& path) {
    std::ifstream obj_file(path, std::ios::binary);
    if (!obj_file) {
        std::cerr << "Failed to load object: " << path + "/myobject.bin" << std::endl;
        return;
    }

    IArchive ia(obj_file);
    ia >> *this;
    obj_file.close();
}

bool Object::IsHitByRay(const glm::vec3& origin, const glm::vec3& dir) {
    Engine::Model* model = GetComponent<Engine::Model>();
    if (model) {
        float tMin = 0.0f;
        float tMax = std::numeric_limits<float>::max();

        glm::vec3 mins = model->GetMins();
        glm::vec3 maxs = model->GetMaxs();

        for (int i = 0; i < 3; ++i) {
            if (dir[i] != 0.0f) {
                float t1 = (mins[i] - origin[i]) / dir[i];
                float t2 = (maxs[i] - origin[i]) / dir[i];

                if (t1 > t2) std::swap(t1, t2);

                tMin = std::max(tMin, t1);
                tMax = std::min(tMax, t2);

                if (tMin > tMax)
                    return false;
            } else {
                if (origin[i] < mins[i] || origin[i] > maxs[i])
                    return false;
            }
        }

        return true;
    }

    return false;
}

void Object::SetENV(const Renderer::Envy& env) {
    this->env = env;
}

void Object::Update() {
    for (Engine::Component* pComponent: components) {
        if (!pComponent) {
            continue;
        }

        pComponent->UpdateComponent();
    }

    if (UpdateOverride) {
        UpdateOverride();
    }
}

template Engine::Transform* Object::GetComponent<Engine::Transform>();
template Engine::Model* Object::GetComponent<Engine::Model>();
template Renderer::Mesh* Object::GetComponent<Renderer::Mesh>();
