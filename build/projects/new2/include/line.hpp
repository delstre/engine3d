#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <vector>

namespace Renderer {
    class ShaderProgram;
    class Line {
        public:
            Line() = delete;
            Line(glm::vec3 start, glm::vec3 end);

            ShaderProgram* pShader;

            std::vector<float> vertices;

            GLuint VAO, VBO;

            void Render(glm::mat4 mvp);
    };
}
