#pragma once

#include "scene.hpp"
#include "interface.hpp"
#include <GLFW/glfw3.h>

namespace Renderer {
    class Interface;
};

namespace Engine {
    class Window {
        public:
            Window(int width, int height, const char* title) : width(width), height(height), window_name(title) {};
            ~Window() { glfwTerminate(); };

            void Init();
            void Resize(int width, int height);
            void SetCursorPos(double xpos, double ypos);
            void SceneInit(Scene* scene);
            void Render();

            GLFWwindow* GetWindow();

        private:
            int width;
            int height;
            const char* window_name;

            GLFWwindow* pWindow = nullptr;
            Scene* pScene = nullptr;
            ComponentManager* pComponentManager = nullptr;
            Renderer::Interface* pInterface = nullptr;
    };
}
