#pragma once

#include <vector>
#include "cube.hpp"
#include "shaderprogram.hpp"
#include "camera.hpp"

namespace Renderer {
    class Camera;

    class ModelManager {
        public:
            ModelManager(ShaderProgram* pShaderProgram, Camera* pCamera);

            void AddModel();
            void AddModel(Cube mdl);
            void RemoveModel(int index);

            void Render();
            std::vector<Cube> vecModels;

            glm::vec4* GetPlanes();
            bool isCubeInFrustum(const glm::vec3& min, const glm::vec3& max);

            void UpdateArrayBuffer();
            Camera* pCamera;

            bool isWireFrame = false;
            std::vector<GLfloat> points;
            GLuint vao, vbo, ibo;
        private:
            ShaderProgram* pShaderProgram; 

            std::vector<GLfloat> normals;
            std::vector<GLuint> elements;
            std::vector<GLfloat> texture_points;

            GLuint c_vbo, t_vbo, n_vbo;
    };
}
