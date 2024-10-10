#include "window.hpp"

#include <iostream>
#include <GL/glew.h>
#include <IL/il.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

using namespace Engine;

static void staticWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
    window->Resize(width, height);
}

static void staticCursorPosCallback(GLFWwindow* pWindow, double xpos, double ypos) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
    window->SetCursorPos(xpos, ypos);
}


void Window::Resize(int width, int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);        
}

void Window::SetCursorPos(double xpos, double ypos) {}

void Window::Init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    ilInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    pWindow = glfwCreateWindow(width, height, window_name, NULL, NULL);
    if (!pWindow) {
        std::cerr << "Window failed" << std::endl;
        glfwTerminate();
        return;
    } 

    glfwMakeContextCurrent(pWindow);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    // callbacks
    
    glfwSetWindowUserPointer(pWindow, this);
    glfwSetWindowSizeCallback(pWindow, staticWindowSizeCallback);

    //glfwSetCursorPosCallback(window, mousePosCallback);
    //glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    //glfwSetMouseButtonCallback(window, mouse_button_callback);

    //Renderer::Camera camManager(window);
    //camManager.yaw = 0.0f;
    //camManager.pitch = 0.0f;
    //camManager.speed = 10.5f;
    //camManager.sensitivity = 0.2f;
    //camManager.position = glm::vec3(0, 0, 0);


    //glfwSetWindowUserPointer(window, &camManager);

    //Renderer::ResourceManager resManager;
    //Renderer::ModelManager models;

    //std::vector<Renderer::Object*> objs;

    //Engine::Controller controller(window);

    //controller.AddCallback(GLFW_KEY_F2, true, false, []() {
        //Config::InterfaceDebugActive = !Config::InterfaceDebugActive;
    //});

    //Renderer::Interface interface(window);
    //interface.AddCameraInfo(&camManager);
    //interface.AddObjectsInfo(&objs);
    //interface.SetModelManager(&models);
    //interface.SetResourceManager(&resManager);

    pInterface = new Renderer::Interface(this);
    pComponentManager = new Engine::ComponentManager();
    pComponentManager->RegisterComponents();

    Engine::Scene* pScene = new Engine::Scene();
    pScene->Init(pWindow);
    SceneInit(pScene);

    //Renderer::FrameBuffer fbo(g_windowSize.x, g_windowSize.y);
    //interface.SetSceneInfo(&fbo);

    // Controller init
    // For keyboard and mouse

    // Scene init 
    // In scene (object manager, resource manager, model manager, etc)
    // SceneInit();

    Render();

    return;
}

void Window::SceneInit(Scene* scene) {
    if (scene != nullptr) {
        pScene = scene;
        pScene->SetFrameSize(width, height);
        pScene->SetComponentManager(pComponentManager);
    }
}

void Window::Render() {
    while (!glfwWindowShouldClose(pWindow)) {
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLuint query;
        glGenQueries(1, &query);

        if (pScene != nullptr) {
            glBeginQuery(GL_TIME_ELAPSED, query);
            pScene->Render();
            glEndQuery(GL_TIME_ELAPSED);
        }

        GLuint64 elapsed_time;
        glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);

        if (pInterface != nullptr)
            pInterface->Render(pScene, elapsed_time);

        glfwPollEvents();
        glfwSwapBuffers(pWindow);
    }
}

GLFWwindow* Window::GetWindow() {
    return pWindow;
}
