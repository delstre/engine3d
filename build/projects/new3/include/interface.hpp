#pragma once

#include <GL/glew.h>
#include <memory>

namespace Engine {
    class Window;
    class Scene;
    class Project;
    class Object;
};

namespace Renderer {
    class FrameBuffer;
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
            void Console(Engine::Scene* scene) const;
            void GetFiles() const;

            void ShowExampleAppSimpleOverlay() const;

            void Render(GLuint64 elapsed_time);

        private:
            Engine::Project* pProject = nullptr;

            FrameBuffer* pFrameBuffer = nullptr;

            GLuint64 elapsed_time = 0;

            Engine::Window* pWindow = nullptr;

            Engine::Object* pSelectedObject = nullptr;
    };
}
