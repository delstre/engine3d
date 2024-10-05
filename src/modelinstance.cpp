#pragma once

#include "modelinstance.hpp"

using namespace Renderer;

ModelInstance::ModelInstance(std::vector<Vertex> vertexs, std::vector<GLuint> faces) : Model(vertexs, faces) {
    glGenBuffers(1, &p_vbo);
    glGenBuffers(1, &pt_vbo);
    glGenBuffers(1, &pc_vbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, p_vbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, p_vbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pt_vbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, pt_vbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pc_vbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, pc_vbo);
}

ModelInstance::ModelInstance(Model* model) : Model(model) {
    pShader = new ShaderProgram("shaders/model_instance.vert", "shaders/model_instance.frag");

    glGenBuffers(1, &p_vbo);
    glGenBuffers(1, &pt_vbo);
    glGenBuffers(1, &pc_vbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, p_vbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, p_vbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pt_vbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, pt_vbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pc_vbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, pc_vbo);

    UpdatePositions({glm::mat4(1.0f)});
    UpdateTextures({0});
    UpdateColors({glm::vec3(1.0f, 1.0f, 1.0f)});
}

ModelInstance::ModelInstance(Model* model, std::vector<glm::mat4> matrixes, std::vector<uint> textures, std::vector<glm::vec3> colors) : ModelInstance(model) {
    UpdatePositions(matrixes);
    UpdateTextures(textures);
    UpdateColors(colors);
}

ModelInstance::~ModelInstance() {
    glDeleteBuffers(1, &p_vbo);
    glDeleteBuffers(1, &pt_vbo);
    glDeleteBuffers(1, &pc_vbo);
}

std::vector<glm::mat4>& ModelInstance::GetMatrixes() {
    return matrixes;
}

std::vector<uint>& ModelInstance::GetTextures() {
    return textures;
}

void ModelInstance::AddModel(const glm::mat4& matrix = glm::mat4(1.0f), const uint& texture = 0) {
    matrixes.push_back(matrix);
    textures.push_back(texture);
}

void ModelInstance::UpdatePositions(const std::vector<glm::mat4>& matrixes) {
    this->matrixes = matrixes;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, p_vbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, matrixes.size() * sizeof(glm::mat4), matrixes.data(), GL_STATIC_DRAW);
}

void ModelInstance::UpdatePositions() {
    UpdatePositions(matrixes);
}


void ModelInstance::UpdateTextures(const std::vector<uint>& textures) {
    this->textures = textures;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pt_vbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, textures.size() * sizeof(uint), textures.data(), GL_STATIC_DRAW);
}

void ModelInstance::UpdateTextures() {
    UpdateTextures(textures);
}

void ModelInstance::UpdateColors(const std::vector<glm::vec3>& colors) {
    this->colors = colors;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pc_vbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
}

void ModelInstance::UpdateColors() {
    UpdateColors(colors);
}

void ModelInstance::UpdateColor(int id, const glm::vec3& color) {
    colors[id] = color;
    UpdateColors();
}

void ModelInstance::Render(const Envy& env, const std::vector<GLuint>& textures) {
    pShader->useProgram();
    glBindVertexArray(vao);
    pShader->setMatrix4("mvp", env.mvp);
    pShader->setVector3("viewPos", env.viewpos);
    pShader->setVector3("viewDir", env.viewdir);

    pShader->setTextures("textures", textures);
    // uniform for max textures (textures[MAX_TEXTURES])
    pShader->setUint("active_id", active_id);
    pShader->setUint("call_id", call_id);
    glDrawElementsInstanced(renderType, indices.size(), GL_UNSIGNED_INT, 0, matrixes.size());
}
