#include "model.hpp"

#include <map>

namespace Renderer {
    class ModelManager {
        public:
            ModelManager() = default;
            ~ModelManager() = default;

            void AddModel(std::string name, Model* model);
            Model* GetModel(std::string name) const;

        private:
            std::map<std::string, Model*> models;
    };
}
