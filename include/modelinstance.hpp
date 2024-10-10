#pragma once

#include <model.hpp>

namespace Renderer {
    class ModelInstanceRender : protected ModelRender {
        public:
            ModelInstanceRender(std::vector<Vertex> vertexs, std::vector<GLuint> faces);
            //ModelInstanceRender(ModelRender* model, std::vector<glm::mat4> matrixes, std::vector<uint> textures, std::vector<glm::vec3> colors);
            ModelInstanceRender(const ModelRender& model);

            ModelInstanceRender(const ModelInstanceRender& other) = default;
            ModelInstanceRender(ModelInstanceRender&& other) = default;
            ModelInstanceRender& operator=(const ModelInstanceRender& other) = default;
            ModelInstanceRender& operator=(ModelInstanceRender&& other) = default;
            ~ModelInstanceRender();

            int active_id = -1;
            int call_id = -1;

            void AddModel(const glm::mat4& matrix, const uint& texture);

            std::vector<glm::mat4>& GetMatrixes();
            std::vector<uint>& GetTextures();

            void UpdatePositions(const std::vector<glm::mat4>& matrixes);
            void UpdatePositions();
            void UpdateTextures(const std::vector<uint>& textures);
            void UpdateTextures();
            void UpdateColors(const std::vector<glm::vec3>& colors);
            void UpdateColors();

            void UpdateColor(int id, const glm::vec3& color);

            void Render(const Envy& env, const std::vector<GLuint>& texture);
        private:
            GLuint p_vbo, pt_vbo, pc_vbo;

            std::vector<glm::mat4> matrixes;
            std::vector<uint> textures;
            std::vector<glm::vec3> colors;
    };
}
