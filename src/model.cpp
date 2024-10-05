#include "model.hpp"

#include <iostream>

#include <cfloat>

using namespace Renderer;

//Model::Model(ShaderProgram* shader, std::vector<GLfloat> points, std::vector<GLuint> faces, std::vector<GLfloat> texture_points) {
    //pShader = shader;

    //glGenVertexArrays(1, &vao);

    //glGenBuffers(1, &vbo);
    //glGenBuffers(1, &t_vbo);
    //glGenBuffers(1, &c_vbo);
    //glGenBuffers(1, &ebo);

    //glBindVertexArray(vao);

    //UpdateVertices(points);
    //UpdateTexturePoints(texture_points);
    //UpdateColors(glm::vec3(1.0f, 1.0f, 1.0f));

    //glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    //glEnableVertexAttribArray(1);
    //glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);

    //glEnableVertexAttribArray(2);
    //glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    //UpdateIndices(faces);
//}

Model::Model(std::vector<Vertex> vertices, std::vector<GLuint> indices) : vertices(vertices), indices(indices) {
    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    UpdateVertices(vertices);
    UpdateIndices(indices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, texCoord)));


    glBindVertexArray(0);

}

Model::Model(Model* other) : Model(other->vertices, other->indices) {
    renderType = other->renderType;
}

Model::~Model() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Model::UpdateVertices(std::vector<Vertex> vertices) {
    this->vertices = vertices;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void Model::UpdateIndices(std::vector<GLuint> indices) {
    this->indices = indices;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void Model::SetRenderType(GLenum renderType) {
    this->renderType = renderType;
}

std::vector<Vertex> Model::GetVertices() {
    return vertices;
}

void Model::SetShader(ShaderProgram* shader) {
    pShader = shader;
}

glm::vec3 Model::GetMinBounds() {
     glm::vec3 minBounds = glm::vec3(FLT_MAX);  // Инициализируем большими значениями

    //for (size_t i = 0; i < vertices.size(); i += 3) {
        //GLfloat x = points[i];
        //GLfloat y = points[i + 1];
        //GLfloat z = points[i + 2];

        //if (x < minBounds.x) minBounds.x = x;
        //if (y < minBounds.y) minBounds.y = y;
        //if (z < minBounds.z) minBounds.z = z;
    //}

    return minBounds;
}

glm::vec3 Model::GetMaxBounds() {
    glm::vec3 maxBounds = glm::vec3(-FLT_MAX);  // Инициализируем большими значениями

    //for (size_t i = 0; i < points.size(); i += 3) {
        //GLfloat x = points[i];
        //GLfloat y = points[i + 1];
        //GLfloat z = points[i + 2];

        //if (x < maxBounds.x) maxBounds.x = x;
        //if (y < maxBounds.y) maxBounds.y = y;
        //if (z < maxBounds.z) maxBounds.z = z;
    //}

    return maxBounds;
}

void Model::Render(const Envy& env, const glm::mat4 position, const GLuint texture) {
    pShader->useProgram();
    //glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    pShader->setMatrix4("mvp", env.mvp);
    pShader->setVector3("viewPos", env.viewpos);
    pShader->setVector3("viewDir", env.viewdir);

    pShader->setTexture("my_texture", texture);
    pShader->setMatrix4("model", position);

    glBindVertexArray(vao);
    glDrawElements(renderType, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
