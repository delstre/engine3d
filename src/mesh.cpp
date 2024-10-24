#include <mesh.hpp>

using namespace Renderer;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices) {
    this->vertices = vertices;
    this->indices = indices;

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

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Mesh::SetShader(ShaderProgram* shader) {
    pShader = shader;
}

void Mesh::SetRenderType(GLenum renderType) {
    this->renderType = renderType;
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

void Mesh::Render(Envy env, GLuint texture, glm::mat4 model) {
    if (pShader == nullptr) {
        return;
    }

    pShader->useProgram();

    pShader->setMatrix4("mvp", env.mvp);
    pShader->setVector3("viewPos", env.viewpos);
    pShader->setVector3("viewDir", env.viewdir);

    pShader->setTexture("my_texture", texture);
    pShader->setMatrix4("model", model);

    glBindVertexArray(vao);
    glDrawElements(renderType, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
