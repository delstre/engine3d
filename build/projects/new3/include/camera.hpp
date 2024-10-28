#pragma once

#include <math.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <cfloat>

namespace Engine {
    class Object;
    class Scene;

    class Camera {
        public:
            Camera() = delete;
            Camera(Scene* scene);

            int wid, hei;

            void UpdateMVP();

            glm::vec4 planes[6]; // frustum

            glm::vec3 position;
            glm::vec3 front;
            glm::vec3 up;
            glm::vec3 right;
            glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

            glm::mat4 projection;
            glm::mat4 view;
            glm::mat4 mvp;

            float zoom = 45.0f;

            float yaw = 0.0f;
            float pitch = 0.0f;;

            float speed = 2.5f;
            float sensitivity = 0.1f;

            bool constrainPitch = true;

            glm::vec3 GetCursorDirection(double mouseX, double mouseY, double width, double height);

            void Think();

            void updateCameraVectors();
            void ProcessMouseMovement(float xoffset, float yoffset);
            void ProcessKeyboardInput(int direction, float deltaTime);

            void Control(GLFWwindow* window, float deltaTime);
        private:
            Scene* scene;
    };
}

