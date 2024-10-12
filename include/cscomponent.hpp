#include "component.hpp"
#include <string>
#include <iostream>

namespace Engine {
    class CSComponent : public Engine::Component {
        using func_t = void* (*)(void*);
        using setparent_t = void (*)(void*, void*);

        public:
            void SetHandler(void* pHandler, void* pComponent);
            void Update() override;
            void InterfaceUpdate() override;
            std::string GetTypeName() const override;
            void SetParent(Renderer::Object* obj) override;
        private:
            void* pHandle;
            void* pComponent;
            
            func_t UpdateFunc;
            setparent_t SetParentFunc;

    };
}
