#include <mesh.hpp>
#include <shaderprogram.hpp>

using namespace Renderer;

Mesh::Mesh() {
    Init();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) : Mesh() {
    this->vertices = vertices;
    this->indices = indices;

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

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Mesh::SetShader(ShaderProgram* shader) {
    pShader = shader;
}

ShaderProgram *Mesh::GetShader() {
    return pShader;
}

void Mesh::UpdateVertices(std::vector<Vertex> vertices) {
    this->vertices = vertices;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void Mesh::UpdateIndices(std::vector<GLuint> indices) {
    this->indices = indices;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

std::vector<Vertex> Mesh::GetVertices() {
    return vertices;
}

std::vector<Vertex>& Mesh::GetVerticesRef() {
    return vertices;
}

std::vector<GLuint> Mesh::GetIndices() {
    return indices;
}

void Mesh::SetColor(glm::vec3 color) {
    this->color = color;
}

glm::vec3 Mesh::GetColor() {
    return color;
}

void Mesh::Render(Envy env, GLuint texture, glm::mat4 model) {
    if (pShader == nullptr) {
        return;
    }

    glPointSize(8.0f);
    pShader->useProgram();

    pShader->setMatrix4("mvp", env.mvp);
    pShader->setVector3("viewPos", env.viewpos);
    pShader->setVector3("viewDir", env.viewdir);
    pShader->setUint("selectTriangle", env.selectTriangle);
    pShader->setVector3("highlightColor", env.highlightColor);
    pShader->setVector3("highlightColor_Vertex", env.highlightColor_Vertex);

    pShader->setVector3("color", color);
    pShader->setTexture("my_texture", texture);
    pShader->setMatrix4("model", model);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_POINTS, 0, vertices.size());
    glBindVertexArray(0);
}

glm::vec3 Mesh::GetMins() {
    glm::vec3 min = vertices[0].position;
    for (int i = 1; i < vertices.size(); i++) {
        min = glm::min(min, vertices[i].position);
    }
    return min;
}

glm::vec3 Mesh::GetMaxs() {
    glm::vec3 max = vertices[0].position;
    for (int i = 1; i < vertices.size(); i++) {
        max = glm::max(max, vertices[i].position);
    }
    return max;
}
