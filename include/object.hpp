#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include <string>
#include <memory>
#include <vector>

#include <component.hpp>

namespace Engine {
    class Component;
    class ComponentManager;
}

namespace Renderer {
    struct Envy {
        glm::vec3 viewpos;
        glm::vec3 viewdir;
        glm::mat4 mvp;
    };

    class Object {
        public:
            Object(const std::string& name);

            void AddComponent(const std::string& name);
            void RemoveComponent(const std::string& name);

            template <typename T>
            T* GetComponent();

            std::vector<Engine::Component*>& GetComponents();

            void SetComponentManager(Engine::ComponentManager* manager);

            void SetENV(const Envy& env);
            void Update();

            std::string name;
            Envy env;
        private:
            Engine::ComponentManager* pComponentManager = nullptr;
            std::vector<Engine::Component*> components;



    };

    //ModelInstance* TranslateModelsToInstance(std::vector<Renderer::Object*>& objects, int start, int end);
}
