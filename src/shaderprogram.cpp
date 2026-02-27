#include "shaderprogram.hpp"
#include <GL/glew.h>
#include <iostream>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

using namespace Renderer;

ShaderProgram::ShaderProgram(const char* vertexShader, const char* fragmentShader) {
    GLuint vertexShaderID;
    if (!createShader(readFromFile(vertexShader), GL_VERTEX_SHADER, vertexShaderID)) {
        std::cerr << "Vertex shader error\n" << std::endl;
        return;
    }

    GLuint fragmentShaderID;
    if (!createShader(readFromFile(fragmentShader), GL_FRAGMENT_SHADER, fragmentShaderID)) {
        std::cerr << "Fragment shader error\n" << std::endl;
        glDeleteShader(vertexShaderID);
        return;
    }

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShaderID);
    glAttachShader(m_ID, fragmentShaderID);
    glLinkProgram(m_ID);

    GLint success;
    glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(m_ID, 1024, nullptr, infoLog);
        std::cerr << "Shader link error\n" << infoLog << std::endl;
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        exit(1); 
    } else {
        m_IsCompiled = true;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

ShaderProgram::ShaderProgram() {
    m_ID = 0;
    m_IsCompiled = false;
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_ID);
}

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept {
    m_ID = shaderProgram.m_ID;
    m_IsCompiled = shaderProgram.m_IsCompiled;

    m_ID = 0; 
    m_IsCompiled = false;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept {
    glDeleteProgram(m_ID);
    m_ID = shaderProgram.m_ID;
    m_IsCompiled = shaderProgram.m_IsCompiled;

    m_ID = 0; 
    m_IsCompiled = false;
    return *this;
}

char* ShaderProgram::readFromFile(const char* filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "failed open file!" << std::endl;
        return nullptr;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[size + 1];

    if (file.read(buffer, size)) {
        buffer[size] = '\0';
    }

    file.close();

    return buffer;
}

void ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setVector4(const std::string& name, const glm::vec4& vector) {
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(vector));
}

void ShaderProgram::setVector3(const std::string& name, const glm::vec3& vector) {
    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(vector));
}

void ShaderProgram::setTexture(const std::string& name, const GLuint& texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), 0);
}

void ShaderProgram::setTextures(const std::string& name, const std::vector<GLuint>& textures) {
    for (GLuint i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glUniform1i(glGetUniformLocation(m_ID, (name + "[" + std::to_string(i) + "]").c_str()), i);
    }
}

void ShaderProgram::setUint(const std::string& name, const GLuint& side) {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), side);
}

bool ShaderProgram::useProgram() const {
    glUseProgram(m_ID); 
    return true;
}

bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID) {
    shaderID = glCreateShader(shaderType);
    const char* code = source.c_str();
    glShaderSource(shaderID, 1, &code, nullptr);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
        std::cerr << "Shader error: Compile time error\n" << infoLog << std::endl;
        return false;
    }

    return true;
}
