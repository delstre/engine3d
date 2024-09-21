#pragma once

#include "shaderprogram.hpp"
#include <glm/vec2.hpp>

#include <math.h>

#include <GLFW/glfw3.h>

namespace Renderer {
    class Camera {
        public:
            Camera(ShaderProgram* shaderProgram, glm::ivec2* pWinSize);

            ShaderProgram* pShaderProgram;
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

            void Render();

            void updateCameraVectors();
            void ProcessMouseMovement(float xoffset, float yoffset);
            void ProcessKeyboardInput(int direction, float deltaTime);

            void Control(GLFWwindow* window, float deltaTime);
    };
}

