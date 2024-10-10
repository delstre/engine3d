#include <math.h>
#include <iostream>

#include <string>
#include <ctime>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <IL/il.h>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include "camera.hpp"
#include "model.hpp"
#include "resourcemanager.hpp"
#include "object.hpp"
#include "line.hpp"

#include "controller.hpp"

#include <GL/glext.h>  // Заголовок для расширений OpenGL

#include <sys/resource.h>

#include "modelmanager.hpp"

#include <time.h>
#include <thread>
#include <mutex>

#include "shaderprogram.hpp"
#include <imgui.h>
#include "interface.hpp"

#include "model.hpp"
#include "modelinstance.hpp"

#include <algorithm>

#include "config.hpp"

#include "window.hpp"

bool showImGui = true;

std::string formatString(const char* format, int value) {
    // Вычисляем необходимую длину буфера для форматированной строки
    int size = snprintf(nullptr, 0, format, value);

    // Создаем строку нужного размера
    std::string result(size, '\0');

    // Форматируем строку с использованием snprintf
    snprintf(&result[0], size + 1, format, value);

    return result; // Возвращаем форматированную строку
}

glm::ivec2 g_windowSize(1600, 800);

void mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(xpos, ypos);
}

bool mousePressed = false;
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    //if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse) {
        //mouseCapture = !mouseCapture;
        //if (mouseCapture) {
            //glfwSetCursorPos(window, g_windowSize.x / 2, g_windowSize.y / 2);
        //}

        //glfwSetInputMode(window, GLFW_CURSOR, mouseCapture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    //}

    //if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        //mousePressed = true; // Флаг нажатия мыши
    //} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        //mousePressed = false; // Флаг отпускания мыши
    //}


    //if (action == GLFW_PRESS) {
        //ImGui::GetIO().MouseDown[button] = true;
    //} else if (action == GLFW_RELEASE) {
        //ImGui::GetIO().MouseDown[button] = false;
    //}
}

void glfwWindowSizeCallback(GLFWwindow* pWindow, int wid, int hei) {
    g_windowSize.x = wid;
    g_windowSize.y = hei;
    glViewport(0, 0, wid, hei);
}


