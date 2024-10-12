#pragma once

#include <imgui.h>
#include <object.hpp>

namespace Renderer {
    class Object;
}

namespace Engine {
    class Transform;

    class Component {
        public:
            virtual ~Component() {}
            virtual void Update() = 0;
            virtual void InterfaceUpdate() = 0;
            virtual void SetParent(Renderer::Object* parent);

            template <typename T> T* GetComponent();
            virtual std::string GetTypeName() const;

        protected:
            Renderer::Object* parent = nullptr;
    };
}

