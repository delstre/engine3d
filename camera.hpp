#pragma once

#include "modelmanager.hpp"
#include "shaderprogram.hpp"
#include <glm/vec2.hpp>

#include <math.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


namespace Renderer {
    class ModelManager;

    enum Face { NONE, FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM };

    struct Ray {
        glm::vec3 origin;
        glm::vec3 direction;
    };

    class Camera {
        public:
            Camera(ShaderProgram* shaderProgram, ShaderProgram* debugProgram, glm::ivec2* pWinSize);

            ShaderProgram* pShaderProgram;
            ShaderProgram* pDebugProgram;
            glm::ivec2* pWinSize;

            glm::vec3 position;
            glm::vec3 front;
            glm::vec3 up;
            glm::vec3 right;
            glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

            glm::mat4 projection;
            glm::mat4 view;

            float yaw = 0.0f;
            float pitch = 0.0f;;

            float speed = 2.5f;
            float sensitivity = 0.1f;

            bool constrainPitch = true;

            glm::vec3 getCursor3DPos(double x, double y);

            float SpectAxis();
            float ReCalculateFOV();

            glm::vec3 GetMouseRay(int mouseX, int mouseY, int windowWidth, int windowHeight, const glm::mat4& projection, const glm::mat4& view);
            void CheckRayIntersection(GLFWwindow* window, ModelManager mdlManager);
            bool RayIntersectsBox(const glm::vec3& origin, const glm::vec3& direction, const glm::vec3& boxMin, const glm::vec3& boxMax);
            Face GetIntersectedFace(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& boxMin, const glm::vec3& boxMax);

            void Render();

            void updateCameraVectors();
            void ProcessMouseMovement(float xoffset, float yoffset);
            void ProcessKeyboardInput(int direction, float deltaTime);

            void CreateRay(const glm::vec3& origin, const glm::vec3& direction, float length);
            void RenderRays();
            std::vector<glm::vec3> rays;
            float rayLength;
            void Control(GLFWwindow* window, float deltaTime);
            GLuint vao, vbo;
    };
}