//int main(int argc, char** argv) {
    //if (!glfwInit()) {
        //std::cerr << "Failed to initialize GLFW" << std::endl;
        //return -1;
    //}

    //ilInit();

    //// Set GLFW window hints
    //const char* glsl_version = "#version 130";
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    //GLFWwindow* window = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "", NULL, NULL);
    //if (!window) {
        //std::cerr << "Window failed" << std::endl;
        //glfwTerminate();
        //return -1;
    //} 
     //glfwSwapInterval(1); // Enable vsync


    //// Setup Dear ImGui context

    //glfwMakeContextCurrent(window);


    //if (glewInit() != GLEW_OK) {
        //std::cerr << "GLEW failed" << std::endl;
        //exit(EXIT_FAILURE);
    //}

    //glEnable(GL_DEPTH_TEST);

    //// callbacks
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
    ////models.ImportModel("/home/delstre-/cnt/prj/vxl/build/models/cube.obj");
    ////models.ImportModel("bobo.obj");


    //// Object Manager // Scene?
    //// Need to save positions of cubes if cubes invisible

    //std::vector<Renderer::Object*> objs;
    ////Renderer::Object* obj = new Renderer::Object(models.GetModel("cube.obj"), 0, 0, 0);
    ////obj->SetTexture(textures[1]);
    ////objs.push_back(obj);
    ////
    ////int grid = 2;
    ////std::vector<Renderer::Object*> objs;

    ////for (int i = 0; i < grid; i++) {
        ////for (int j = 0; j < grid; j++) {
            ////for (int k = 0; k < grid; k++) {
                ////Renderer::Object* cube = new Renderer::Object(models.GetModel("cube.obj"), i*2, j*2, k*2);
                //////cube->SetTexture(textures[1]);
                //////objs[i*grid*grid + j*grid + k] = cube;
                ////objs.push_back(cube);
            ////}
        ////}
    ////}

    ////objs[0]->SetModelInstance(Renderer::TranslateModelsToInstance(objs, 0, objs.size()));
    ////objs.erase(objs.begin() + 1, objs.begin() + objs.size());

    ////std::vector<Renderer::Object*> render_objs;
    ////enum Face {
        ////TOP,
        ////BOTTOM,
        ////LEFT,
        ////RIGHT,
        ////FRONT,
        ////BACK,
        ////FACE_COUNT // Удобно для определения размера
    ////};

    ////std::vector<std::vector<Renderer::Object*>> objs_faces(grid * grid * grid, std::vector<Renderer::Object*>(FACE_COUNT, nullptr));

    ////for (int i = 0; i < grid; i++) {
        ////for (int j = 0; j < grid; j++) {
            ////for (int k = 0; k < grid; k++) {
                ////Renderer::Object* cube = objs[i * grid * grid + j * grid + k];

                ////// Проверяем и связываем соседние объекты
                ////if (i + 1 < grid && objs[(i + 1) * grid * grid + j * grid + k] != nullptr) {
                    ////objs_faces[i * grid * grid + j * grid + k][RIGHT] = objs[(i + 1) * grid * grid + j * grid + k];
                ////}

                ////if (i - 1 >= 0 && objs[(i - 1) * grid * grid + j * grid + k] != nullptr) {
                    ////objs_faces[i * grid * grid + j * grid + k][LEFT] = objs[(i - 1) * grid * grid + j * grid + k];
                ////}

                ////if (j + 1 < grid && objs[i * grid * grid + (j + 1) * grid + k] != nullptr) {
                    ////objs_faces[i * grid * grid + j * grid + k][TOP] = objs[i * grid * grid + (j + 1) * grid + k];
                ////}

                ////if (j - 1 >= 0 && objs[i * grid * grid + (j - 1) * grid + k] != nullptr) {
                    ////objs_faces[i * grid * grid + j * grid + k][BOTTOM] = objs[i * grid * grid + (j - 1) * grid + k];
                ////}

                ////if (k + 1 < grid && objs[i * grid * grid + j * grid + (k + 1)] != nullptr) {
                    ////objs_faces[i * grid * grid + j * grid + k][BACK] = objs[i * grid * grid + j * grid + (k + 1)];
                ////}

                ////if (k - 1 >= 0 && objs[i * grid * grid + j * grid + (k - 1)] != nullptr) {
                    ////objs_faces[i * grid * grid + j * grid + k][FRONT] = objs[i *grid*grid+ j*grid+ (k-1)];
                ////}
            ////}
        ////}
    ////}

    ////for (int i = 0; i < grid * grid * grid; i++) {
        ////for (int j = 0; j < FACE_COUNT; j++) {
            ////if (objs_faces[i][j] == nullptr) {
                ////render_objs.push_back(objs[i]);
                ////break;
            ////}
        ////}
    ////}

    ////std::cout << render_objs.size() << std::endl;

    ////std::vector<Renderer::Object*> render_objs_x(render_objs);

    ////// Optimize for render


    ////render_objs[0]->SetModelInstance(Renderer::TranslateModelsToInstance(render_objs, 0, render_objs.size()));
    ////render_objs.erase(render_objs.begin() + 1, render_objs.begin() + render_objs.size());
    ////objs[0]->GetModelInstance()->UpdateColors({glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f)});
    ////objs[0]->GetModelInstance()->UpdateColors();

    //Engine::Controller controller(window);

    //controller.AddCallback(GLFW_KEY_F2, true, false, []() {
        //Config::InterfaceDebugActive = !Config::InterfaceDebugActive;
    //});

    ////controller.AddCallback(GLFW_MOUSE_BUTTON_LEFT, true, true, [&camManager, &lines, &render_objs_x, &render_objs]() {
        ////camManager.UpdateFrustum();

        ////std::vector<glm::mat4> matrixes;
        ////for (Renderer::Object* obj : render_objs_x) {
            ////if (camManager.IsObjectInFrustum(obj)) {
                ////matrixes.push_back(obj->GetMatrix());
            ////}
        ////}

        ////render_objs[0]->GetModelInstance()->UpdatePositions(matrixes);
    ////});


    ////std::thread updateFrustum([&window, &camManager, &render_objs_x, &render_objs]() {
        ////while (!glfwWindowShouldClose(window)) {
            ////camManager.UpdateFrustum();

            ////std::vector<glm::mat4> matrixes;
            ////for (Renderer::Object* obj : render_objs_x) {
                ////if (camManager.IsObjectInFrustum(obj)) {
                    ////matrixes.push_back(obj->GetMatrix());
                ////}
            ////}

            ////render_objs[0]->GetModelInstance()->UpdatePositions(matrixes);
            ////std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ////} 
    ////});

    ////std::thread updateActiveBlocks([&window, &camManager, &render_objs, &render_objs_x]() {
        ////while (!glfwWindowShouldClose(window)) {
            ////double mouseX, mouseY;
            ////glfwGetCursorPos(window, &mouseX, &mouseY);

            ////int idIntersect = -1;
            ////float nearest = FLT_MAX;
            ////for (int i = 0; i < render_objs_x.size(); i++) {
                ////Renderer::Object* obj = render_objs_x[i];
                ////if (obj->GetModel() == nullptr)
                    ////continue;
                //////if (glm::distance(camManager.position, obj->position) < 50) {
                //////if (camManager.RayIntersectsBox(
                    //////camManager.position, 
                    //////camManager.GetMouseRay(mouseX, mouseY, g_windowSize.x, g_windowSize.y, camManager.projection, camManager.view), 
                    //////obj->GetMinBounds(), 
                    //////obj->GetMaxBounds(), 
                    //////near, 
                    //////far)) {

                    //////if (near < nearest && near >= 0) {
                        //////nearest = near;
                        //////idIntersect = i;
                    //////}
                //////}
                ////if (camManager.IsHitByRay(camManager.position, camManager.GetMouseRay(mouseX, mouseY, g_windowSize.x, g_windowSize.y, camManager.projection, camManager.view), obj->position)) {
                    ////float dist = glm::distance(camManager.position, obj->position);
                    ////if (dist < nearest) {
                        ////nearest = dist;
                        ////idIntersect = i;
                    ////}
                ////}
                //////}
            ////}

            ////std::cout << idIntersect << std::endl;
            ////render_objs[0]->GetModelInstance()->call_id = idIntersect; 

            ////std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ////}
    ////});

    ////updateActiveBlocks.detach();

    //Renderer::Interface interface(window);
    //interface.AddCameraInfo(&camManager);
    //interface.AddObjectsInfo(&objs);
    //interface.SetModelManager(&models);
    //interface.SetResourceManager(&resManager);

    //Renderer::FrameBuffer fbo(g_windowSize.x, g_windowSize.y);
    //interface.SetSceneInfo(&fbo);


    //float deltaTime, lastFrame;
    //while (!glfwWindowShouldClose(window)) {
        //glClearColor(0, 0, 0, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //float currentFrame = glfwGetTime();
        //deltaTime = currentFrame - lastFrame;
        //lastFrame = currentFrame;

        //camManager.Think();
        //camManager.Control(window, deltaTime);

        //controller.ProcessInput();

        //int state = glfwGetKey(window, GLFW_KEY_F2);

        //if (state == GLFW_PRESS) {
        //} else if (state == GLFW_RELEASE) {
            //f2Pressed = false; // Reset when the key is released
        //}

        //if (mousePressed) {
            //mousePressed = false;
        //}

        //GLuint query;
        //glGenQueries(1, &query);

        //// Начало измерения
        //glBeginQuery(GL_TIME_ELAPSED, query);

        //// Рендеринг
        //fbo.Bind();
        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Renderer::Envy env;
        //env.viewpos = camManager.position;
        //env.viewdir = camManager.front;
        //env.mvp = camManager.mvp;

        //for (Renderer::Object* obj : objs) {
            //if (!obj->ModelIsInstanced()) {
                //if (camManager.IsObjectInFrustum(obj))
                    //obj->Render(env, resManager.GetTextures());
            //} else {
                //obj->Render(env, resManager.GetTextures());
            //}
        //}

        //fbo.Unbind();

        //glEndQuery(GL_TIME_ELAPSED);

        //// Получение результата
        //GLuint64 elapsed_time;
        //glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);

        //interface.Render(elapsed_time);

        //glfwPollEvents();
        //glfwSwapBuffers(window);
    //}

    //ImGui_ImplOpenGL3_Shutdown();
    //ImGui_ImplGlfw_Shutdown();
    //ImGui::DestroyContext();
    //glfwTerminate();
    //return 0;
//}

int main() {
    Engine::Window window(1280, 720, "Engine Window");
    window.Init();

    return 0;
}
