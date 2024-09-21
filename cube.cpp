#include <iostream>

#include <glm/vec2.hpp>

#include <IL/il.h>

#include "cube.hpp"
#include "resoucemanager.hpp"

using namespace Renderer;

Cube::Cube() {}

Cube::Cube(glm::vec3 pos) : Cube() {
    position = pos;
}

Cube::Cube(glm::vec3 pos, GLuint texture) : Cube(pos) {
    texture = texture;
}

bool Cube::IsActive() {
    return true;
}
