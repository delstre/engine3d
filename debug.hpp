#pragma once

#include <GL/glew.h>

namespace Engine {
    class Debug {
        public:
            template<typename... Args>
            void Log(Args&&... args);

            GLfloat GetGPUMemoryUsage();
            long GetMemoryUsage();

            double GetFPS() const;

            void CounterFPS();

            static Debug* GetInstance() {
                static Debug* instance;
                return instance;
            }

        private:
            double fps;
            int frameCount = 0;
            double lastTime;

    };
}

