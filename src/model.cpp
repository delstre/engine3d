#include <model.hpp>
#include <transform.hpp>
#include "object.hpp"

#include <iostream>

#include <cfloat>

using namespace Renderer;

ModelRender::ModelRender(std::vector<Vertex> vertices, std::vector<GLuint> indices) : vertices(vertices), indices(indices) {
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

ModelRender::~ModelRender() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

ModelRender& ModelRender::operator=(const ModelRender& other) {
    if (this == &other) {
        return *this;
    }

    ModelRender* model = new ModelRender(other.vertices, other.indices);
    model->SetShader(new ShaderProgram("../shaders/model.vert", "../shaders/model.frag"));
    model->SetRenderType(other.renderType);

    return *model;
}

void ModelRender::UpdateVertices(std::vector<Vertex> vertices) {
    this->vertices = vertices;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void ModelRender::UpdateIndices(std::vector<GLuint> indices) {
    this->indices = indices;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void ModelRender::SetRenderType(GLenum renderType) {
    this->renderType = renderType;
}

std::vector<Vertex> ModelRender::GetVertices() {
    return vertices;
}

void ModelRender::SetShader(ShaderProgram* shader) {
    pShader = shader;
}

void ModelRender::SetModel(ModelRender* model) {
    renderType = model->renderType;
    pShader = model->pShader;
    vertices = model->vertices;
    indices = model->indices;
    vao = model->vao;
    vbo = model->vbo;
    ebo = model->ebo;
}

glm::vec3 ModelRender::GetMinBounds() {
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

glm::vec3 ModelRender::GetMaxBounds() {
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

void ModelRender::Update() {
    if (pShader == nullptr) {
        return;
    }

    pShader->useProgram();
    //glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    pShader->setMatrix4("mvp", parent->env.mvp);
    pShader->setVector3("viewPos", parent->env.viewpos);
    pShader->setVector3("viewDir", parent->env.viewdir);

    pShader->setTexture("my_texture", texture);
    Engine::Transform* transform = GetComponent<Engine::Transform>();
    if (transform != nullptr)
        pShader->setMatrix4("model", transform->GetMatrix());

    glBindVertexArray(vao);
    glDrawElements(renderType, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void ModelRender::InterfaceUpdate() {
    ImGui::Text("Vertices: %d", vertices.size());
    ImGui::Text("Indices: %d", indices.size());
}
