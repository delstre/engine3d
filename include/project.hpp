#pragma once

#include <window.hpp>

#include <string>

namespace Engine {
    class Window;
    class Project {
        public:
            Project(Window* pWindow);
            void Init();
            void Save(std::string path);
            bool Load(std::string path);
            bool IncludeFile(std::string path);
            Scene* GetScene();

            std::vector<std::string> GetFiles();

        private:
            Window* pWindow;
            Scene* pScene;

            std::vector<std::string> files;
    };
}
