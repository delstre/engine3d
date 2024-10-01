#pragma once

#include "model.hpp"

namespace Renderer {
    class ModelInstance : public Model {
        public:
            ModelInstance(ShaderProgram* shader, std::vector<GLfloat> points, std::vector<GLuint> faces, std::vector<GLfloat> texture_points);

            ModelInstance(const ModelInstance& other) = default;
            ModelInstance(ModelInstance&& other) = default;
            ModelInstance& operator=(const ModelInstance& other) = default;
            ModelInstance& operator=(ModelInstance&& other) = default;
            ~ModelInstance();

            GLuint p_vbo, pt_vbo;
            std::vector<glm::mat4> positions;
            std::vector<uint> textures;

            int active_id;
            int call_id;

            void UpdatePositions(std::vector<glm::mat4*> positions);
            void UpdateTextures(std::vector<glm::uint> textures);

            void Render(const glm::mat4 mvp, const std::vector<GLuint>& texture);
    };
}
