#pragma once

#include "shaderprogram.hpp"
#include <glm/vec2.hpp>

#include <math.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


namespace Renderer {
    enum Face { NONE, FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM };

    class Camera {
        public:
            Camera() = delete;
            Camera(GLFWwindow* window);

            GLFWwindow* pWindow;
            int wid, hei;

            void UpdateWindowSize();
            void UpdateMVP();

            glm::vec3 position;
            glm::vec3 front;
            glm::vec3 up;
            glm::vec3 right;
            glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

            glm::mat4 projection;
            glm::mat4 view;
            glm::mat4 mvp;

            float yaw = 0.0f;
            float pitch = 0.0f;;

            float speed = 2.5f;
            float sensitivity = 0.1f;

            bool constrainPitch = true;

            glm::vec3 getCursor3DPos(double x, double y);


            float SpectAxis();
            float ReCalculateFOV();

            glm::vec3 GetMouseRay(int mouseX, int mouseY, int windowWidth, int windowHeight, const glm::mat4& projection, const glm::mat4& view);
            void CheckRayIntersection(GLFWwindow* window);
            bool RayIntersectsBox(const glm::vec3& origin, const glm::vec3& direction, const glm::vec3& boxMin, const glm::vec3& boxMax);
            Face GetIntersectedFace(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& boxMin, const glm::vec3& boxMax);

            void Think();

            void updateCameraVectors();
            void ProcessMouseMovement(float xoffset, float yoffset);
            void ProcessKeyboardInput(int direction, float deltaTime);

            void Control(GLFWwindow* window, float deltaTime);
            GLuint vao, vbo;
    };
}

