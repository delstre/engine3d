#pragma once

#include <vector>
#include "cube.hpp"
#include "shaderprogram.hpp"
#include "camera.hpp"

namespace Renderer {
    class ModelManager {
        public:
            ModelManager(ShaderProgram* pShaderProgram, Camera* pCamera);

            void AddModel();
            void AddModel(Cube mdl);
            void RemoveModel(int index);

            void Render();
            std::vector<Cube> vecModels;

            Camera* pCamera;

            bool isWireFrame = false;
        private:
            ShaderProgram* pShaderProgram; 

            std::vector<GLfloat> points;
            std::vector<GLfloat> colors;
            std::vector<GLuint> elements;
            std::vector<GLfloat> texture_points;

            GLuint vao, vbo, ibo;
            GLuint c_vbo, t_vbo;
    };
}
