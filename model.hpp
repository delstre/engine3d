#pragma once

#include <vector>
#include <GL/glew.h>

#include "shaderprogram.hpp"

namespace Renderer {
    struct Envy {
        glm::vec3 viewpos;
        glm::vec3 viewdir;
        glm::mat4 mvp;
    };

    struct Vertex {
        glm::vec3 position;   // 3 floats for position
        glm::vec3 normal;     // 3 floats for normal
        glm::vec2 texCoord;   // 2 floats for texture coordinates
    };

    class Model {
        public:
            Model() {};
            Model(std::vector<Vertex> vertices, std::vector<GLuint> indices);
            Model(Model* other);

            Model(Model&& other) = default;
            Model& operator=(const Model& other) = default;
            Model& operator=(Model&& other) = default;
            ~Model();

            void UpdateVertices(std::vector<Vertex> points);
            void UpdateIndices(std::vector<GLuint> faces);

            std::vector<Vertex> GetVertices();

            void SetRenderType(GLenum renderType);
            void SetShader(ShaderProgram* shader);

            glm::vec3 GetMinBounds();
            glm::vec3 GetMaxBounds();

            void Render(const Envy& envy, const glm::mat4 position, const GLuint texture);
        protected:
            GLuint vao;
            ShaderProgram* pShader;

            GLuint vbo, ebo;
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;

            GLenum renderType = GL_TRIANGLES;

            bool hasNormals;
            bool hasTextureCoords;
    };
}
