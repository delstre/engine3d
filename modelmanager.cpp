#include "modelmanager.hpp"

using namespace Renderer;

Model* ModelManager::Cube() {
    if (cubeInstance != nullptr)
        return cubeInstance;

    Renderer::Model* mdl = new Renderer::Model(new Renderer::ShaderProgram("shaders/model.vert", "shaders/model.frag"), {
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f 
        }, {
            // Front face
            0, 1, 2,
            2, 3, 0,
            // Right face
            1, 5, 6,
            6, 2, 1,
            // Back face
            7, 6, 5,
            5, 4, 7,
            // Left face
            4, 0, 3,
            3, 7, 4,
            // Bottom face
            4, 5, 1,
            1, 0, 4,
            // Top face
            3, 2, 6,
            6, 7, 3
        }, {
            // Front face
            0.0f, 0.0f, 
            1.0f, 0.0f, 
            1.0f, 1.0f, 
            0.0f, 1.0f, 

            // Right face
            1.0f, 0.0f, 
            1.0f, 1.0f, 
            0.0f, 1.0f, 
            0.0f, 0.0f, 

            // Back face
            1.0f, 0.0f, 
            1.0f, 1.0f, 
            0.0f, 1.0f, 
            0.0f, 0.0f, 

            // Left face
            0.0f, 0.0f, 
            1.0f, 0.0f, 
            1.0f, 1.0f, 
            0.0f, 1.0f, 


            // Bottom face
            1.0f, 1.0f, // up left
            1.0f, 0.0f, // up right
            1.0f, 0.0f, // bottom right
            0.0f, 0.0f, // bottom left
            // Top face
            0.0f, 1.0f, 
            0.0f, 0.0f, 
            1.0f, 0.0f, 
            1.0f, 1.0f, 
        }
    );

    cubeInstance = mdl;
    return cubeInstance;
}

// maybe no
ModelInstance* ModelManager::CubeInstaced() {
    if (cubeInstance_Instance != nullptr)
        return cubeInstance_Instance;

    cubeInstance_Instance = new ModelInstance(Cube());
    return cubeInstance_Instance;
}

void ModelManager::AddModel(std::string name, Model* model) {
    models[name] = model;
}

Model* ModelManager::GetModel(std::string name) const {
    return models.at(name);
}
