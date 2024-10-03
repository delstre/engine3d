#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <memory>

#include "camera.hpp"
#include "object.hpp"
#include "debug.hpp"

namespace Renderer {
    class Interface {
        public:
            Interface(GLFWwindow* window);

            void AddCameraInfo(Camera* camera);
            void AddObjectsInfo(std::vector<Object*>* objects);

            void GetDebugInfo() const;
            void GetCameraInfo() const;
            void GetObjectsInfo() const;
            void GetConfigInfo() const;
            void ShowExampleAppSimpleOverlay() const;

            void Render(GLuint64 elapsed_time);

        private:
            Camera* pCamera = nullptr;
            std::vector<Object*>* pObjects = nullptr;
            std::shared_ptr<Engine::Debug> pDebug = Engine::Debug::GetInstance();

            GLuint64 elapsed_time = 0;
    };
}
