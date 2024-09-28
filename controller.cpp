#include "controller.hpp"

using namespace Engine;

Controller::Controller(GLFWwindow* window) {
    pWindow = window;
}

void Controller::AddCallback(int key, bool ispressed, void (*func)()) {
    callbacks[key] = actfunc {false, ispressed, func};
};

void Controller::ProcessInput() {
    if (!callbacks.empty()) {
        for (auto& callback : callbacks) {
            int action = glfwGetKey(pWindow, callback.first);
            if (callback.second.action == true) {
                if (action == GLFW_PRESS && callback.second.pressed == false) {
                    callback.second.pressed = true;
                    callback.second.func();
                } else if (action == GLFW_RELEASE && callback.second.pressed == true) {
                    callback.second.pressed = false;
                }
            } else {
                if (action == GLFW_PRESS && callback.second.pressed == false)
                    callback.second.func();
            }
        }
    }
}
