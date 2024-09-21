#pragma once

#include <map>
#include <string>

namespace Renderer {
    class ResourceManager {
        public:
        ResourceManager();

        void CreateTexture(std::string str);
        GLuint GetTexture(std::string str);

        std::map<std::string, GLuint> textures;
    };
}
