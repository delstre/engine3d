#pragma once

#include <GL/glew.h>

#include <memory>

namespace Engine {
    namespace Debug {
        template<typename... Args>
        void Log(Args&&... args);

        GLfloat GetGPUMemoryUsage();
        long GetMemoryUsage();

        double GetFPS();

        void CounterFPS();

        static double fps;
        static int frameCount = 0;
        static double lastTime;
    };
}

