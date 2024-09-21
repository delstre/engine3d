#include <iostream>

#include <glm/vec2.hpp>

#include <IL/il.h>

#include "cube.hpp"

using namespace Renderer;

Cube::Cube() {}

Cube::Cube(glm::vec3 pos) : Cube() {
    position = pos;
}

Cube::Cube(glm::vec3 pos, GLuint _texture) : Cube(pos) {
    texture = _texture;
}

glm::vec3 Cube::GetMin() const {
    glm::vec3 halfSize(scale * 0.5f);
    return position - halfSize;
}

glm::vec3 Cube::GetMax() const {
    glm::vec3 halfSize(scale * 0.5f);
    return position + halfSize;
}

bool Cube::IsActive() {
    return true;
}
