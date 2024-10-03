#include "model.hpp"

#include <iostream>

#include <cfloat>

using namespace Renderer;

Model::Model(ShaderProgram* shader, std::vector<GLfloat> points, std::vector<GLuint> faces, std::vector<GLfloat> texture_points) {
    pShader = shader;

    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &t_vbo);
    glGenBuffers(1, &c_vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    UpdateVertices(points);
    UpdateTexturePoints(texture_points);
    UpdateColors(glm::vec3(1.0f, 1.0f, 1.0f));

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    UpdateIndices(faces);
}

Model::Model(Model* other) : Model(other->pShader, other->points, other->faces, other->texture_points) {}

Model::~Model() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &t_vbo);
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &c_vbo);
}

void Model::UpdateVertices(std::vector<GLfloat> points) {
    this->points = points;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);
}

void Model::UpdateIndices(std::vector<GLuint> faces) {
    this->faces = faces;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint), faces.data(), GL_STATIC_DRAW);
}

void Model::UpdateTexturePoints(std::vector<GLfloat> texture_points) {
    this->texture_points = texture_points;
    glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
    glBufferData(GL_ARRAY_BUFFER, texture_points.size() * sizeof(GLfloat), texture_points.data(), GL_STATIC_DRAW);
}

void Model::UpdateColors(glm::vec3 color) {
    this->colors = {
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
        color.x, color.y, color.z,
    };

    glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
}

glm::vec3 Model::GetMinBounds() {
     glm::vec3 minBounds = glm::vec3(FLT_MAX);  // Инициализируем большими значениями

    for (size_t i = 0; i < points.size(); i += 3) {
        GLfloat x = points[i];
        GLfloat y = points[i + 1];
        GLfloat z = points[i + 2];

        if (x < minBounds.x) minBounds.x = x;
        if (y < minBounds.y) minBounds.y = y;
        if (z < minBounds.z) minBounds.z = z;
    }

    return minBounds;
}

glm::vec3 Model::GetMaxBounds() {
    glm::vec3 maxBounds = glm::vec3(-FLT_MAX);  // Инициализируем большими значениями

    for (size_t i = 0; i < points.size(); i += 3) {
        GLfloat x = points[i];
        GLfloat y = points[i + 1];
        GLfloat z = points[i + 2];

        if (x < maxBounds.x) maxBounds.x = x;
        if (y < maxBounds.y) maxBounds.y = y;
        if (z < maxBounds.z) maxBounds.z = z;
    }

    return maxBounds;
}

void Model::Render(const glm::mat4 mvp, const glm::mat4 position, const GLuint texture) {
    pShader->useProgram();
    //glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    pShader->setTexture("my_texture", texture);
    pShader->setMatrix4("mvp", mvp);
    pShader->setMatrix4("model", position);

    glBindVertexArray(vao); // Bind the VAO containing VBO and IBO configurations
    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
}
