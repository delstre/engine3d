#pragma once

#ifdef INTERFACE_DEBUG
#include "interface.hpp"
#endif

#include <project.hpp>
#include <GLFW/glfw3.h>

namespace Renderer {
    class Interface;
};

namespace Engine {
    class Scene;
    class Project;
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
            Project* pProject = nullptr;
            #ifdef INTERFACE_DEBUG
            Renderer::Interface* pInterface = nullptr;
            #endif
    };
}
