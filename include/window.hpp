#pragma once

#ifdef INTERFACE_DEBUG
#include "interface.hpp"
#endif

#include <project.hpp>
#include <GLFW/glfw3.h>

namespace Renderer {
    class Interface;
};

namespace Engine {
    class Scene;
    class Project;

    /**
     * Класс Window инкапсулирует создание и управление окном GLFW,
     * инициализацию OpenGL, а также главный цикл рендеринга.
     * В режиме INTERFACE_DEBUG добавляет отрисовку пользовательского интерфейса.
     */
    class Window {
        public:
            /**
             * Конструктор окна.
             * @param w Ширина окна в пикселях.
             * @param h Высота окна в пикселях.
             * @param t Заголовок окна.
             */
            Window(int w, int h, const char *t);

            /**
             * Деструктор. Завершает работу GLFW.
             */
            ~Window();

            /**
             * Инициализирует окно: создаёт контекст OpenGL, настраивает GLEW,
             * регистрирует компоненты, создаёт интерфейс (если INTERFACE_DEBUG) и
             * загружает проект, если передан аргумент командной строки.
             * @param argc Количество аргументов командной строки.
             * @param argv Массив аргументов командной строки.
             */
            void Init(int argc, char *argv[]);

            /**
             * Изменяет размер окна и области просмотра OpenGL.
             * @param width Новая ширина.
             * @param height Новая высота.
             */
            void Resize(int width, int height);

            /**
             * Устанавливает позицию курсора в окне (заглушка, не реализована).
             * @param xpos Новая координата X.
             * @param ypos Новая координата Y.
             */
            void SetCursorPos(double xpos, double ypos);

            /**
             * Инициализирует сцену для окна: вызывает scene->Init() и устанавливает размер фреймбуфера.
             * @param scene Указатель на сцену, которую нужно привязать к окну.
             */
            void SceneInit(Scene *scene);

            /**
             * Главный цикл рендеринга. Очищает буферы, вызывает рендеринг сцены,
             * а в режиме INTERFACE_DEBUG также рендерит интерфейс.
             */
            void Render();

            /**
             * Возвращает указатель на внутреннее окно GLFW.
             * @return Указатель GLFWwindow*.
             */
            GLFWwindow* GetWindow();

            /**
             * Возвращает соотношение сторон окна (ширина / высота).
             * @return Значение aspect ratio.
             */
            float GetAspectRatio();

            /**
             * Возвращает указатель на текущую сцену, связанную с окном.
             * @return Указатель на объект Scene.
             */
            Scene* GetScene() const;

            /// Текущая ширина окна.
            int width;
            /// Текущая высота окна.
            int height;

        private:
            const char* window_name;          ///< Заголовок окна.

            GLFWwindow* pWindow = nullptr;    ///< Указатель на GLFW окно.
            Scene* pScene = nullptr;           ///< Указатель на текущую сцену.
            Project* pProject = nullptr;       ///< Указатель на текущий проект (используется в режиме INTERFACE_DEBUG).

            #ifdef INTERFACE_DEBUG
            Renderer::Interface* pInterface = nullptr; ///< Указатель на объект интерфейса (только в режиме отладки).
            #endif
    };
}
