#include "debug.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sys/resource.h>
#include <cstdarg>
#include <iostream>

using namespace Engine;

template<typename... Args>
void Debug::Log(Args&&... args) {
    ((std::cout << std::forward<Args>(args) << " "), ...);
    std::cout << std::endl;
}

GLfloat Debug::GetGPUMemoryUsage() {
    GLint totalMemory = 0;
    GLint freeMemory = 0;

    glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemory);
    glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &freeMemory);

    return (1 - ((float)freeMemory / (float)totalMemory)) * 100;
}

long Debug::GetMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

double Debug::GetFPS() const {
    return fps;
}

void Debug::CounterFPS() {
    frameCount++;

    double currentTime = glfwGetTime();
    double elapsedTime = currentTime - lastTime;

    if (elapsedTime >= 0.01) {
        fps = frameCount / elapsedTime;

        frameCount = 0;
        lastTime = currentTime;
    }
}
