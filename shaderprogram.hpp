#pragma once

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <string>

namespace Renderer {
    class ShaderProgram {
        public:
            ShaderProgram(const char* vertexShader, const char* fragmentShader);
            
            ShaderProgram();
            ShaderProgram(const ShaderProgram&) = delete;
            ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

            ~ShaderProgram();

            ShaderProgram& operator=(const ShaderProgram&) = delete;
            ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;

            char* readFromFile(const char* filename);

            void setMatrix4(const std::string& name, const glm::mat4& matrix);
            void setTexture(const std::string& name, const GLuint& texture);
            void setSide(const std::string& name, const GLuint& side);

            bool isCompiled() const { return m_IsCompiled; };
            bool useProgram() const;

        private:
            bool createShader(const std::string& name, const GLenum shaderType, GLuint& shaderID);
            bool m_IsCompiled = false;
            GLuint m_ID = 0;
    };
}

