#include <GL/glew.h>
#include <window.hpp>
#include <componentmanager.hpp>
#include <scene.hpp>

#include <IL/il.h>

#include <iostream>

using namespace Engine;

static void staticWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
    window->Resize(width, height);
}

static void staticCursorPosCallback(GLFWwindow* pWindow, double xpos, double ypos) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(pWindow));
    window->SetCursorPos(xpos, ypos);
}
  
Window::Window(int w, int h, const char *t) : width(w), height(h), window_name(t) { }

Window::~Window() 
{ 
    glfwTerminate(); 
}

void Window::Resize(int width, int height) {
    this->width = width;
    this->height = height;
    glViewport(0, 0, width, height);        
}

void Window::SetCursorPos(double xpos, double ypos) {}

void Window::Init(int argc, char *argv[]) {
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

    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK) {
        if (glewErr == GLEW_ERROR_NO_GLX_DISPLAY) {
            std::cerr << "glewInit() returned GLEW_ERROR_NO_GLX_DISPLAY" << std::endl;
        } else {
            std::cerr << "failed to initialize GLEW:\n" << std::endl; 
            exit(EXIT_FAILURE);
        }
    }

    glEnable(GL_DEPTH_TEST);

    glfwSetWindowUserPointer(pWindow, this);
    glfwSetWindowSizeCallback(pWindow, staticWindowSizeCallback);

    ComponentManager::RegisterComponents();
    
    #ifdef INTERFACE_DEBUG

    pInterface = new Renderer::Interface(this);

    if (argc == 2)
    {
        pProject = new Engine::Project(this);
        pProject->Load(argv[argc-1]);
        pInterface->SetProject(pProject);
    }

    #else

    // ?
    //pProject->Load();
    //pScene = pProject->GetScene();

    #endif
}

void Window::SceneInit(Scene* scene) {
    if (scene != nullptr) {
        scene->Init(GetWindow());
        scene->SetFrameSize(width, height);
    }
}

void Window::Render() {
    while (!glfwWindowShouldClose(pWindow)) {
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        #ifdef INTERFACE_DEBUG
        GLuint query;
        glGenQueries(1, &query);

        pScene = pInterface->GetScene();
        if (pScene != nullptr) {
            glBeginQuery(GL_TIME_ELAPSED, query);
            pScene->Render();
            glEndQuery(GL_TIME_ELAPSED);
        }

        GLuint64 elapsed_time;
        glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);

        if (pInterface != nullptr)
            pInterface->Render(elapsed_time);
        #else
        if (pScene != NULL) {
            pScene->Render();
        }
        #endif

        glfwPollEvents();
        glfwSwapBuffers(pWindow);
    }
}

GLFWwindow* Window::GetWindow() {
    return pWindow;
}

float Window::GetAspectRatio() {
    return (float)width / (float)height;
}

Scene* Window::GetScene() const {
    return pScene;
}
