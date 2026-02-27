#pragma once

#include <unordered_map>
#include <functional>
#include <string>

namespace Engine {
    class Object;
    class IComponent;

    /**
     * Пространство имён для управления регистрацией и созданием компонентов.
     * Позволяет динамически создавать компоненты по их строковому имени
     * (необходимо для сериализации и работы фабрик).
     */
    namespace ComponentManager {

        /**
         * Регистрирует конструктор компонента по его строковому имени.
         * @param name Строковое имя типа компонента (например, "Transform").
         * @param constructor Функция, возвращающая новый экземпляр компонента.
         */
        void RegisterComponent(const std::string& name, std::function<IComponent*()> constructor);

        /**
         * Регистрирует все доступные компоненты.
         * Должна вызываться при инициализации приложения.
         * Обычно содержит вызовы макроса REGISTER_COMPONENT для каждого класса компонента.
         */
        void RegisterComponents();

        /**
         * Возвращает словарь зарегистрированных конструкторов компонентов.
         * @return Константная ссылка на unordered_map, где ключ – имя компонента, значение – конструктор.
         */
        const std::unordered_map<std::string, std::function<IComponent*()>>& GetConstructors();

        /**
         * Создаёт экземпляр компонента по его имени.
         * @param name Имя компонента.
         * @return Указатель на новый компонент или nullptr, если имя не зарегистрировано.
         */
        IComponent* CreateComponent(const std::string& name);

        /**
         * Статическая карта зарегистрированных конструкторов.
         * Инициализируется при первом использовании и хранит все известные типы компонентов.
         */
        static std::unordered_map<std::string, std::function<IComponent*()>> constructors;
    };
}

/**
 * Макрос для автоматической регистрации класса компонента.
 * Создаёт временный объект компонента, чтобы получить его демангированное имя типа,
 * регистрирует конструктор, затем удаляет временный объект.
 * 
 * Пример использования:
 * REGISTER_COMPONENT(Transform);
 * 
 * @param NAME Имя класса компонента (без кавычек).
 */
#define REGISTER_COMPONENT(NAME) \
    NAME* NAME##_PTR = new NAME(); \
    Engine::ComponentManager::RegisterComponent(NAME##_PTR->GetTypeName(), []() { return new NAME(); }); \
    delete NAME##_PTR;
