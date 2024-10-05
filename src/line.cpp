#include "line.hpp"
#include <glm/ext/matrix_transform.hpp>

using namespace Renderer;

Line::Line(glm::vec3 start, glm::vec3 end) {
    pShader = new ShaderProgram("shaders/line.vert", "shaders/line.frag");

    vertices = {
         start.x, start.y, start.z,
         end.x, end.y, end.z,

    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    pShader->useProgram();
}

void Line::Render(glm::mat4 mvp) {
    pShader->useProgram();
    glBindVertexArray(VAO);

    pShader->setMatrix4("mvp", mvp);
    glDrawArrays(GL_LINES, 0, 2);
}
