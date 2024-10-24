#pragma once

#include <map>

#include <mesh.hpp>

namespace Engine {
    namespace ModelManager {
        void SetPath(std::string path);
        void AddModel(std::string name, Renderer::Mesh* model);
        Renderer::Mesh* GetModel(const std::string& name);
        std::map<std::string, Renderer::Mesh*> GetModels();

        bool ImportModel(const std::string& path);

        static std::map<std::string, Renderer::Mesh*> models;
        static std::string path;
    };
}
