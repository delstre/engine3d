#pragma once

#include <GL/glew.h>
#include <memory>

namespace Engine {
    class Window;
    class Scene;
    class Project;
    class Object;
};

namespace Renderer {
    class FrameBuffer;

    /**
     * Класс Interface отвечает за отрисовку пользовательского интерфейса с использованием ImGui.
     * Предоставляет методы для отображения различных окон: сцена, инспектор объектов, консоль,
     * отладочная информация и т.д.
     */
    class Interface {
        public:
            /**
             * Конструктор. Инициализирует ImGui, создаёт контекст и настраивает бэкенды для GLFW и OpenGL.
             * @param window Указатель на основное окно приложения (Engine::Window).
             */
            Interface(Engine::Window* window);

            /**
             * Устанавливает текущий проект, с которым работает интерфейс.
             * @param project Указатель на объект проекта.
             */
            void SetProject(Engine::Project* project);

            /**
             * Возвращает текущую сцену из проекта.
             * @return Указатель на объект Scene или nullptr, если проект не установлен.
             */
            Engine::Scene* GetScene() const;

            /**
             * Отображает окно с отладочной информацией: позиция мыши, время рендера, использование памяти и FPS.
             */
            void GetDebugInfo() const;

            /**
             * Отображает окно с настройками конфигурации (например, VSync, отображение интерфейса).
             */
            void GetConfigInfo() const;

            /**
             * Отображает окно сцены (рендер в текстуру). Здесь происходит рендеринг игрового вида,
             * обработка ввода мыши для управления камерой, а также отрисовка Gizmo для выбранных объектов.
             * @param scene Указатель на текущую сцену.
             */
            void GetSceneInfo(Engine::Scene* scene) const;

            /**
             * Отображает окно с информацией о текущей камере и её параметрах (положение, углы, векторы).
             * @param scene Указатель на текущую сцену.
             */
            void GetCameraInfo(Engine::Scene* scene) const;

            /**
             * Отображает окно со списком объектов сцены. Позволяет выбирать объекты, создавать новые
             * (в том числе из префабов) и удалять существующие.
             * @param scene Указатель на текущую сцену.
             */
            void GetObjectsInfo(Engine::Scene* scene);

            /**
             * Отображает Gizmo (манипулятор) для выбранного объекта или вершин. (В текущей реализации объединено с GetSceneInfo.)
             * @param scene Указатель на текущую сцену.
             */
            void GetGizmo(Engine::Scene* scene);

            /**
             * Отображает окно инспектора для выбранного объекта. Показывает его компоненты и их переменные,
             * позволяет редактировать значения, добавлять/удалять компоненты, сохранять объект как префаб.
             * @param scene Указатель на текущую сцену.
             */
            void ObjectInspector(Engine::Scene* scene) const;

            /**
             * Отображает окно управления моделями (ModelManager). Позволяет загружать модели из файлов.
             * @param scene Указатель на текущую сцену.
             */
            void GetModelManager(Engine::Scene* scene) const;

            /**
             * Отображает окно управления ресурсами (ResourceManager). Позволяет загружать текстуры.
             * @param scene Указатель на текущую сцену.
             */
            void GetResourceManager(Engine::Scene* scene) const;

            /**
             * Отображает окно консоли, в котором выводятся логи выполнения команд (например, из Execute).
             * @param scene Указатель на текущую сцену.
             */
            void Console(Engine::Scene* scene) const;

            /**
             * Отображает окно файловой системы проекта (не полностью реализовано).
             */
            void GetFiles() const;

            /**
             * Демонстрационное окно с отладочной информацией, которое всегда находится поверх остальных.
             */
            void ShowExampleAppSimpleOverlay() const;

            /**
             * Основной метод рендеринга интерфейса. Вызывается каждый кадр.
             * Создаёт docking space, меню и все активные окна в зависимости от состояния проекта.
             * @param elapsed_time Время, прошедшее с предыдущего кадра (в микросекундах).
             */
            void Render(GLuint64 elapsed_time);

        private:
            Engine::Project* pProject = nullptr;       ///< Указатель на текущий проект.
            FrameBuffer* pFrameBuffer = nullptr;       ///< Указатель на framebuffer для рендера сцены (не используется напрямую).
            GLuint64 elapsed_time = 0;                  ///< Время кадра (передаётся в Render).
            Engine::Window* pWindow = nullptr;          ///< Указатель на главное окно.
            Engine::Object* pSelectedObject = nullptr;  ///< Указатель на выбранный объект в инспекторе.
    };
}
