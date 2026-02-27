#pragma once

#include <structs.hpp>

#include <string>
#include <vector>
#include <functional>

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include <reflection.hpp>

namespace Engine {
    class IComponent;
    class Component;
}

namespace Engine {
    class IComponent;
    class Component;

    /**
     * Базовый класс для всех объектов сцены.
     * Объект может содержать компоненты (Component), обеспечивающие его поведение и внешний вид.
     * Поддерживает рефлексию переменных и сериализацию.
     */
    class Object {
        public:
            /**
             * Конструктор по умолчанию. Инициализирует переменные класса через макросы рефлексии.
             */
            Object();

            /**
             * Конструктор с заданием имени объекта.
             * @param name Имя объекта.
             */
            Object(const std::string& name);

            /**
             * Добавляет компонент указанного типа к объекту.
             * Компонент создаётся через ComponentManager по строковому имени типа.
             * @param name Строковое имя типа компонента (например, "Transform").
             * @return Указатель на созданный компонент или nullptr, если тип не зарегистрирован.
             */
            Component* AddComponent(const std::string& name);

            /**
             * Добавляет уже существующий компонент к объекту.
             * Компонент не должен принадлежать другому объекту.
             * @param component Ссылка на компонент.
             */
            void AddComponent(Engine::Component& component);

            /**
             * Удаляет первый компонент с заданным именем типа.
             * @param name Строковое имя типа компонента.
             */
            void RemoveComponent(const std::string& name);

            /**
             * Удаляет конкретный компонент по указателю.
             * @param component Указатель на компонент.
             */
            void RemoveComponent(Engine::Component* component);

            /**
             * Возвращает первый компонент типа T, прикреплённый к объекту.
             * @tparam T Тип искомого компонента (должен быть наследником Component).
             * @return Указатель на компонент или nullptr, если компонент не найден.
             */
            template <typename T>
            T* GetComponent();

            /**
             * Возвращает первый компонент с указанным именем типа, приведённый к типу T.
             * @tparam T Тип, к которому нужно привести найденный компонент.
             * @param name Строковое имя типа компонента.
             * @return Указатель на компонент или nullptr.
             */
            template <typename T>
            T* GetComponent(const std::string& name);

            /**
             * Возвращает ссылку на вектор всех компонентов объекта.
             * @return Ссылка на std::vector<Engine::Component*>.
             */
            std::vector<Engine::Component*>& GetComponents();
            
            /**
             * Проверяет, пересекает ли луч ограничивающий параллелепипед (AABB) модели объекта.
             * Работает только если объект содержит компонент Model.
             * @param origin Начало луча в мировых координатах.
             * @param direction Направление луча (нормализованное).
             * @return true, если есть пересечение с AABB модели.
             */
            bool IsHitByRay(const glm::vec3& origin, const glm::vec3& direction);

            /**
             * Устанавливает параметры окружения (освещение, фон) для объекта.
             * @param env Структура Envy с настройками окружения.
             */
            void SetENV(const Renderer::Envy& env);

            /**
             * Сохраняет объект как префаб в бинарный файл.
             * @param path Путь к директории, в которой будет создан файл "myobject.bin".
             */
            void SaveAsPrefab(const std::string& path);

            /**
             * Загружает объект из бинарного файла префаба.
             * Заменяет текущее состояние объекта загруженными данными.
             * @param path Полный путь к файлу префаба.
             */
            void LoadFromPrefab(const std::string& path);

            /**
             * Обновляет состояние объекта, вызывая UpdateComponent() для всех его компонентов.
             * Если установлен UpdateOverride, он вызывается после обновления компонентов.
             */
            void Update();

            /// Функция обратного вызова, которая может переопределить стандартное обновление.
            std::function<void()> UpdateOverride;

            /// Параметры окружения (освещение, фон), применяемые к объекту при рендеринге.
            Renderer::Envy env;

            /// Вектор указателей на все компоненты объекта.
            std::vector<Engine::Component*> components;

            // Макросы для поддержки рефлексии и сериализации переменных класса
            DECLARE_CLASS_VARIABLE(std::string, name, "MyObject");
            DECLARE_VARIABLES_VECTOR()
            DECLARE_CLASS_VARIABLES(
                REGISTER_CLASS_VARIABLE(std::string, name);
            )
    };
}
