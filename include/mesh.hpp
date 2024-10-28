#pragma once

#include <structs.hpp>

#include <GL/glew.h>

#include <vector>

#include <reflection.hpp>

namespace Renderer {
    class ShaderProgram;

    class Mesh {
        public:
            Mesh();
            Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
            ~Mesh();

            void SetShader(ShaderProgram* shader);
            void SetRenderType(GLenum renderType);

            void UpdateVertices(std::vector<Vertex> vertices);
            void UpdateIndices(std::vector<GLuint> indices);

            void Render(Envy env, GLuint texture, glm::mat4 model);

            std::vector<Vertex> GetVertices();

            glm::vec3 GetMins();
            glm::vec3 GetMaxs();

            void SetColor(glm::vec3 color);
            glm::vec3 GetColor();

            DECLARE_CLASS_VARIABLE(std::string, name, "")

            DECLARE_VARIABLES_VECTOR()

            DECLARE_CLASS_VARIABLES(
                REGISTER_CLASS_VARIABLE(std::string, name);
            )
        private:
            GLuint vao;
            ShaderProgram* pShader = nullptr;

            glm::vec3 color;

            GLuint vbo, ebo;
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;

            GLenum renderType = GL_TRIANGLES;
    };
}
