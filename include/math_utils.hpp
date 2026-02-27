#pragma once

namespace Renderer {
    class Mesh;
};

namespace Engine {
    class Object;
    class Scene;
};

#include <glm/glm.hpp>
#include <vector>

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

struct Intersection {
    bool hit;
    float distance;
    glm::vec3 point;
    glm::vec3 barycentric;
    int triangleIndex;
    int vertexIndex;
    Renderer::Mesh* mesh;
    Engine::Object* object;
    glm::vec3 localPosition;
};

bool rayTriangleIntersect(const Ray&, const glm::vec3&, const glm::vec3&, const glm::vec3&, float&, float&, float&);
std::vector<int> collectVerticesAtPosition(Renderer::Mesh*, const glm::vec3&, float eps = 0.001f);
