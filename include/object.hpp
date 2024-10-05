#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include "model.hpp"
#include "modelinstance.hpp"

namespace Renderer {
    class Object {
        public:
            Object();
            Object(glm::vec3 position);
            Object(Model* model, glm::vec3 position);
            Object(ModelInstance* model, glm::vec3 position);

            glm::vec3 angle = glm::vec3(0.0, 0.0, 0.0);
            glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
            glm::mat4 matmodel = glm::translate(glm::mat4(1.0f), position);

            glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);

            bool IsActive = true;

            void UpdatePosition();

            void SetModel(Model* model);
            Model* GetModel();

            void SetModelInstance(ModelInstance* model);
            ModelInstance* GetModelInstance();

            void SetPosition(glm::vec3 position);
            void SetPosition(int x, int y, int z);

            glm::vec3& GetPosition();
            glm::mat4& GetMatrix();

            void SetColor(glm::vec3 color);
            glm::vec3& GetColor();

            void SetTexture(uint texture);
            GLuint GetTexture();

            void SetTextureInstance(int id, uint texture);
            GLuint GetTextureInstance(int id);

            bool ModelIsInstanced();

            glm::vec3 GetMinBounds();
            glm::vec3 GetMaxBounds();
    
            void Render(const Envy& envy, const std::vector<GLuint>& textures);
        private:
            Model* pModel = nullptr;
            ModelInstance* pModelInstance = nullptr;
            GLuint texture = 0;

    };

    ModelInstance* TranslateModelsToInstance(std::vector<Renderer::Object*>& objects, int start, int end);
}
