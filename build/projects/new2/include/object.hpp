#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include <string>
#include <memory>
#include <vector>

#include <component.hpp>
#include <componentmanager.hpp>

namespace Engine {
    class IComponent;
    class Component;
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
            void RemoveComponent(Engine::Component* component);

            template <typename T>
            T* GetComponent();

            template <typename T>
            T* GetComponent(const std::string& name);

            std::vector<Engine::Component*>& GetComponents();

            void SetENV(const Envy& env);

            void Update();

            std::string name;
            Envy env;
        private:
            std::vector<Engine::Component*> components;



    };

    //ModelInstance* TranslateModelsToInstance(std::vector<Renderer::Object*>& objects, int start, int end);
}
