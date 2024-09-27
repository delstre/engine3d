#include <iostream>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <IL/il.h>

#include "cube.hpp"

using namespace Renderer;

Cube::Cube() {}

Cube::Cube(glm::vec3 pos) : Cube() {
    position = pos;
    model = glm::translate(glm::mat4(1.0f), position);
}

Cube::Cube(glm::vec3 pos, GLuint _texture) : Cube(pos) {
    texture = _texture;
}

glm::vec3 Cube::GetMin() const {
    glm::vec3 halfSize(scale * 1.0f);
    return position - halfSize;
}

glm::vec3 Cube::GetMax() const {
    glm::vec3 halfSize(scale * 1.0f);
    return position + halfSize;
}
