#include "resoucemanager.cpp"

#include <string>

using namespace Renderer;

ResourceManager::ResourceManager() {}

void ResourceManager::CreateTexture(std::string str) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Устанавливаем параметры текстуры (фильтрация и повторение)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Инициализируем DevIL и создаем ID изображения
    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    // Загружаем изображение с использованием DevIL
    if (!ilLoadImage(filename)) {
        std::cerr << "Ошибка загрузки изображения: " << filename << std::endl;
        ilDeleteImages(1, &imageID);
        return 0;
    }

    // Преобразуем изображение в формат, совместимый с OpenGL
    ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

    // Получаем размеры изображения
    int width = ilGetInteger(IL_IMAGE_WIDTH);
    int height = ilGetInteger(IL_IMAGE_HEIGHT);
    unsigned char* data = ilGetData();

    // Передаем данные изображения в OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Удаляем ID изображения DevIL, поскольку он больше не нужен
    ilDeleteImages(1, &imageID);

    textures[str] = textureID; 
}

GLuint ResourceManager::GetTexture(const char* str) {
    return textures[str];
}
