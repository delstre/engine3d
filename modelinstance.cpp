#pragma once

#include "modelinstance.hpp"

using namespace Renderer;

ModelInstance::ModelInstance(ShaderProgram* shader, std::vector<GLfloat> points, std::vector<GLuint> faces, std::vector<GLfloat> texture_points) : Model(shader, points, faces, texture_points) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, p_vbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, p_vbo);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pt_vbo);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, pt_vbo);
}

ModelInstance::~ModelInstance() {
    glDeleteBuffers(1, &p_vbo);
    glDeleteBuffers(1, &pt_vbo);
}

void ModelInstance::UpdatePositions(std::vector<glm::mat4*> pPositions) {
    std::vector<glm::mat4> mat4s;
    for (glm::mat4* position : pPositions) {
        mat4s.push_back(*position);
    }

    this->positions = mat4s;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, p_vbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, positions.size() * sizeof(glm::mat4), positions.data(), GL_STATIC_DRAW);
}

void ModelInstance::UpdateTextures(std::vector<glm::uint> textures) {
    this->textures = textures;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pt_vbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, textures.size() * sizeof(uint), textures.data(), GL_STATIC_DRAW);
}

void ModelInstance::Render(const glm::mat4 mvp, const std::vector<GLuint>& textures) {
    pShader->useProgram();
    glBindVertexArray(vao);
    pShader->setMatrix4("mvp", mvp);
    pShader->setTextures("textures", textures);
    pShader->setUint("active_id", active_id);
    pShader->setUint("call_id", call_id);
    glDrawElementsInstanced(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0, positions.size());
}
