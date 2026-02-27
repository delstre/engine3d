#pragma once

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include <mutex>
#include <vector>
#include <string>
#include <math_utils.hpp>

namespace Renderer {
    class FrameBuffer;
}

/**
 * Режим выделения в редакторе:
 * - MODE_OBJECT: выделение целых объектов.
 * - MODE_MESH: выделение треугольников меша.
 * - MODE_VERTEX: выделение вершин.
 */
typedef enum { MODE_OBJECT, MODE_MESH, MODE_VERTEX } selectModeEn;

namespace Engine {
    class Object;
    class Camera;
    class WindowController;

    /**
     * Класс Scene представляет сцену, содержащую объекты, камеры и управляющую
     * рендерингом, обновлением и взаимодействием с пользователем.
     */
    class Scene {
        public:
            /**
             * Инициализирует сцену: создаёт контроллер ввода, фреймбуфер, включает тест глубины.
             * @param pWindow Указатель на GLFW окно, с которым связана сцена.
             */
            void Init(GLFWwindow* pWindow);

            /**
             * Устанавливает путь к директории, ассоциированной со сценой (например, для ресурсов).
             * @param path Строка с путём.
             */
            void SetPath(std::string path);

            /**
             * Добавляет объект в сцену. Потокобезопасно (использует мьютекс).
             * @param obj Указатель на объект для добавления.
             */
            void AddObject(Engine::Object* obj);

            /**
             * Удаляет объект из сцены по указателю. Потокобезопасно.
             * @param id Указатель на удаляемый объект.
             */
            void DeleteObject(Engine::Object* id);

            /**
             * Создаёт и добавляет камеру в сцену.
             * @param position Начальная позиция камеры.
             * @return Указатель на созданную камеру.
             */
            Engine::Camera* AddCamera(glm::vec3 position);

            /**
             * Устанавливает размер области рендеринга (framebuffer) и при необходимости создаёт/изменяет его.
             * @param width  Новая ширина.
             * @param height Новая высота.
             */
            void SetFrameSize(int width, int height);

            /**
             * Выполняет raycast по всем объектам сцены.
             * Проверяет пересечение с треугольниками моделей, используя мировые координаты.
             * @param worldRay Луч в мировых координатах.
             * @return Структура Intersection с информацией о ближайшем пересечении.
             */
            Intersection Raycast(const Ray& worldRay);

            /**
             * Возвращает активную камеру сцены. Если активная камера не установлена,
             * создаёт камеру по умолчанию в начале координат.
             * @return Указатель на активную камеру.
             */
            Engine::Camera* GetActiveCamera();

            /**
             * Возвращает указатель на фреймбуфер, в который выполняется рендеринг сцены.
             * @return Указатель на FrameBuffer.
             */
            Renderer::FrameBuffer* GetFrameBuffer();

            /**
             * Возвращает вектор идентификаторов всех загруженных текстур (из ResourceManager).
             * @return std::vector<GLuint> с идентификаторами текстур.
             */
            std::vector<GLuint> GetTextures();

            /**
             * Возвращает вектор всех объектов сцены.
             * @return std::vector<Engine::Object*> с объектами.
             */
            std::vector<Engine::Object*> GetObjects();

            /**
             * Возвращает указатель на GLFW окно, связанное со сценой.
             * @return Указатель на GLFWwindow.
             */
            GLFWwindow* GetWindow();

            /**
             * Возвращает соотношение сторон фреймбуфера (ширина / высота).
             * @return Значение aspect ratio.
             */
            float GetAspectRatio();

            /**
             * Выполняет рендеринг сцены:
             * - обрабатывает ввод (контроллер),
             * - обновляет камеру,
             * - вызывает Update() для всех объектов,
             * - при необходимости переключает фреймбуфер.
             * Должен вызываться каждый кадр.
             */
            void Render();

            /// Текущая ширина области рендеринга.
            int width;
            /// Текущая высота области рендеринга.
            int height;

            /// Вектор всех объектов сцены.
            std::vector<Engine::Object*> objs;

            /// Указатель на объект, выделенный в редакторе.
            Engine::Object *selectedObject = nullptr;

            /// Указатель на меш, выделенный в редакторе (если режим MODE_MESH).
            Renderer::Mesh *selectedMesh = nullptr;

            /// Индекс выделенного треугольника (для режима MODE_MESH).
            int triangleIndex = -1;

            /// Вектор индексов выделенных вершин (для режима MODE_VERTEX).
            std::vector<int> selectedVertexIndices;

            /// Текущий режим выделения (объект, треугольник, вершина).
            selectModeEn selectMode;

        private:
            mutable std::mutex scene_mutex;          ///< Мьютекс для потокобезопасного доступа к объектам.
            std::string path;                         ///< Путь к ресурсам сцены.
            std::vector<std::string> comps;           ///< Не используется в текущей реализации.

            Renderer::FrameBuffer* pFbo = nullptr;    ///< Указатель на фреймбуфер для рендеринга.
            Engine::Camera* pActiveCamera = nullptr;  ///< Активная камера сцены.
            Engine::WindowController* pController = nullptr; ///< Контроллер ввода.
            GLFWwindow* pWindow = nullptr;            ///< GLFW окно, ассоциированное со сценой.
            
            std::vector<Engine::Object*> render;      ///< Вектор для сортировки объектов при рендеринге (не используется).

            bool initialized = false;                  ///< Флаг инициализации сцены.
    };
}
