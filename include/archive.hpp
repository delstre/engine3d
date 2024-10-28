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

IArchive& operator>>(IArchive& ia, std::string& a);
OArchive& operator<<(OArchive& oa, const std::string& a);

IArchive& operator>>(IArchive& ia, Variable& a);
OArchive& operator<<(OArchive& oa, Variable const& a);

template<typename T>
OArchive& operator<<(OArchive& oa, const std::unique_ptr<T>& ptr);

template<typename T>
IArchive& operator>>(IArchive& ia, std::unique_ptr<T>& ptr);

OArchive& operator<<(OArchive& oa, const glm::vec<3, float>& v);

IArchive& operator>>(IArchive& ia, glm::vec<3, float>& v);

OArchive& operator<<(OArchive& oa, const bool& b);

IArchive& operator>>(IArchive& ia, bool& b);

template<typename T>
IArchive& operator>>(IArchive& ia, std::vector<T>& a);

template<typename T>
OArchive& operator<<(OArchive& oa, const std::vector<T>& a);

template<typename T>
IArchive& operator>>(IArchive& ia, std::vector<T*>& a);

template<typename T>
OArchive& operator<<(OArchive& oa, const std::vector<T*>& a);

IArchive& operator>>(IArchive& ia, Engine::Model* a);
OArchive& operator<<(OArchive& oa, Engine::Model* const& a);

IArchive& operator>>(IArchive& ia, Engine::Component* a);
OArchive& operator<<(OArchive& oa, Engine::Component* const& a);

IArchive& operator>>(IArchive& ia, Engine::Object& a);
OArchive& operator<<(OArchive& oa, const Engine::Object& a);

IArchive& operator>>(IArchive& ia, Engine::Scene& a);
OArchive& operator<<(OArchive& oa, Engine::Scene const& a);
