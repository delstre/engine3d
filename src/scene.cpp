#include <scene.hpp>
#include <icomponent.hpp>
#include <modelmanager.hpp>
#include <controller.hpp>
#include <framebuffer.hpp>
#include <camera.hpp>
#include <resourcemanager.hpp>

#include <csignal>
#include <typeinfo>
#include <iostream>

using namespace Engine;

void Scene::Init(GLFWwindow* pWindow) {
    pController = new Engine::WindowController(pWindow); 

    int wid, hei;
    glfwGetWindowSize(pWindow, &wid, &hei);
   //pController->AddCallbackMouse(GLFW_MOUSE_BUTTON_LEFT, false, [this, &wid, &hei]() {
        //static bool firstMouse = true;

        //double xpos, ypos;
        //glfwGetCursorPos(this->pWindow, &xpos, &ypos);

        //float xoffset = (wid / 2) - xpos;
        //float yoffset = (hei / 2 - ypos); // инвертируем ось Y, так как движемся по экрану

        //glfwSetCursorPos(this->pWindow, wid / 2, hei / 2);

        //this->GetActiveCamera()->ProcessMouseMovement(-xoffset, yoffset);
    //});

    pController->AddCallback(GLFW_KEY_F2, true, [this]() {});

    // project initialize
    // importing all models from dir?
    ModelManager::SetPath(this->path);
    ModelManager::ImportModel("models/cube.obj");
    
    pFbo = new Renderer::FrameBuffer(wid, hei);
    this->pWindow = pWindow;
    glEnable(GL_DEPTH_TEST);

    initialized = true;
}

void Scene::SetPath(std::string path) {
    this->path = path;
}

Engine::Camera* Scene::AddCamera(glm::vec3 position) {
    if (pWindow == nullptr) {
        return nullptr;
    }

    Engine::Camera* cam = new Engine::Camera(pWindow);
    cam->yaw = 0.0f;
    cam->pitch = 0.0f;
    cam->speed = 10.5f;
    cam->sensitivity = 0.2f;
    cam->position = position;

    return cam;
}

void Scene::AddObject(Engine::Object* obj) {
    std::cout << typeid(*obj).name() << " Created" << std::endl;

    objs.push_back(obj);
}

void Scene::DeleteObject(Engine::Object* id) {
    for (int i = 0; i < objs.size(); i++) {
        if (objs[i] == id) {
            objs.erase(objs.begin() + i);
            break;
        }
    }
}

Engine::Camera* Scene::GetActiveCamera() {
    if (pActiveCamera == nullptr) {
        pActiveCamera = AddCamera(glm::vec3(0, 0, 0));
    }

    return pActiveCamera;
}

std::vector<GLuint> Scene::GetTextures() {
    return Engine::ResourceManager::GetTextures();
}

std::vector<Engine::Object*> Scene::GetObjects() {
    return objs;
}

// bind and unbind for final application need to remove!

float lastFrame = 0;
float deltaTime = 0;
void Scene::Render() {
    if (!initialized)
        return;

    pController->ProcessInput();

    #ifdef INTERFACE_DEBUG
    pFbo->Bind();
    #endif

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Engine::Camera* cam = GetActiveCamera();
    cam->Think();

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    cam->Control(pWindow, deltaTime);

    Renderer::Envy env;
    env.viewpos = cam->position;
    env.viewdir = cam->front;
    env.mvp = cam->mvp;
    for (int i = 0; i < objs.size(); i++) {
        objs[i]->SetENV(env);
        objs[i]->Update();
    }

    #ifdef INTERFACE_DEBUG
    pFbo->Unbind();
    #endif
}

void Scene::SetFrameSize(int width, int height) {
    if (pFbo == nullptr) {
        pFbo = new Renderer::FrameBuffer(width, height);
    }

    pFbo->RescaleFrameBuffer(width, height);
}

Renderer::FrameBuffer* Scene::GetFrameBuffer() {
    return pFbo;
}

BOOST_CLASS_EXPORT(Engine::Scene)
