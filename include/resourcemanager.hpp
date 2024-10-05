#pragma once

#include <map>
#include <string>
#include <GL/glew.h>
#include <vector>

namespace Renderer {
    class ResourceManager {
        public:
        ResourceManager();

        void CreateTexture(std::string str);
        GLuint GetTexture(std::string str);
        std::vector<GLuint> GetTextures();

        std::map<std::string, GLuint> MapTextures;
        std::vector<GLuint> VecTextures = { 0 };
        GLuint textureID;
    };
}
