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

#include "scene.hpp"
#include "project.hpp"

namespace Engine {
    class Window;
    class Project;
};

namespace Renderer {
    class Interface {
        public:
            Interface(Engine::Window* window);

            void SetProject(Engine::Project* project);
            Engine::Scene* GetScene() const;

            void GetDebugInfo() const;
            void GetConfigInfo() const;

            void GetSceneInfo(Engine::Scene* scene) const;
            void GetCameraInfo(Engine::Scene* scene) const;
            void GetObjectsInfo(Engine::Scene* scene);
            void ObjectInspector(Engine::Scene* scene) const;
            void GetModelManager(Engine::Scene* scene) const;
            void GetResourceManager(Engine::Scene* scene) const;
            void GetFiles() const;

            void ShowExampleAppSimpleOverlay() const;

            void Render(GLuint64 elapsed_time);

        private:
            std::shared_ptr<Engine::Debug> pDebug = Engine::Debug::GetInstance();

            Engine::Project* pProject = nullptr;

            FrameBuffer* pFrameBuffer = nullptr;

            GLuint64 elapsed_time = 0;

            Engine::Window* pWindow = nullptr;

            Renderer::Object* pSelectedObject = nullptr;
    };
}
