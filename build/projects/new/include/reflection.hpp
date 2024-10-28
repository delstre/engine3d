#pragma once

#include <cassert>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cstring>

struct Variable {
    std::string type_name = "";
    const char* name = nullptr;
    void* data = nullptr;

    virtual ~Variable() = default;
    virtual void*& ptr() = 0;
};

template<typename T>
struct VariableImpl : public Variable {
    VariableImpl(const std::string& type_name, const char* name, void* data);

    void*& ptr() {
        return data;
    }
};

template<typename T>
VariableImpl<T>::VariableImpl(const std::string& type_name, const char* name, void* data) {
    this->data = data;
    this->name = name;
    this->type_name = type_name;
}

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

