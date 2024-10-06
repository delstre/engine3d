#pragma once

#include <GLFW/glfw3.h>

#include <map>
#include <functional>

namespace Engine {
    struct actfunc {
        bool pressed;
        bool action;
        bool ismouse;
        std::function<void()> func;
    };

    class WindowController {
        public:
            WindowController(GLFWwindow* window);

            void ProcessInput();
            void AddCallback(int key, bool ispressed, std::function<void()> func);
            void AddCallbackMouse(int key, bool ispressed, std::function<void()> func);

            std::map<int, actfunc> callbacks;
            std::map<int, bool> keys;

        private:
            GLFWwindow* pWindow = nullptr;
    };
}
