#pragma once

#include <glm/vec3.hpp>
#include <GL/glew.h>
#include <vector>

namespace Renderer {
    class Cube {
        public:
            Cube();
            Cube(glm::vec3 pos);
            Cube(glm::vec3 pos, GLuint texture);

            bool IsActive();

            glm::vec3 angle = glm::vec3(0.0, 0.0, 0.0);
            glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);

            float scale = 1.0f;
            GLuint texture;
        private:
    };
}
