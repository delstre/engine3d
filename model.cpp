#include "model.hpp"

#include <iostream>

using namespace Renderer;

Model::Model() {};

Model::Model(std::vector<GLfloat> points, std::vector<GLuint> faces, std::vector<GLfloat> texture_points) : points(points), faces(faces), texture_points(texture_points) {
    pShader = new ShaderProgram("shaders/model.vert", "shaders/model.frag");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &t_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
    glBufferData(GL_ARRAY_BUFFER, texture_points.size() * sizeof(GLfloat), texture_points.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint), faces.data(), GL_STATIC_DRAW);

    pShader->useProgram();
}

Model::Model(ShaderProgram* shader, std::vector<GLfloat> points, std::vector<GLuint> faces, std::vector<GLfloat> texture_points) : points(points), faces(faces), texture_points(texture_points) {
    pShader = shader;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &t_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
    glBufferData(GL_ARRAY_BUFFER, texture_points.size() * sizeof(GLfloat), texture_points.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint), faces.data(), GL_STATIC_DRAW);

}

void Model::Render(const glm::mat4 mvp, const glm::mat4 position, const GLuint texture, bool wireframe) {
    pShader->useProgram();
    //glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    //pShader->setTexture("my_texture", mdl.texture);
    pShader->setMatrix4("mvp", mvp);
    pShader->setMatrix4("model", position);
    if (texture != 0)
        pShader->setTexture("texture", texture);

    glBindVertexArray(vao); // Bind the VAO containing VBO and IBO configurations
    glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
}
