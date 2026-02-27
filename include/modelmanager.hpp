#pragma once

#include <map>
#include <string>
#include <filesystem>

namespace Renderer {
    class Mesh;
}

namespace Engine {
    /**
     * Пространство имён ModelManager отвечает за управление загруженными моделями (мешами).
     * Предоставляет функции для добавления, получения, импорта моделей из файлов .obj,
     * а также обновления шейдеров для всех моделей.
     */
    namespace ModelManager {
        /**
         * Устанавливает пути к директориям с моделями и шейдерами.
         * В режиме INTERFACE_DEBUG используются встроенные пути движка,
         * иначе пути формируются относительно корня проекта.
         * @param path Базовый путь к проекту (или любой путь, на основе которого строятся modelsPath и shadersPath).
         */
        void SetPath(const std::string& path);

        /**
         * Добавляет модель (меш) в менеджер под заданным именем.
         * @param name Имя модели (ключ).
         * @param mesh Указатель на объект Mesh.
         */
        void AddModel(const std::string& name, Renderer::Mesh* mesh);

        /**
         * Возвращает указатель на модель по её имени.
         * @param name Имя модели.
         * @return Указатель на Mesh, если модель найдена, иначе поведение не определено (может бросить исключение).
         */
        Renderer::Mesh* GetModel(const std::string& name);

        /**
         * Возвращает словарь всех загруженных моделей.
         * @return std::map, где ключ — имя модели, значение — указатель на Mesh.
         */
        std::map<std::string, Renderer::Mesh*> GetModels();

        /**
         * Импортирует все .obj файлы из директории modelsPath.
         * Для каждого найденного файла вызывается ImportModel.
         */
        void ImportAllModels();

        /**
         * Импортирует одну модель из файла .obj.
         * Создаёт объект Mesh, загружает геометрию, назначает стандартный шейдер и добавляет в менеджер.
         * @param modelPath Полный путь к .obj файлу.
         * @return true, если импорт успешен, иначе false.
         */
        bool ImportModel(const std::string& modelPath);

        /**
         * Перезагружает шейдеры для всех загруженных моделей.
         * Используется при изменении исходных файлов шейдеров.
         */
        void UpdateShaders();

        /// Словарь загруженных моделей (статическая, общая для всех единиц трансляции).
        static std::map<std::string, Renderer::Mesh*> models;

        /// Путь к директории с моделями.
        static std::string modelsPath;
        /// Путь к директории с шейдерами.
        static std::string shadersPath;
    };
}
