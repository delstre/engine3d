#pragma once

#include <vector>
#include <GL/glew.h>

#include "shaderprogram.hpp"

namespace Renderer {
    class Model {
        public:
            Model();
            Model(ShaderProgram* shader, std::vector<GLfloat> points, std::vector<GLuint> faces, std::vector<GLfloat> texture_points);
            Model(std::vector<GLfloat> points, std::vector<GLuint> faces, std::vector<GLfloat> texture_points);

            Model(const Model& other) = default;
            Model(Model&& other) = default;
            Model& operator=(const Model& other) = default;
            Model& operator=(Model&& other) = default;
            ~Model() = default;

            GLuint vao, vbo, ibo;
            GLuint t_vbo;
            std::vector<GLfloat> points;
            std::vector<GLuint> faces;
            std::vector<GLfloat> texture_points;

            ShaderProgram* pShader;

            void Render(const glm::mat4 mvp, const glm::mat4 position);
    };
}
