#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <vector>

namespace Renderer {
    class Cube {
        public:
            Cube();
            Cube(glm::vec3 pos);
            Cube(glm::vec3 pos, GLuint texture);

            bool IsActive = true;

            glm::vec3 angle = glm::vec3(0.0, 0.0, 0.0);
            glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);

            glm::vec3 GetMin() const;
            glm::vec3 GetMax() const;

            glm::mat4 model = glm::mat4(1.0f);

            float scale = 1.0f;
            GLuint texture;
            GLuint vao;
        private:
    };
}
