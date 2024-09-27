#include "modelmanager.hpp"
#include "shaderprogram.hpp"
#include "cube.hpp"
#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

using namespace Renderer;

ModelManager::ModelManager(ShaderProgram* shaderProgram, Camera* camera) {
    points = {
        // Positions for each corner of the cube
        -1.0f, -1.0f,  1.0f,  // 0: Front bottom left
         1.0f, -1.0f,  1.0f,  // 1: Front bottom right
         1.0f,  1.0f,  1.0f,  // 2: Front top right
        -1.0f,  1.0f,  1.0f,  // 3: Front top left
        -1.0f, -1.0f, -1.0f,  // 4: Back bottom left
         1.0f, -1.0f, -1.0f,  // 5: Back bottom right
         1.0f,  1.0f, -1.0f,  // 6: Back top right
        -1.0f,  1.0f, -1.0f   // 7: Back top left
    };

    elements = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Back face
        7, 6, 5,
        5, 4, 7,
        // Left face
        4, 0, 3,
        3, 7, 4,
        // Bottom face
        4, 5, 1,
        1, 0, 4,
        // Top face
        3, 2, 6,
        6, 7, 3
    };

    texture_points = {
        // Front face
        0.0f, 0.0f, 
        1.0f, 0.0f, 
        1.0f, 1.0f, 
        0.0f, 1.0f, 

        // Right face
        1.0f, 0.0f, 
        1.0f, 1.0f, 
        0.0f, 1.0f, 
        0.0f, 0.0f, 

        // Back face
        1.0f, 0.0f, 
        1.0f, 1.0f, 
        0.0f, 1.0f, 
        0.0f, 0.0f, 

        // Left face
        0.0f, 0.0f, 
        1.0f, 0.0f, 
        1.0f, 1.0f, 
        0.0f, 1.0f, 

        // Top face
        0.0f, 1.0f, 
        0.0f, 0.0f, 
        1.0f, 0.0f, 
        1.0f, 1.0f, 

        // Bottom face
        1.0f, 1.0f, // up left
        1.0f, 0.0f, // up right
        1.0f, 0.0f, // bottom right
        0.0f, 0.0f, // bottom left
    };

    normals = {
        // Передняя грань
        0.0f, 0.0f,  1.0f,  // 4
        0.0f, 0.0f,  1.0f,  // 5
        0.0f, 0.0f,  1.0f,  // 6
        0.0f, 0.0f,  1.0f,  // 7
        // Правая грань
        1.0f, 0.0f, 0.0f,   // 1
        1.0f, 0.0f, 0.0f,   // 2
        1.0f, 0.0f, 0.0f,   // 6
        1.0f, 0.0f, 0.0f,    // 5
         //// Задняя грань
        0.0f, 0.0f, -1.0f,  // 0
        0.0f, 0.0f, -1.0f,  // 1
        0.0f, 0.0f, -1.0f,  // 2
        0.0f, 0.0f, -1.0f,  // 3
        // Левая грань
        -1.0f, 0.0f, 0.0f,  // 0
        -1.0f, 0.0f, 0.0f,  // 3
        -1.0f, 0.0f, 0.0f,  // 7
        -1.0f, 0.0f, 0.0f,  // 4
        // Верхняя грань
        0.0f,  1.0f, 0.0f,  // 3
        0.0f,  1.0f, 0.0f,  // 2
        0.0f,  1.0f, 0.0f,  // 6
        0.0f,  1.0f, 0.0f,  // 7
        //// Нижняя грань
        0.0f, -1.0f, 0.0f,  // 0
        0.0f, -1.0f, 0.0f,  // 1
        0.0f, -1.0f, 0.0f,  // 5
        0.0f, -1.0f, 0.0f,  // 4
    };

    glGenVertexArrays(1, &vao); // Create a VAO to store state
    glBindVertexArray(vao);     // Bind the VAO

    // Generate and bind VBO for vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &n_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, n_vbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &t_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
    glBufferData(GL_ARRAY_BUFFER, texture_points.size() * sizeof(GLfloat), texture_points.data(), GL_STATIC_DRAW);

    // Enable vertex attribute array (assuming the shader uses location 0 for position)
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, n_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, t_vbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);

    // Generate and bind IBO for indices
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), elements.data(), GL_STATIC_DRAW);

    // Unbind the VAO (optional, good practice)
    //glBindVertexArray(0);
    pShaderProgram = shaderProgram;
    pCamera = camera;

    glBindVertexArray(vbo); // Bind the VAO containing VBO and IBO configurations
    glPointSize(10.0f);

}

void ModelManager::UpdateArrayBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);

    std::cout << "UPDATE BUFFER" << std::endl;
}

void ModelManager::AddModel() {
    Cube mdl = Cube();
}

void ModelManager::AddModel(Cube mdl) {
    mdl.vao = vbo;
    vecModels.insert(vecModels.begin(), mdl);
}

void ModelManager::RemoveModel(int index) {
    vecModels.erase(vecModels.begin() + index);
}

glm::vec4* ModelManager::GetPlanes() {}

bool ModelManager::isCubeInFrustum(const glm::vec3& min, const glm::vec3& max) {
    return true; // Куб находится в фрустуме
}


void ModelManager::Render() {
    pShaderProgram->useProgram();

    for (Cube mdl : vecModels) {
        //if (isCubeInFrustum(mdl.GetMin(), mdl.GetMax())) {
            //glm::mat4 model = glm::translate(glm::mat4(1.0f), mdl.position);
            //model = glm::scale(model, glm::vec3(mdl.scale, mdl.scale, mdl.scale));
            glPolygonMode(GL_FRONT_AND_BACK, isWireFrame ? GL_LINE : GL_FILL);
            pShaderProgram->setTexture("my_texture", mdl.texture);
            pShaderProgram->setMatrix4("model", mdl.model);

            //glBindVertexArray(vbo); // Bind the VAO containing VBO and IBO configurations
            glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);

            //glBindVertexArray(0);   // Unbind the VAO
        //}
    }
}
