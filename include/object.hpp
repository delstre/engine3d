#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include <component.hpp>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <string>

namespace Renderer {
    class Object {
        public:
            Object(const std::string& name);

            template<typename T, typename... Args>
            T* AddComponent(Args&&... args);

            template <typename T>
            void AddComponent(T* component);

            template <typename T>
            T* GetComponent();

            void Update();

            std::string name;
        private:
            std::unordered_map<std::type_index, std::unique_ptr<Engine::Component>> components; // Хранение компонентов

    };

    //ModelInstance* TranslateModelsToInstance(std::vector<Renderer::Object*>& objects, int start, int end);
}
