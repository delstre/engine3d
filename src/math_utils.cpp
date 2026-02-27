#include <math_utils.hpp>
#include <mesh.hpp>

bool rayTriangleIntersect(const Ray& ray, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t, float& u, float& v) 
{
    const float EPSILON = 1e-6f;
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 h = glm::cross(ray.direction, edge2);
    float a = glm::dot(edge1, h);
    if (fabs(a) < EPSILON) return false;
    if (a < 0) return false; 

    float f = 1.0f / a;
    glm::vec3 s = ray.origin - v0;
    u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f) return false;

    glm::vec3 q = glm::cross(s, edge1);
    v = f * glm::dot(ray.direction, q);
    if (v < 0.0f || u + v > 1.0f) return false;

    t = f * glm::dot(edge2, q);
    return (t > EPSILON);
}

std::vector<int> collectVerticesAtPosition(Renderer::Mesh* mesh, const glm::vec3& localPos, float epsilon) {
    std::vector<int> result;
    const auto& vertices = mesh->GetVertices();
    for (size_t i = 0; i < vertices.size(); ++i) {
        if (glm::distance(vertices[i].position, localPos) <= epsilon) {
            result.push_back(static_cast<int>(i));
        }
    }
    return result;
}
