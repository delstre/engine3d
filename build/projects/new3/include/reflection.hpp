#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <memory>
#include <cstring>

#include <fstream>

struct Variable {
    std::string type_name = "";
    const char* name = nullptr;
    void* data = nullptr;

    virtual ~Variable() = default;
    virtual void*& ptr() = 0;

    virtual void serialize(std::ofstream& ofs) const = 0;
    virtual void deserialize(std::ifstream& ifs) = 0;
};

template<typename T>
struct VariableImpl : public Variable {
    VariableImpl(const std::string& type_name, const char* name, void* data) {
        this->data = data;
        this->name = name;
        this->type_name = type_name;
    }

    void*& ptr() override {
        return data;
    }

    void serialize(std::ofstream& ofs) const override {
        size_t name_length = strlen(name);
        ofs.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
        ofs.write(name, name_length);

        if (type_name == "Renderer::Mesh*") {

        } else {
            ofs.write(reinterpret_cast<const char*>(static_cast<T*>(data)), sizeof(T));
        }
    }

    void deserialize(std::ifstream& ifs) override {
        size_t name_length;
        ifs.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
        name = new char[name_length + 1];
        ifs.read(const_cast<char*>(name), name_length);
        const_cast<char*>(name)[name_length] = '\0';

        if (type_name == "Renderer::Mesh*") {

        } else {
            ifs.read(reinterpret_cast<char*>(static_cast<T*>(data)), sizeof(T));
        }
    }
};

#define DECLARE(TYPE, NAME, VALUE) \
TYPE NAME = VALUE; \
variables.push_back(std::make_unique<VariableImpl<TYPE>>(# TYPE, # NAME, & NAME));

#define DECLARE_CLASS_VARIABLE(TYPE, NAME, VALUE) \
    TYPE NAME = VALUE; \

#define REGISTER_CLASS_VARIABLE(TYPE, NAME) \
    variables.push_back(std::make_unique<VariableImpl<TYPE>>(# TYPE, # NAME, & NAME)); \

#define DECLARE_VARIABLES_VECTOR() \
    std::vector<std::unique_ptr<Variable>> variables;

#define DECLARE_CLASS_VARIABLES(...) \
    private: \
    void Init() { \
        __VA_ARGS__; \
    }

#define REGISTER_CLASS_INTERFACE(...)\
    void UpdateInterface() { \
        __VA_ARGS__; \
    }

