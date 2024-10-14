#pragma once

#include <map>
#include <typeindex>
#include <vector>
#include <string>
#include "dictionary.hpp"

// Macro to register a type with its description
#define REGISTER_TYPE(Derived, Base) \
    class Derived##Register { \
    public: \
        Derived##Register() { \
            TypeRegistry::registerType<Derived, Base>(); \
        } \
    }; \
    static Derived##Register global_##Derived##Register; // Исправлено здесь

#define REGISTER_BASE(Base) \
    class Derived##Register { \
    public: \
        Derived##Register() { \
            TypeRegistry::registerBase<Base>(); \
        } \
    }; \
    static Derived##Register global_##Derived##Register; // Исправлено здесь

// Class for storing type information
class TypeRegistry {
public:
    // Template function to register a type
    template<typename Derived, typename Base>
    static void registerType();

    template<typename Base>
    static void registerBase();

    static Dictionary<std::type_index, std::string>& getDict();

private:
    static Dictionary<std::type_index, std::string> dict;
};
