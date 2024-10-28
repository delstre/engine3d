#include <archive.hpp>
#include <object.hpp>
#include <component.hpp>
#include <model.hpp>
#include <scene.hpp>

IArchive& operator>>(IArchive& ia, std::string& a) {
    size_t len;
    ia >> len;
    a.resize(len);
    ia.get().read(&a[0], len);
    return ia;
}

OArchive& operator<<(OArchive& oa, const std::string& a) {
    size_t len = a.size();
    oa << len;
    oa.get().write(&a[0], a.size());
    return oa;
}

IArchive& operator>>(IArchive& ia, Variable& a) {
    ia >> a.name;
    ia >> a.type_name;

    if (a.type_name == "int") {
        ia >> *static_cast<int*>(a.data);
    } else if (a.type_name == "float") {
        ia >> *static_cast<float*>(a.data);
    } else if (a.type_name == "bool") {
        ia >> *static_cast<bool*>(a.data); 
    } else if (a.type_name == "std::string") {
        ia >> *static_cast<std::string*>(a.data);
    } else if (a.type_name == "glm::vec3") {
        ia >> *static_cast<glm::vec<3, float>*>(a.data);
    }

    return ia;
}

OArchive& operator<<(OArchive& oa, Variable const& a) {
    oa << a.name;
    oa << a.type_name;

    if (a.type_name == "int") {
        oa << *static_cast<int*>(a.data);
    } else if (a.type_name == "float") {
        oa << *static_cast<float*>(a.data);
    } else if (a.type_name == "bool") {
        oa << *static_cast<bool*>(a.data); 
    } else if (a.type_name == "std::string") {
        oa << *static_cast<std::string*>(a.data);
    } else if (a.type_name == "glm::vec3") {
        oa << *static_cast<glm::vec<3, float>*>(a.data);
    }

    return oa;
}

template<typename T>
OArchive& operator<<(OArchive& oa, const std::unique_ptr<T>& ptr) {
    if (ptr) {
        oa << *ptr;
    }
    return oa;
}

template<typename T>
IArchive& operator>>(IArchive& ia, std::unique_ptr<T>& ptr) {
    if (ptr) {
        ia >> *ptr;
    }
    return ia;
}

OArchive& operator<<(OArchive& oa, const glm::vec<3, float>& v) {
    return oa << v.x << v.y << v.z;
}

IArchive& operator>>(IArchive& ia, glm::vec<3, float>& v) {
    return ia >> v.x >> v.y >> v.z;
}

OArchive& operator<<(OArchive& oa, const bool& b) {
    return oa << static_cast<int>(b);
}

IArchive& operator>>(IArchive& ia, bool& b) {
    int val;
    ia >> val;
    b = (val != 0);
    return ia;
}

template<typename T>
IArchive& operator>>(IArchive& ia, std::vector<T>& a) {
    size_t len;
    ia >> len;
    a.resize(len);

    for (size_t i = 0; i < len; ++i) {
        ia >> a[i];
    }

    return ia;
}

template<typename T>
OArchive& operator<<(OArchive& oa, const std::vector<T>& a) {
    size_t len = a.size();
    oa << len;
    for (size_t i = 0; i < len; ++i) {
        oa << a[i];
    }

    return oa;
}

template<typename T>
IArchive& operator>>(IArchive& ia, std::vector<T*>& a) {
    size_t len;
    ia >> len;
    a.resize(len);

    for (size_t i = 0; i < len; ++i) {
        ia >> *a[i];
    }

    return ia;
}

template<typename T>
OArchive& operator<<(OArchive& oa, const std::vector<T*>& a) {
    size_t len = a.size();
    oa << len;
    for (size_t i = 0; i < len; ++i) {
        oa << *a[i];
    }

    return oa;
}

IArchive& operator>>(IArchive& ia, Engine::Model* a) {
    std::string _name;

    ia >> _name;
    a->mesh = Engine::ModelManager::GetModel(_name);

    return ia;
}

OArchive& operator<<(OArchive& oa, Engine::Model* const& a) {
    oa << a->mesh->name;

    return oa;
}

IArchive& operator>>(IArchive& ia, Engine::Component* a) {
    ia >> a->variables;

    if (auto b = dynamic_cast<Engine::Model*>(a)) {
        ia >> b;
    }

    return ia;
}

OArchive& operator<<(OArchive& oa, Engine::Component* const& a) {
    oa << a->variables;

    if (auto b = dynamic_cast<Engine::Model*>(a)) {
        oa << b;
    }

    return oa;
}

IArchive& operator>>(IArchive& ia, Engine::Object& a) {
    ia >> a.variables;

    size_t len;
    ia >> len;
    for (size_t i = 0; i < len; ++i) {
        std::string name;
        ia >> name;
        Engine::Component* component = a.AddComponent(name);
        ia >> component;
    }

    return ia;
}

OArchive& operator<<(OArchive& oa, const Engine::Object& a) {
    oa << a.variables;

    oa << a.components.size();
    for (auto& component : a.components) {
        oa << component->GetTypeName();
        oa << component;
    }

    return oa;
}

IArchive& operator>>(IArchive& ia, Engine::Scene& a) {
    size_t len;
    ia >> len;
    for (size_t i = 0; i < len; ++i) {
        Engine::Object* obj = new Engine::Object();
        ia >> *obj;
        a.AddObject(obj);
    }

    return ia;
}

OArchive& operator<<(OArchive& oa, Engine::Scene const& a) {
    oa << a.objs.size();
    for (auto& obj : a.objs) {
        oa << *obj;
    }

    return oa;
}
