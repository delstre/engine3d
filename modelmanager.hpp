#include "model.hpp"
#include "modelinstance.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>

namespace Renderer {
    class ModelManager {
        public:
            ModelManager() = default;
            ~ModelManager() = default;

            Model* Cube();
            ModelInstance* CubeInstaced();

            void AddModel(std::string name, Model* model);
            Model* GetModel(std::string name) const;

            void ImportModel(std::string path);

        private:
            Model* cubeInstance = nullptr; 
            ModelInstance* cubeInstance_Instance = nullptr; 

            std::map<std::string, Model*> models;

            Assimp::Importer importer;
    };
}
