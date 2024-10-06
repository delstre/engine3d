#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <IL/il.h>

#include <object.hpp>
#include <modelinstance.hpp>

#include <stdexcept>

using namespace Renderer;

Object::Object(const std::string& name) {
    this->name = name;
};

template<typename T, typename... Args>
T* Object::AddComponent(Args&&... args) {
    static_assert(std::is_base_of<Engine::Component, T>::value, "T must be a Component");
    auto component = std::make_unique<T>(std::forward<Args>(args)...);
    T* ptr = component.get();
    components[typeid(T)] = std::move(component); // Добавляем в unordered_map
    return ptr; // Возвращаем указатель на добавленный компонент
}

template <typename T>
void Object::AddComponent(T* component) {
    static_assert(std::is_base_of<Engine::Component, T>::value, "T must be a Component");
    if (!component) {
        throw std::invalid_argument("Component pointer is null");
    }
    // Сохраняем компонент в unordered_map
    components[typeid(T)] = std::unique_ptr<Engine::Component>(component); // Уникальный указатель для управления памятью
}

template void Object::AddComponent<Engine::Component>(Engine::Component*);


template <typename T>
T* Object::GetComponent() {
    auto it = components.find(typeid(T));
    return it != components.end() ? static_cast<T*>(it->second.get()) : nullptr;
}

void Object::Update() {
    for (const auto& [key, component] : components) {
        // need for model and modelinstance mvp, textures arguments
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
