#pragma once

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <memory>
#include <nfd.h>

#include "camera.hpp"
#include "object.hpp"
#include "debug.hpp"

#include "modelmanager.hpp"
#include "resourcemanager.hpp"

namespace Renderer {
    class Interface {
        public:
            Interface(GLFWwindow* window);

            void AddCameraInfo(Camera* camera);
            void AddObjectsInfo(std::vector<Object*>* objects);

            void SetModelManager(ModelManager* mdlManager);
            void SetResourceManager(ResourceManager* resManager);

            void GetDebugInfo() const;
            void GetCameraInfo() const;
            void GetObjectsInfo() const;
            void GetConfigInfo() const;
            
            void GetModelManagerInfo() const;
            void GetResourceManagerInfo() const;

            void ShowExampleAppSimpleOverlay() const;

            void Render(GLuint64 elapsed_time);

        private:
            Camera* pCamera = nullptr;
            std::vector<Object*>* pObjects = nullptr;
            std::shared_ptr<Engine::Debug> pDebug = Engine::Debug::GetInstance();

            ModelManager* pModelManager;
            ResourceManager* pResourceManager;

            GLuint64 elapsed_time = 0;
    };
}
