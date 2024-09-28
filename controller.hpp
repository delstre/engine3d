#pragma once

#include <GLFW/glfw3.h>

#include <map>

namespace Engine {

    struct actfunc {
        bool pressed;
        bool action;
        void (*func)();
    };

    class Controller {
        public:
            Controller(GLFWwindow* window);

            void ProcessInput();
            void AddCallback(int key, bool ispressed, void (*func)());

            std::map<int, actfunc> callbacks;
            std::map<int, bool> keys;

        private:
            GLFWwindow* pWindow = nullptr;
    };
}
