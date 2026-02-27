#pragma once

#include <map>
#include <string>
#include <vector>

namespace Engine {
    /**
     * Пространство имён ResourceManager отвечает за загрузку и хранение текстур.
     * Текстуры загружаются с помощью DevIL и сохраняются в OpenGL-совместимом формате.
     * Все текстуры хранятся в статических контейнерах, доступных по имени файла.
     */
    namespace ResourceManager {
        /**
         * Загружает текстуру из файла и создаёт соответствующий объект OpenGL.
         * Изображение конвертируется в формат RGB, генерируются мип-карты.
         * Текстура добавляется в глобальные контейнеры MapTextures и VecTextures.
         * @param str Путь к файлу изображения (например, "textures/wood.png").
         */
        void CreateTexture(std::string str);

        /**
         * Возвращает OpenGL-идентификатор текстуры по имени файла.
         * @param str Имя файла (ключ в MapTextures).
         * @return GLuint идентификатор текстуры, либо 0, если текстура не найдена.
         */
        uint GetTexture(std::string str);   // Примечание: uint подразумевает unsigned int / GLuint.

        /**
         * Возвращает вектор всех загруженных идентификаторов текстур.
         * Первый элемент обычно 0 (заглушка), остальные — реальные текстуры.
         * @return std::vector<uint> с идентификаторами текстур.
         */
        std::vector<uint> GetTextures();

        /// Ассоциативный массив: имя файла -> идентификатор текстуры.
        static std::map<std::string, uint> MapTextures;

        /// Вектор всех идентификаторов текстур (включая нулевую текстуру по умолчанию).
        static std::vector<uint> VecTextures = { 0 };

        /// Временная переменная для хранения последнего созданного идентификатора (используется внутри CreateTexture).
        static uint textureID;
    };
}
