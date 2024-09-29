#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include "model.hpp"

namespace Renderer {
    enum Face { NONE, FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM };

    class Object {
        public:
            Object();
            Object(int x, int y, int z);
            Object(Model* model, int x, int y, int z);

            glm::vec3 angle = glm::vec3(0.0, 0.0, 0.0);
            glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);
            glm::mat4 matmodel = glm::translate(glm::mat4(1.0f), position);

            glm::vec4 color = glm::vec4(1.0, 1.0, 1.0, 1.0);

            bool IsActive = false;

            void UpdatePosition();

            void SetModel(Model* model);
            void SetModelColor(float r, float g, float b);
            void SetPosition(glm::vec3 position);
            void SetPosition(int x, int y, int z);
            void SetMVP(glm::mat4 mvp);
            void SetTexture(const GLuint& texture);
    
            void Render(const glm::mat4 mvp);

            void UpdateActiveFaces();
            void AddActiveFaces(Face face, Object* object);
            std::vector<Object*> activeFaces = {nullptr};

        private:
            Model* pModel;
            GLuint texture;


    };
}
