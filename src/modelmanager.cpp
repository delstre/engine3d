#include <iostream>
#include <fstream>
#include <sstream>

#include <filesystem>

#include <structs.hpp>
#include <modelmanager.hpp>

using namespace Engine;

void ModelManager::SetPath(std::string _path) {
    path = _path;
}

void ModelManager::AddModel(std::string name, Renderer::Mesh* model) {
    models[name] = model;
}

Renderer::Mesh* ModelManager::GetModel(const std::string& name) { 
    return models.at(name);
}

std::map<std::string, Renderer::Mesh*> ModelManager::GetModels() {
    return models;
}

bool ModelManager::ImportModel(const std::string& _path) {
    std::filesystem::path full_path = path + "/" + _path;

    std::cout << "Importing model: " << full_path << std::endl;
    std::ifstream file(full_path);
    if (!file.is_open()) {
        std::cerr << "Could not open the OBJ file: " << full_path << std::endl;
        return false;
    }

    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempTexCoords;
    std::vector<glm::vec3> tempNormals;

    std::vector<Renderer::Vertex> vertices;  // Vector to hold vertex data
    std::vector<GLuint> indices;    // Vector to hold indices

    GLenum mode = GL_TRIANGLES;

    std::string name;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {  // Vertex position
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            tempVertices.push_back(vertex);
        }
        else if (prefix == "vt") {  // Texture coordinate
            glm::vec2 texCoord;
            ss >> texCoord.x >> texCoord.y;
            tempTexCoords.push_back(texCoord);
        }
        else if (prefix == "vn") {  // Vertex normal
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (prefix == "o") {
            ss >> name;
        }
        else if (prefix == "f") {  // Face
            unsigned int vertexIndex[4], texCoordIndex[4], normalIndex[4]; // Support for up to 4 indices
            char slash;

            // Count how many vertices are in the face
            size_t vertexCount = 0;

            while (ss >> vertexIndex[vertexCount] >> slash >> texCoordIndex[vertexCount] >> slash >> normalIndex[vertexCount]) {
                vertexCount++;
            }

            for (size_t i = 0; i < vertexCount; i++) {
                unsigned int vi = vertexIndex[i] - 1;
                unsigned int ti = texCoordIndex[i] - 1;
                unsigned int ni = normalIndex[i] - 1;

                // Создаем вершину
                Renderer::Vertex vertex;
                vertex.position = tempVertices[vi];
                vertex.texCoord = tempTexCoords[ti];
                vertex.normal = tempNormals[ni];

                // Добавляем вершину в массив
                vertices.push_back(vertex);
                indices.push_back(static_cast<GLuint>(vertices.size() - 1));
            }

            if (vertexCount == 3) {
                mode = GL_TRIANGLES;
            } else if (vertexCount == 4) {
                mode = GL_QUADS;
            }
        }
    }

    file.close();

    Renderer::Mesh* mesh = new Renderer::Mesh(vertices, indices);
    mesh->SetShader(new Renderer::ShaderProgram((path + "/shaders/model.vert").c_str(), (path + "/shaders/model.frag").c_str()));
    mesh->SetRenderType(mode);
    mesh->name = name;
    AddModel(name, mesh);

    return true;
}
