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

                // Убедимся, что t1 <= t2
                if (t1 > t2) std::swap(t1, t2);

                // Обновляем tMin и tMax
                tMin = std::max(tMin, t1);
                tMax = std::min(tMax, t2);

                // Если в какой-то момент tMin становится больше tMax, пересечения нет
                if (tMin > tMax)
                    return false;
            } else {
                // Если направление луча параллельно оси и начальная точка не в пределах этой оси
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

//void Object::serialize(std::ofstream& ofs) const {
    //for (auto& var : variables) {
        //var->serialize(ofs);
    //}

    //size_t vec_size = components.size();
    //ofs.write(reinterpret_cast<const char*>(&vec_size), sizeof(vec_size));

    //for (const auto& ptr : components) {
        //std::string type = ptr->GetTypeName();
        //size_t type_len = type.length();
        //ofs.write(reinterpret_cast<const char*>(&type_len), sizeof(type_len));
        //ofs.write(type.c_str(), type_len);

        //ptr->serialize(ofs);
    //}

//}

//void Object::deserialize(std::ifstream& ifs) {
    //for (auto& var : variables) {
        //var->deserialize(ifs);
    //}

    //size_t vec_size;
    //ifs.read(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));
    //components.resize(vec_size);

    //for (size_t i = 0; i < vec_size; ++i) {
        //size_t type_len;
        //ifs.read(reinterpret_cast<char*>(&type_len), sizeof(type_len));
        //std::string type(type_len, ' ');
        //ifs.read(&type[0], type_len);

        //std::cout << "Component: " << type << std::endl;
        //AddComponent(type)->deserialize(ifs);
    //}
//}


//Renderer::ModelInstance* Renderer::TranslateModelsToInstance(std::vector<Engine::Object*>& objects, int start, int end) {
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
