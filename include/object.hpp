#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include <string>
#include <vector>

#include <structs.hpp>

#include <component.hpp>
#include <componentmanager.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

namespace Engine {
    class IComponent;
    class Component;
}

namespace Renderer {
    class Object {
        public:
            Object() {};
            Object(const std::string& name);

            void AddComponent(const std::string& name);
            void AddComponent(Engine::Component& component);
            void RemoveComponent(const std::string& name);
            void RemoveComponent(Engine::Component* component);

            template <typename T>
            T* GetComponent();

            template <typename T>
            T* GetComponent(const std::string& name);

            std::vector<Engine::Component*>& GetComponents();

            void SetENV(const Envy& env);

            void SaveAsPrefab(const std::string& path);
            void LoadFromPrefab(const std::string& path);

            void Update();

            std::string name;
            Envy env;
        private:
            std::vector<Engine::Component*> components;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive& ar, const unsigned int version) {
                ar & name;
                ar & components;
            }
    };

    //ModelInstance* TranslateModelsToInstance(std::vector<Renderer::Object*>& objects, int start, int end);
}
