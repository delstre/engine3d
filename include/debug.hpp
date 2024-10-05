#pragma once

#include <GL/glew.h>

#include <memory>

namespace Engine {
    class Debug {
        public:
            template<typename... Args>
            void Log(Args&&... args);

            GLfloat GetGPUMemoryUsage();
            long GetMemoryUsage();

            double GetFPS() const;

            void CounterFPS();

            // Problem: Debug* == nullptr?
            static std::shared_ptr<Debug> GetInstance() {
                static std::shared_ptr<Debug> instance(new Debug());
                return instance;
            }

        private:
            double fps;
            int frameCount = 0;
            double lastTime;
    };
}

