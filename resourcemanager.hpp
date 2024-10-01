#pragma once

#include <map>
#include <string>
#include <GL/glew.h>

namespace Renderer {
    class ResourceManager {
        public:
        ResourceManager();

        void CreateTexture(std::string str);
        GLuint GetTexture(std::string str);

        std::map<std::string, GLuint> textures;
        GLuint textureID;
    };
}
