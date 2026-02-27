#pragma once

#include <icomponent.hpp>
#include <object.hpp>

#include <cassert>

namespace Engine {
    class Object;

    /**
     * Базовый класс для всех компонентов, которые можно прикреплять к объектам сцены.
     * Предоставляет жизненный цикл (Start, Update, End), управление доступом и рефлексию.
     */
    class Component : public IComponent {
        public:
            /**
             * Виртуальный деструктор по умолчанию.
             */
            virtual ~Component() = default;
            
            /**
             * Конструктор по умолчанию. Вызывает инициализацию переменных класса.
             */
            Component();

            /**
             * Устанавливает родительский объект для компонента.
             * Родитель может быть установлен только один раз (если текущий parent == nullptr).
             * @param parent Указатель на объект-владелец.
             */
            void SetParent(Engine::Object* parent);

            /**
             * Возвращает указатель на родительский объект.
             * @return Указатель на Object, которому принадлежит компонент.
             */
            Engine::Object* GetParent() const;

            /**
             * Обновляет интерфейс компонента (например, для редактора).
             * По умолчанию ничего не делает, может быть переопределён.
             */
            void UpdateInterface();

            /**
             * Обновляет состояние компонента, если он включён.
             * Вызывает виртуальный метод Update().
             */
            void UpdateComponent() override;

            /**
             * Вызывается при активации компонента или при старте сцены.
             * Если компонент включён, вызывает одноимённый виртуальный метод.
             */
            virtual void Start() override;

            /**
             * Вызывается при деактивации компонента или при завершении работы.
             * Если компонент включён, вызывает одноимённый виртуальный метод.
             */
            virtual void End() override;

            /**
             * Виртуальный метод, который можно переопределить в производных классах
             * для реализации пользовательской логики, обновляемой каждый кадр.
             * Вызывается из UpdateComponent().
             */
            virtual void Update();

            /**
             * Включает или отключает компонент.
             * @param enable true – компонент активен, false – неактивен.
             */
            void SetEnable(bool enable);

            /**
             * Возвращает первый компонент типа T, прикреплённый к родительскому объекту.
             * @tparam T Тип искомого компонента (должен быть наследником Component).
             * @return Указатель на компонент или nullptr, если компонент не найден или родитель не установлен.
             */
            template<typename T>
            T* GetComponent();

            /**
             * Возвращает демангированное имя типа компонента (для рефлексии).
             * @return Строка с именем класса.
             */
            std::string GetTypeName() const;

            // Макросы для поддержки рефлексии и сериализации
            DECLARE_CLASS_VARIABLE(Engine::Object*, parent, nullptr);
            DECLARE_CLASS_VARIABLE(bool, isEnabled, true);

            DECLARE_VARIABLES_VECTOR();

            DECLARE_CLASS_VARIABLES(
                REGISTER_CLASS_VARIABLE(Engine::Object*, parent);
                REGISTER_CLASS_VARIABLE(bool, isEnabled);
            )
    };
}
