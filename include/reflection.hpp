#pragma once

#include <cassert>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cstring>

/**
 * Базовый класс для описания переменной с поддержкой рефлексии.
 * Хранит имя переменной, имя типа и указатель на данные.
 * Предназначен для наследования с шаблонным параметром типа.
 */
struct Variable {
    std::string type_name = "";   ///< Строковое представление типа (например, "int").
    std::string name = "";        ///< Имя переменной.
    void* data = nullptr;         ///< Указатель на область памяти, где хранится значение.

    virtual ~Variable() = default;
    virtual void*& ptr() = 0;     ///< Чисто виртуальный метод для доступа к указателю на данные (реализуется в шаблонном наследнике).
};

/**
 * Шаблонная реализация Variable для конкретного типа T.
 * Позволяет хранить указатель на переменную типа T и предоставляет реализацию ptr().
 */
template<typename T>
struct VariableImpl : public Variable {
    VariableImpl();
    /**
     * Конструктор, инициализирующий поля базового класса.
     * @param type_name Имя типа (обычно передаётся через макрос #TYPE).
     * @param name Имя переменной.
     * @param data Указатель на переменную типа T.
     */
    VariableImpl(const std::string& type_name, const char* name, void* data);

    void*& ptr() override {
        return data;
    }
};

template<typename T>
VariableImpl<T>::VariableImpl() {}

template<typename T>
VariableImpl<T>::VariableImpl(const std::string& type_name, const char* name, void* data) {
    this->data = data;
    this->name = name;
    this->type_name = type_name;
}

/**
 * Макрос для объявления переменной с автоматической регистрацией в векторе variables.
 * Создаёт переменную с заданным типом, именем и начальным значением,
 * затем добавляет в вектор умный указатель на VariableImpl, описывающий эту переменную.
 * 
 * Пример использования: DECLARE(int, myVar, 42);
 */
#define DECLARE(TYPE, NAME, VALUE) \
TYPE NAME = VALUE; \
variables.push_back(std::make_unique<VariableImpl<TYPE>>(# TYPE, # NAME, & NAME));

/**
 * Макрос для объявления переменной внутри класса без автоматической регистрации.
 * Используется совместно с DECLARE_CLASS_VARIABLES для последующей регистрации в Init().
 * 
 * Пример: DECLARE_CLASS_VARIABLE(int, m_health, 100);
 */
#define DECLARE_CLASS_VARIABLE(TYPE, NAME, VALUE) \
    TYPE NAME = VALUE; \

/**
 * Макрос для регистрации ранее объявленной переменной класса в векторе variables.
 * Обычно вызывается внутри метода Init(), который определён макросом DECLARE_CLASS_VARIABLES.
 * 
 * Пример: REGISTER_CLASS_VARIABLE(int, m_health);
 */
#define REGISTER_CLASS_VARIABLE(TYPE, NAME) \
    variables.push_back(std::make_unique<VariableImpl<TYPE>>(# TYPE, # NAME, & NAME)); \

/**
 * Макрос для объявления вектора умных указателей на Variable внутри класса.
 * Этот вектор будет хранить информацию обо всех переменных, зарегистрированных
 * через REGISTER_CLASS_VARIABLE.
 */
#define DECLARE_VARIABLES_VECTOR() \
    std::vector<std::unique_ptr<Variable>> variables;

/**
 * Макрос для создания приватного метода Init(), который регистрирует переменные класса.
 * В теле метода через запятую перечисляются вызовы REGISTER_CLASS_VARIABLE для каждой переменной.
 * Этот метод должен вызываться в конструкторе класса для заполнения вектора variables.
 * 
 * Пример:
 * DECLARE_CLASS_VARIABLES(
 *     REGISTER_CLASS_VARIABLE(int, m_health);
 *     REGISTER_CLASS_VARIABLE(std::string, m_name);
 * )
 */
#define DECLARE_CLASS_VARIABLES(...) \
    private: \
    void Init() { \
        __VA_ARGS__; \
    }
