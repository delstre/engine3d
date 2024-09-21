#include "shaderprogram.hpp"
#include <GL/glew.h>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

using namespace Renderer;

ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    GLuint vertexShaderID;
    if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {
        std::cerr << "Vertex shader error\n" << std::endl;
        return;
    }

    GLuint fragmentShaderID;
    if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
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
    } else {
        m_IsCompiled = true;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
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

void ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setTexture(const std::string& name, const GLuint& texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), 0);
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
