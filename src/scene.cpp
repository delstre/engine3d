#include <transform.hpp>
#include <mesh.hpp>
#include <model.hpp>
#include <object.hpp>
#include <scene.hpp>
#include <icomponent.hpp>
#include <controller.hpp>
#include <framebuffer.hpp>
#include <camera.hpp>
#include <resourcemanager.hpp>

#include <csignal>
#include <typeinfo>
#include <iostream>
#include <sstream>

using namespace Engine;
using namespace Renderer;

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

    pFbo = new Renderer::FrameBuffer(wid, hei);
    this->pWindow = pWindow;
    glEnable(GL_DEPTH_TEST);

    initialized = true;
    selectedObject = NULL;
}

void Scene::SetPath(std::string path) {
    this->path = path;
}

Engine::Camera* Scene::AddCamera(glm::vec3 position) {
    if (pWindow == nullptr) {
        return nullptr;
    }

    Engine::Camera* cam = new Engine::Camera(this);
    cam->yaw = 0.0f;
    cam->pitch = 0.0f;
    cam->speed = 10.5f;
    cam->sensitivity = 0.2f;
    cam->position = position;

    return cam;
}


void Scene::AddObject(Engine::Object* obj) {
    std::lock_guard<std::mutex> lock(scene_mutex);
    objs.push_back(obj);
}

void Scene::DeleteObject(Engine::Object* id) {
    std::lock_guard<std::mutex> lock(scene_mutex);
    for (int i = 0; i < objs.size(); i++) {
        if (objs[i] == id) {
            objs.erase(objs.begin() + i);
            break;
        }
    }
}

std::string vec3_to_str(const glm::vec3& v) {
    std::stringstream ss;
    ss << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return ss.str();
}


Intersection Scene::Raycast(const Ray& worldRay)
{
    Intersection closest;
    closest.hit = false;
    closest.distance = std::numeric_limits<float>::max();

    for (auto& object : GetObjects()) {
        Model *model = object->GetComponent<Model>();
        Transform *form = object->GetComponent<Transform>();
        std::cout << "Object pos: (" << form->position.x << "," << form->position.y << "," << form->position.z << ")" << std::endl;

        
        if (form == NULL || model == NULL)
        {
            std::cout << "Model or Transforfm not found!" << std::endl;
            continue;
        }

        const glm::mat4& modelMatrix = form->GetMatrix();

        Mesh *mesh = model->GetMesh();
        if (mesh == NULL) 
        {
            std::cout << "Mesh not found!" << std::endl;
            continue;
        }

        const auto& vertices = mesh->GetVertices();
        const auto& indices = mesh->GetIndices();

        for (size_t i = 0; i < indices.size(); i += 3) {
            glm::vec3 v0 = glm::vec3(modelMatrix * glm::vec4(vertices[indices[i]].position, 1.0f));
            glm::vec3 v1 = glm::vec3(modelMatrix * glm::vec4(vertices[indices[i+1]].position, 1.0f));
            glm::vec3 v2 = glm::vec3(modelMatrix * glm::vec4(vertices[indices[i+2]].position, 1.0f));

            float t, u, v;
            if (rayTriangleIntersect(worldRay, v0, v1, v2, t, u, v)) {
                float w = 1.0f - u - v;

                int nearestVertex = 0;
                float maxCoord = w;
                if (u > maxCoord) { maxCoord = u; nearestVertex = 1; }
                if (v > maxCoord) { maxCoord = v; nearestVertex = 2; }

                int vertexIdx = indices[i + nearestVertex];

                if (t < closest.distance) {
                    closest.hit = true;
                    closest.distance = t;
                    closest.point = worldRay.origin + worldRay.direction * t;
                    closest.barycentric = glm::vec3(w, u, v);
                    closest.triangleIndex = i / 3;
                    closest.localPosition = vertices[vertexIdx].position;
                    closest.mesh = mesh;
                    closest.object = object;
                }

            }
        }
    }
    return closest;
}

Engine::Camera* Scene::GetActiveCamera() {
    if (pActiveCamera == nullptr) {
        pActiveCamera = AddCamera(glm::vec3(0, 0, 0));
    }

    return pActiveCamera;
}

Renderer::FrameBuffer* Scene::GetFrameBuffer() {
    return pFbo;
}

std::vector<GLuint> Scene::GetTextures() {
    return Engine::ResourceManager::GetTextures();
}

std::vector<Engine::Object*> Scene::GetObjects() {
    return objs;
}

GLFWwindow* Scene::GetWindow() {
    return pWindow;
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
    glColor3f(0.4,0.4,0.4);
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
        env.highlightColor = selectMode == MODE_OBJECT && objs[i] == selectedObject ? glm::vec3(0.8f, 0.6f, 0.3f) : glm::vec3(0.6f, 0.6f, 0.6f); 
        env.highlightColor_Vertex = selectMode == MODE_MESH && objs[i] == selectedObject ? glm::vec3(0.8f, 0.6f, 0.3f) : glm::vec3(0.6f, 0.6f, 0.6f); 
        env.selectTriangle = selectMode == MODE_MESH && objs[i] == selectedObject ? triangleIndex : -1; 
        objs[i]->SetENV(env);
        objs[i]->Update();
    }

    #ifdef INTERFACE_DEBUG
    pFbo->Unbind();
    #endif
}

void Scene::SetFrameSize(int width, int height) {
    std::lock_guard<std::mutex> lock(scene_mutex);
    this->width = width;
    this->height = height;

    if (pFbo == nullptr) {
        pFbo = new Renderer::FrameBuffer(width, height);
    }

    pFbo->RescaleFrameBuffer(width, height);
}

float Scene::GetAspectRatio() {
    return pFbo->GetAspectRatio();
}
