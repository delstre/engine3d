#pragma once

#include "model.hpp"
#include "modelinstance.hpp"
#include <map>

namespace Renderer {
    class ModelManager {
        public:
            ModelManager() = default;
            ~ModelManager() = default;

            void AddModel(std::string name, Model* model);
            Model* GetModel(std::string name) const;
            std::map<std::string, Model*> GetModels() const;
            std::string GetModelName(Model* model) const;

            bool ImportModel(const std::string& path);

        private:
            std::map<std::string, Model*> models;
    };
}
