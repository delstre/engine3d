#pragma once

#include <GL/glew.h>
#include <vector>

#include <structs.hpp>

#include <shaderprogram.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>


namespace Renderer {
    class Mesh {
        public:
            Mesh() = default;
            Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
            ~Mesh();

            void SetShader(ShaderProgram* shader);
            void SetRenderType(GLenum renderType);

            void UpdateVertices(std::vector<Vertex> vertices);
            void UpdateIndices(std::vector<GLuint> indices);

            void Render(Envy env, GLuint texture, glm::mat4 model);

            std::vector<Vertex> GetVertices();

            std::string name;
        private:
            GLuint vao;
            ShaderProgram* pShader = nullptr;

            GLuint vbo, ebo;
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;

            GLenum renderType = GL_TRIANGLES;

            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive& ar, const unsigned int version) {
                ar & name;
            }

    };
}
