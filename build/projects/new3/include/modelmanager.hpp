#pragma once

#include "model.hpp"
#include <map>

namespace Renderer {
    class ModelManager {
        public:
            ModelManager() = default;
            ~ModelManager() = default;

            void SetPath(std::string path);
            void AddModel(std::string name, ModelRender* model);
            ModelRender* GetModel(const std::string& name) const;
            std::map<std::string, ModelRender*> GetModels() const;

            bool ImportModel(const std::string& path);

        private:
            std::map<std::string, ModelRender*> models;

            std::string path;
    };
}
