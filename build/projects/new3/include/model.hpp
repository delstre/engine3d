#pragma once

#include <vector>
#include <GL/glew.h>

#include <shaderprogram.hpp>
#include <component.hpp>

namespace Renderer {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    class ModelRender : public Engine::Component {
        public:
            ModelRender() : Engine::Component() {};
            ModelRender(std::vector<Vertex> vertices, std::vector<GLuint> indices);
            ModelRender(const ModelRender& other) = default;
            ModelRender(ModelRender&& other) = default;
            ModelRender& operator=(const ModelRender& other);
            ModelRender& operator=(ModelRender&& other) = default;
            ~ModelRender();

            void UpdateVertices(std::vector<Vertex> points);
            void UpdateIndices(std::vector<GLuint> faces);

            std::vector<Vertex> GetVertices();

            void SetRenderType(GLenum renderType);
            void SetShader(ShaderProgram* shader);

            void SetModel(ModelRender* model);

            glm::vec3 GetMinBounds();
            glm::vec3 GetMaxBounds();

            void Update();
            void InterfaceUpdate();
            void Start();
            void End();
        protected:
            GLuint texture;

            GLuint vao;
            ShaderProgram* pShader = nullptr;

            GLuint vbo, ebo;
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;

            GLenum renderType = GL_TRIANGLES;

            bool hasNormals;
            bool hasTextureCoords;

        private:
            friend class boost::serialization::access;

            template<class Archive>
            void serialize(Archive& ar, const unsigned int version) {
                ar & boost::serialization::base_object<Component>(*this);
            }
    };
}
