#include <structs.hpp>
#include <modelmanager.hpp>
#include <mesh.hpp>
#include <shaderprogram.hpp>

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>

using namespace Engine;

void ModelManager::SetPath(const std::string& path)
{
    #ifdef INTERFACE_DEBUG
    modelsPath = ENGINE_PATH "/models";
    shadersPath = ENGINE_PATH "/shaders";
    #else
    modelsPath = path + "/models";
    shadersPath = path + "/shaders";
    #endif
}

void ModelManager::AddModel(const std::string& name, Renderer::Mesh* model) {
    models[name] = model;
}

Renderer::Mesh* ModelManager::GetModel(const std::string& name) { 
    return models.at(name);
}

std::map<std::string, Renderer::Mesh*> ModelManager::GetModels() {
    return models;
}

void ModelManager::ImportAllModels()
{
    std::cout << "Importing models: " << modelsPath << std::endl;
    if (!std::filesystem::exists(modelsPath) || !std::filesystem::is_directory(modelsPath)) 
        return;

    for (const auto& entry : std::filesystem::directory_iterator(modelsPath)) 
        if (entry.is_regular_file() && entry.path().extension() == ".obj") 
            ModelManager::ImportModel(entry.path().string());
}

bool ModelManager::ImportModel(const std::string& modelPath) {
    std::filesystem::path full_path = modelPath;

    std::cout << "Importing model: " << full_path << std::endl;
    std::ifstream file(full_path);
    if (!file.is_open()) {
        std::cerr << "Could not open the OBJ file: " << full_path << std::endl;
        return false;
    }

    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec2> tempTexCoords;
    std::vector<glm::vec3> tempNormals;

    std::vector<Renderer::Vertex> vertices;
    std::vector<GLuint> indices;

    GLenum mode = GL_TRIANGLES;

    std::string name;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            tempVertices.push_back(vertex);
        }
        else if (prefix == "vt") {
            glm::vec2 texCoord;
            ss >> texCoord.x >> texCoord.y;
            tempTexCoords.push_back(texCoord);
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (prefix == "o") {
            ss >> name;
        }
        else if (prefix == "f") {
            std::vector<unsigned int> vIndices, tIndices, nIndices;
            unsigned int v, t, n;
            char slash;

            while (ss >> v >> slash >> t >> slash >> n) {
                vIndices.push_back(v - 1);
                tIndices.push_back(t - 1);
                nIndices.push_back(n - 1);
            }

            size_t vertexCount = vIndices.size();

            if (vertexCount == 3) {
                for (size_t i = 0; i < 3; ++i) {
                    Renderer::Vertex vertex;
                    vertex.position = tempVertices[vIndices[i]];
                    vertex.texCoord = tempTexCoords[tIndices[i]];
                    vertex.normal   = tempNormals[nIndices[i]];
                    vertices.push_back(vertex);
                    indices.push_back(static_cast<GLuint>(vertices.size() - 1));
                }
            } else if (vertexCount == 4) {
                for (int tri = 0; tri < 2; ++tri) {
                    int idx[3];
                    if (tri == 0) {
                        idx[0] = 0; idx[1] = 1; idx[2] = 2;
                    } else {
                        idx[0] = 0; idx[1] = 2; idx[2] = 3;
                    }
                    for (int j = 0; j < 3; ++j) {
                        int i = idx[j];
                        Renderer::Vertex vertex;
                        vertex.position = tempVertices[vIndices[i]];
                        vertex.texCoord = tempTexCoords[tIndices[i]];
                        vertex.normal   = tempNormals[nIndices[i]];
                        vertices.push_back(vertex);
                        indices.push_back(static_cast<GLuint>(vertices.size() - 1));
                    }
                }
            }
        }
    }

    file.close();

    Renderer::Mesh* mesh = new Renderer::Mesh(vertices, indices);
    mesh->SetShader(new Renderer::ShaderProgram((shadersPath + "/model.vert").c_str(), (shadersPath + "/model.frag").c_str()));
    mesh->name = name;
    AddModel(name, mesh);

    return true;
}

void ModelManager::UpdateShaders()
{
    for (auto it = models.begin(); it != models.end(); ++it) 
    {
        Renderer::Mesh* mesh = it->second;
        delete mesh->GetShader();
        mesh->SetShader(new Renderer::ShaderProgram((shadersPath + "/model.vert").c_str(), (shadersPath + "/model.frag").c_str()));
    }
}
