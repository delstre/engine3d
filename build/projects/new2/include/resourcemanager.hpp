#pragma once

#include <map>
#include <string>
#include <vector>

namespace Engine {
    namespace ResourceManager {
        void CreateTexture(std::string str);
        uint GetTexture(std::string str);
        std::vector<uint> GetTextures();

        static std::map<std::string, uint> MapTextures;
        static std::vector<uint> VecTextures = { 0 };
        static uint textureID;
    };
}
