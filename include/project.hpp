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
            void SetPath(std::string path);
            void Save();
            void Create(std::string path);
            void Load();
            bool Load(std::string path);
            bool LoadLast();
            bool IncludeFile(std::string path);
            void IncludeFiles();
            void CompileFiles();
            void CompileAndRunApplication();
            Scene* GetScene();

            std::string GetPath();

            bool refresh_files = false;

        private:
            Window* pWindow;
            Scene* pScene;

            std::string path;
    };
}
