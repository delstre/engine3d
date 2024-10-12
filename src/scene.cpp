#include "scene.hpp"
#include <csignal>
#include <typeinfo>
#include <cscomponent.hpp>

using namespace Engine;

void Scene::Init(GLFWwindow* pWindow) {
    pModelManager = new Renderer::ModelManager();
    pResourceManager = new Renderer::ResourceManager();
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
    pModelManager->ImportModel("projects/new/models/cube.obj");
    
    pFbo = new Renderer::FrameBuffer(wid, hei);
    this->pWindow = pWindow;
    glEnable(GL_DEPTH_TEST);

    initialized = true;
}

Renderer::Camera* Scene::AddCamera(glm::vec3 position) {
    if (pWindow == nullptr) {
        return nullptr;
    }

    Renderer::Camera* cam = new Renderer::Camera(pWindow);
    cam->yaw = 0.0f;
    cam->pitch = 0.0f;
    cam->speed = 10.5f;
    cam->sensitivity = 0.2f;
    cam->position = position;

    return cam;
}

void Scene::AddObject(Renderer::Object* obj) {
    obj->SetComponentManager(pComponentManager);
    obj->AddComponent("Transform");
    obj->AddComponent("ModelRender");
    obj->GetComponent<Renderer::ModelRender>()->SetModel(pModelManager->GetModel("cube"));

    objs.push_back(obj);
}

Renderer::Camera* Scene::GetActiveCamera() {
    if (pActiveCamera == nullptr) {
        pActiveCamera = AddCamera(glm::vec3(0, 0, 0));
    }

    return pActiveCamera;
}

std::vector<GLuint> Scene::GetTextures() {
    if (pResourceManager == nullptr) {
        return std::vector<GLuint>();
    }

    return pResourceManager->GetTextures();
}

std::vector<Renderer::Object*> Scene::GetObjects() {
    return objs;
}

Renderer::ModelManager* Scene::GetModelManager() {
    return pModelManager;
}

Renderer::ResourceManager* Scene::GetResourceManager() {
    return pResourceManager;
}

// bind and unbind for final application need to remove!

float lastFrame = 0;
float deltaTime = 0;
void Scene::Render() {
    if (!initialized)
        return;

    pController->ProcessInput();

    pFbo->Bind();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Renderer::Camera* cam = GetActiveCamera();
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

    pFbo->Unbind();
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

void Scene::SetComponentManager(Engine::ComponentManager* manager) {
    pComponentManager = manager;
}
