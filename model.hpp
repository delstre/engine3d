#pragma once

#include <vector>
#include <GL/glew.h>

#include "shaderprogram.hpp"

namespace Renderer {
    class Model {
        public:
            Model() {};
            Model(ShaderProgram* shader, std::vector<GLfloat> points, std::vector<GLuint> faces, std::vector<GLfloat> texture_points);
            Model(Model* other);

            Model(Model&& other) = default;
            Model& operator=(const Model& other) = default;
            Model& operator=(Model&& other) = default;
            ~Model();

            void UpdateVertices(std::vector<GLfloat> points);
            void UpdateIndices(std::vector<GLuint> faces);
            void UpdateTexturePoints(std::vector<GLfloat> texture_points);
            void UpdateColors(glm::vec3 color);

            glm::vec3 GetMinBounds();
            glm::vec3 GetMaxBounds();

            void Render(const glm::mat4 mvp, const glm::mat4 position, const GLuint texture);
        protected:
            GLuint vao;
            ShaderProgram* pShader;

            GLuint vbo, ibo;
            GLuint c_vbo, t_vbo;
            std::vector<GLfloat> points;
            std::vector<GLfloat> colors;
            std::vector<GLuint> faces;
            std::vector<GLfloat> texture_points;
    };
}
