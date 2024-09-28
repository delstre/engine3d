#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include "model.hpp"

namespace Renderer {
    class Object {
        public:
            Object();
            Object(int x, int y, int z);
            Object(Model* model, int x, int y, int z);

            glm::vec3 angle = glm::vec3(0.0, 0.0, 0.0);
            glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
            glm::mat4 matmodel = glm::translate(glm::mat4(1.0f), position);

            bool IsActive = true;

            void UpdatePosition();

            void SetModel(Model* model);
            void SetPosition(glm::vec3 position);
            void SetPosition(int x, int y, int z);
            void SetMVP(glm::mat4 mvp);
            void SetTexture(const GLuint& texture);
    
            void Render(const glm::mat4 mvp);

        private:
            Model* pModel;
            GLuint texture;
    };
}
