#pragma once

#include <glm/glm.hpp>

namespace Renderer {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    struct Envy {
        glm::vec3 viewpos;
        glm::vec3 viewdir;
        glm::mat4 mvp;
    };
}
