#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <reflection.hpp>
#include <serialize.hpp>

#include <glm/glm.hpp>

namespace Engine {
    class Model;
    class Component;
    class Scene;
    class Object;
}

/**
 * Десериализует std::string из архива.
 * Сначала читается размер строки, затем выделяется память и читаются данные.
 */
IArchive& operator>>(IArchive& ia, std::string& a);

/**
 * Сериализует std::string в архив.
 * Сначала записывается размер строки, затем её содержимое.
 */
OArchive& operator<<(OArchive& oa, const std::string& a);

/**
 * Десериализует объект Variable из архива.
 * Читает имя, имя типа и значение переменной в зависимости от типа.
 */
IArchive& operator>>(IArchive& ia, Variable& a);

/**
 * Сериализует объект Variable в архив.
 * Записывает имя, имя типа и значение переменной в зависимости от типа.
 */
OArchive& operator<<(OArchive& oa, Variable const& a);

/**
 * Сериализует std::unique_ptr в архив.
 * Записывает флаг существования (true, если указатель не nullptr) и,
 * если указатель действителен, сериализует объект, на который он указывает.
 */
template<typename T>
OArchive& operator<<(OArchive& oa, const std::unique_ptr<T>& ptr);

/**
 * Десериализует std::unique_ptr из архива.
 * Читает флаг существования; если true, создаёт новый объект T и десериализует его.
 */
template<typename T>
IArchive& operator>>(IArchive& ia, std::unique_ptr<T>& ptr);

/**
 * Сериализует glm::vec3 в архив (записывает три компоненты: x, y, z).
 */
OArchive& operator<<(OArchive& oa, const glm::vec<3, float>& v);

/**
 * Десериализует glm::vec3 из архива (читает три компоненты: x, y, z).
 */
IArchive& operator>>(IArchive& ia, glm::vec<3, float>& v);

/**
 * Сериализует bool в архив как int (0 или 1).
 */
OArchive& operator<<(OArchive& oa, const bool& b);

/**
 * Десериализует bool из архива, читая int и преобразуя его в bool.
 */
IArchive& operator>>(IArchive& ia, bool& b);

/**
 * Десериализует std::vector из архива.
 * Читает размер вектора, изменяет размер вектора, затем десериализует каждый элемент,
 * предполагая, что элементы уже выделены и имеют операторы сериализации.
 */
template<typename T>
IArchive& operator>>(IArchive& ia, std::vector<T>& a);

/**
 * Сериализует std::vector в архив.
 * Записывает размер вектора, затем каждый элемент (разыменовывая его, если это необходимо).
 */
template<typename T>
OArchive& operator<<(OArchive& oa, const std::vector<T>& a);

/**
 * Десериализует std::vector сырых указателей из архива.
 * Читает размер, изменяет размер вектора и десериализует каждый объект по указателю.
 * (Реализация должна обеспечивать корректное выделение памяти.)
 */
template<typename T>
IArchive& operator>>(IArchive& ia, std::vector<T*>& a);

/**
 * Сериализует std::vector сырых указателей в архив.
 * Записывает размер вектора, затем каждый указатель (обычно сериализуя сам объект).
 */
template<typename T>
OArchive& operator<<(OArchive& oa, const std::vector<T*>& a);

/**
 * Десериализует указатель на Model из архива.
 * Читает имя меша и присваивает указателю соответствующий меш из ModelManager.
 * Предполагается, что указатель уже указывает на существующий объект Model.
 */
IArchive& operator>>(IArchive& ia, Engine::Model* a);

/**
 * Сериализует указатель на Model в архив.
 * Записывает имя меша, хранящегося в ModelManager.
 */
OArchive& operator<<(OArchive& oa, Engine::Model* const& a);

/**
 * Десериализует указатель на Component из архива.
 * Читает переменные компонента; если компонент является моделью, дополнительно
 * десериализует его как модель.
 */
IArchive& operator>>(IArchive& ia, Engine::Component* a);

/**
 * Сериализует указатель на Component в архив.
 * Записывает переменные компонента; если компонент является моделью,
 * дополнительно сериализует его как модель.
 */
OArchive& operator<<(OArchive& oa, Engine::Component* const& a);

/**
 * Десериализует объект Object из архива.
 * Читает переменные объекта, затем список компонентов, создавая компоненты
 * по имени типа и десериализуя их.
 */
IArchive& operator>>(IArchive& ia, Engine::Object& a);

/**
 * Сериализует объект Object в архив.
 * Записывает переменные объекта, количество компонентов и для каждого компонента
 * его тип и содержимое.
 */
OArchive& operator<<(OArchive& oa, const Engine::Object& a);

/**
 * Десериализует сцену Scene из архива.
 * Читает количество объектов, создаёт новые объекты, десериализует их
 * и добавляет в сцену.
 */
IArchive& operator>>(IArchive& ia, Engine::Scene& a);

/**
 * Сериализует сцену Scene в архив.
 * Записывает количество объектов и каждый объект сцены.
 */
OArchive& operator<<(OArchive& oa, Engine::Scene const& a);
