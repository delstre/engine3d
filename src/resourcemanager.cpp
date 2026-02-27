#include <resourcemanager.hpp>

#include <GL/glew.h>
#include <IL/il.h>

#include <string>
#include <iostream>

using namespace Engine;

void ResourceManager::CreateTexture(std::string filename) {
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    if (!ilLoadImage(filename.c_str())) {
        std::cerr << "Ошибка загрузки изображения: " << filename << std::endl;
        ilDeleteImages(1, &imageID);
        return;
    }

    ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

    int width = ilGetInteger(IL_IMAGE_WIDTH);
    int height = ilGetInteger(IL_IMAGE_HEIGHT);
    unsigned char* data = ilGetData();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    ilDeleteImages(1, &imageID);

    MapTextures[filename] = textureID; 
    VecTextures.push_back(textureID);
}

GLuint ResourceManager::GetTexture(std::string file) {
    return MapTextures[file];
}

std::vector<GLuint> ResourceManager::GetTextures() {
    return VecTextures;
}
