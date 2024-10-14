#include "regclass.hpp"
#include "component.hpp"
#include "transform.hpp"
#include "model.hpp"
#include <iostream>

Dictionary<std::type_index, std::string> TypeRegistry::dict;

template<typename Derived, typename Base>
void TypeRegistry::registerType() {
    auto baseIndex = std::type_index(typeid(Base));
    auto derivedName = typeid(Derived).name();


    std::cout << "Registered type: " << derivedName << " as derived from " << typeid(Base).name() << std::endl;
}


template<typename Base>
void TypeRegistry::registerBase() {
    auto baseIndex = std::type_index(typeid(Base));

    //dict["asd"] = "asd";
    //if (typeMap.find(baseIndex) == typeMap.end()) {
        //std::cout << "Registered base type: " << typeid(Base).name() << std::endl;
        ////typeMap[baseIndex] = "";
    //}
}

Dictionary<std::type_index, std::string>& TypeRegistry::getDict() {
    return dict;
}

template void TypeRegistry::registerBase<Engine::Component>();
template void TypeRegistry::registerType<Engine::Transform, Engine::Component>();
template void TypeRegistry::registerType<Renderer::ModelRender, Engine::Component>();
