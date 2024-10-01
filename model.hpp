#pragma once

#include <vector>
#include <GL/glew.h>

#include "shaderprogram.hpp"

namespace Renderer {
    class Model {
        public:
            Model() {};
            Model(ShaderProgram* shader, std::vector<GLfloat> points, std::vector<GLuint> faces, std::vector<GLfloat> texture_points);

            Model(const Model& other) = default;
            Model(Model&& other) = default;
            Model& operator=(const Model& other) = default;
            Model& operator=(Model&& other) = default;
            ~Model();

            GLuint vao, vbo, ibo;
            GLuint c_vbo, t_vbo;
            std::vector<GLfloat> points;
            std::vector<GLfloat> colors;
            std::vector<glm::mat4> positions;
            std::vector<GLuint> faces;
            std::vector<GLfloat> texture_points;

            ShaderProgram* pShader;

            void UpdateVertices(std::vector<GLfloat> points);
            void UpdateIndices(std::vector<GLuint> faces);
            void UpdateTexturePoints(std::vector<GLfloat> texture_points);
            void UpdateColors(glm::vec3 color);

            glm::vec3 GetMins();
            glm::vec3 GetMaxs();

            void Render(const glm::mat4 mvp, const glm::mat4 position, const GLuint texture);
    };
}
