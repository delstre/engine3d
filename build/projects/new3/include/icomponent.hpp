#pragma once

namespace Engine {
    class IComponent {
        public:
            virtual ~IComponent() {}
            virtual void UpdateComponent() = 0;
            virtual void Start() = 0;
            virtual void End() = 0;
    };
}

#define REGCLASS(ClassName) \
extern "C" ClassName* Create() { return new ClassName(); } \
extern "C" void Destroy(ClassName* Instance) { delete Instance; }
