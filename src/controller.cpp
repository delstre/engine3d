#include "controller.hpp"

using namespace Engine;

WindowController::WindowController(GLFWwindow* window) {
    pWindow = window;
}

void WindowController::AddCallback(int key, bool ispressed, std::function<void()> func) {
    callbacks[key] = actfunc {false, ispressed, false, func};
};

void WindowController::AddCallbackMouse(int key, bool ispressed, std::function<void()> func) {
    callbacks[key] = actfunc {false, ispressed, true, func};
};

void WindowController::ProcessInput() {
    if (!callbacks.empty()) {
        for (auto& callback : callbacks) {
            int action = callback.second.ismouse ? glfwGetMouseButton(pWindow, callback.first) : glfwGetKey(pWindow, callback.first);
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
