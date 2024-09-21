#pragma once

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <string>
#include <mutex>

namespace Renderer {
    class ShaderProgram {
        public:
            ShaderProgram(const std::string& name, const std::string& frag);
            ~ShaderProgram();
            
            ShaderProgram() = delete;
            ShaderProgram(ShaderProgram&) = delete;
            ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

            ShaderProgram& operator=(const ShaderProgram&) = delete;
            ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;

            void setMatrix4(const std::string& name, const glm::mat4& matrix);
            void setTexture(const std::string& name, const GLuint& texture);

            bool isCompiled() const { return m_IsCompiled; };
            bool useProgram() const;

            std::mutex mutex;
        private:
            bool createShader(const std::string& name, const GLenum shaderType, GLuint& shaderID);
            bool m_IsCompiled = false;
            GLuint m_ID = 0;
    };
}

