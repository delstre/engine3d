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
#include "framebuffer.hpp"

#include "window.hpp"

namespace Engine {
    class Window;
};

namespace Renderer {
    class Interface {
        public:
            Interface(Engine::Window* window);

            void SetModelManager(ModelManager* mdlManager);
            void SetResourceManager(ResourceManager* resManager);

            void GetDebugInfo() const;
            void GetConfigInfo() const;
            void GetScene(FrameBuffer* pFbo) const;
            void GetObjectsInfo(Engine::Scene* scene) const;
            void GetCameraInfo(Engine::Scene* scene) const;
            
            void GetModelManager() const;
            void GetResourceManagerInfo() const;

            void ShowExampleAppSimpleOverlay() const;

            void Render(Engine::Scene* scene, GLuint64 elapsed_time);

        private:
            std::shared_ptr<Engine::Debug> pDebug = Engine::Debug::GetInstance();

            FrameBuffer* pFrameBuffer = nullptr;
            ModelManager* pModelManager;
            ResourceManager* pResourceManager;

            GLuint64 elapsed_time = 0;

            Engine::Window* pWindow = nullptr;
    };
}
