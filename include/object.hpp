#pragma once

#include <structs.hpp>

#include <string>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include <reflection.hpp>

namespace Engine {
    class IComponent;
    class Component;
}

namespace Engine {
    class IComponent;
    class Component;

    class Object {
        public:
            Object();
            Object(const std::string& name);

            Component* AddComponent(const std::string& name);
            void AddComponent(Engine::Component& component);
            void RemoveComponent(const std::string& name);
            void RemoveComponent(Engine::Component* component);

            template <typename T>
            T* GetComponent();

            template <typename T>
            T* GetComponent(const std::string& name);
            std::vector<Engine::Component*>& GetComponents();
            
            bool IsHitByRay(const glm::vec3& origin, const glm::vec3& direction);

            void SetENV(const Renderer::Envy& env);

            void SaveAsPrefab(const std::string& path);
            void LoadFromPrefab(const std::string& path);

            void Update();

            Renderer::Envy env;
            std::vector<Engine::Component*> components;

            DECLARE_CLASS_VARIABLE(std::string, name, "MyObject");

            DECLARE_VARIABLES_VECTOR()

            DECLARE_CLASS_VARIABLES(
                REGISTER_CLASS_VARIABLE(std::string, name);
            )
    };

    //ModelInstance* TranslateModelsToInstance(std::vector<Engine::Object*>& objects, int start, int end);
}
