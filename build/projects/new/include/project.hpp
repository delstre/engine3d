#pragma once

#include <window.hpp>
#include <scene.hpp>

#include <string>

namespace Engine {
    class Window;
    class Project {
        public:
            Project(Window* pWindow);
            void Init();
            void Save();
            bool Load(std::string path);
            void LoadLast();
            bool IncludeFile(std::string path);
            void IncludeFiles();
            bool CompileFiles();
            Scene* GetScene();

            std::string GetPath();

            bool refresh_files = false;

        private:
            Window* pWindow;
            Scene* pScene;

            std::string path;
    };
}
