#include <math.h>
#include <iostream>

#include <string>
#include <ctime>
#include <GL/glew.h>
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

#include "interface.hpp"
#include "controller.hpp"

#include <GL/glext.h>  // Заголовок для расширений OpenGL

#include <sys/resource.h>

#include "modelmanager.hpp"

#include <time.h>
#include <thread>
#include <mutex>

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

bool mouseCapture = false;

void mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
    if (mouseCapture) {
        int width = g_windowSize.x, height = g_windowSize.y;
        static bool firstMouse = true;

        float xoffset = (width / 2) - xpos;
        float yoffset = (height / 2 - ypos); // инвертируем ось Y, так как движемся по экрану

        glfwSetCursorPos(window, width / 2, height / 2);

        Renderer::Camera* camera = static_cast<Renderer::Camera*>(glfwGetWindowUserPointer(window));  // Получаем камеру
        camera->ProcessMouseMovement(-xoffset, yoffset);
    }

    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(xpos, ypos);
}

bool mousePressed = false;
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse) {
        mouseCapture = !mouseCapture;
        if (mouseCapture) {
            glfwSetCursorPos(window, g_windowSize.x / 2, g_windowSize.y / 2);
        }

        glfwSetInputMode(window, GLFW_CURSOR, mouseCapture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mousePressed = true; // Флаг нажатия мыши
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mousePressed = false; // Флаг отпускания мыши
    }


    if (action == GLFW_PRESS) {
        ImGui::GetIO().MouseDown[button] = true;
    } else if (action == GLFW_RELEASE) {
        ImGui::GetIO().MouseDown[button] = false;
    }
}

void glfwWindowSizeCallback(GLFWwindow* pWindow, int wid, int hei) {
    g_windowSize.x = wid;
    g_windowSize.y = hei;
    glViewport(0, 0, wid, hei);
}


bool f2Pressed = false;
std::mutex cubeMutex;

int main(int argc, char** argv) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    ilInit();

    // Set GLFW window hints
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    GLFWwindow* window = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "", NULL, NULL);
    if (!window) {
        std::cerr << "Window failed" << std::endl;
        glfwTerminate();
        return -1;
    } 
     glfwSwapInterval(1); // Enable vsync


    // Setup Dear ImGui context

    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    // callbacks
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    Renderer::Camera camManager(window);
    camManager.yaw = 0.0f;
    camManager.pitch = 0.0f;
    camManager.speed = 10.5f;
    camManager.sensitivity = 0.2f;
    camManager.position = glm::vec3(0, 0, 0);


    glfwSetWindowUserPointer(window, &camManager);

    Renderer::ResourceManager resManager;
    resManager.CreateTexture("dirt.jpg");
    resManager.CreateTexture("grass.png");

    Renderer::ModelManager models;

    std::vector<GLfloat> points = {
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f 
    };

    std::vector<GLuint> faces = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Back face
        7, 6, 5,
        5, 4, 7,
        // Left face
        4, 0, 3,
        3, 7, 4,
        // Bottom face
        4, 5, 1,
        1, 0, 4,
        // Top face
        3, 2, 6,
        6, 7, 3
    };

    std::vector<GLfloat> texture_points = {
        // Front face
        0.0f, 0.0f, 
        1.0f, 0.0f, 
        1.0f, 1.0f, 
        0.0f, 1.0f, 

        // Right face
        1.0f, 0.0f, 
        1.0f, 1.0f, 
        0.0f, 1.0f, 
        0.0f, 0.0f, 

        // Back face
        1.0f, 0.0f, 
        1.0f, 1.0f, 
        0.0f, 1.0f, 
        0.0f, 0.0f, 

        // Left face
        0.0f, 0.0f, 
        1.0f, 0.0f, 
        1.0f, 1.0f, 
        0.0f, 1.0f, 

        // Top face
        0.0f, 1.0f, 
        0.0f, 0.0f, 
        1.0f, 0.0f, 
        1.0f, 1.0f, 

        // Bottom face
        1.0f, 1.0f, // up left
        1.0f, 0.0f, // up right
        1.0f, 0.0f, // bottom right
        0.0f, 0.0f, // bottom left
    };

    models.AddModel("cube", new Renderer::Model(points, faces, texture_points));

    int grid = 16;
    std::vector<Renderer::Object*> objs(grid*grid*grid);

    clock_t start_time = clock();

    // Сделать отдельный класс для кубов наследонный от Renderer::Object
    // Также сделать отдельный класс для хранения кубов (может в октодереве)
    int cubes = 0;
    for (int i = 0; i<grid; i++) {
        for (int j = 0; j<grid; j++) {
            for (int t = 0; t<grid; t++) {
                Renderer::Object* o = new Renderer::Object(i*2, t*2, j*2);
                o->SetModel(models.GetModel("cube"));
                o->SetTexture(resManager.GetTexture("dirt.jpg"));

                int index = i*grid*grid + j*grid + t;
                objs[index] = o;
                cubes++;
            }
        }
    }

    // Не работает IsActive, но сами faces определяются правильно (желательно перепроверить)
    for (int i = 0; i<grid; i++) {
        for (int j = 0; j<grid; j++) {
            for (int t = 0; t<grid; t++) {
                int index = i*grid*grid + j*grid + t;
                if (i < grid - 1) {
                    objs[index]->AddActiveFaces(Renderer::Face::FRONT, objs[(i+1)*grid*grid + j*grid + t]);
                }

                if (i > 0) {
                    objs[index]->AddActiveFaces(Renderer::Face::BACK, objs[(i-1)*grid*grid + j*grid + t]);
                }

                if (j < grid - 1) {
                    objs[index]->AddActiveFaces(Renderer::Face::RIGHT, objs[i*grid*grid + (j+1)*grid + t]);
                }

                if (j > 0) {
                    objs[index]->AddActiveFaces(Renderer::Face::LEFT, objs[i*grid*grid + (j-1)*grid + t]);
                }

                if (t < grid - 1) {
                    objs[index]->AddActiveFaces(Renderer::Face::TOP, objs[i*grid*grid + j*grid + (t+1)]);
                }

                if (t > 0) {
                    objs[index]->AddActiveFaces(Renderer::Face::BOTTOM, objs[i*grid*grid + j*grid + (t-1)]);
                }

                objs[index]->UpdateActiveFaces();
            }
        }
    }


    float deltaTime, lastFrame;

    int in;

    Engine::Controller controller(window);

    controller.AddCallback(GLFW_KEY_F2, true, []() {});


    Renderer::Model* cube = models.GetModel("cube");

    std::vector<glm::mat4*> obj_positions;

    for (int i = 0; i<grid; i++) {
        for (int j = 0; j<grid; j++) {
            for (int t = 0; t<grid; t++) {
                if (objs[i*grid*grid + j*grid + t]->IsActive == true) {
                    obj_positions.push_back(&objs[i*grid*grid + j*grid + t]->matmodel);
                }
            }
        }
    }

    obj_positions.erase(obj_positions.begin());
    cube->UpdatePositions(obj_positions);
    obj_positions.push_back(&objs[0]->matmodel);

    //std::thread updateThread = std::thread([&]() {
        //std::lock_guard<std::mutex> lock(cubeMutex);
        //cube->UpdatePositions(obj_positions);
    //});

    //updateThread.detach();

    Renderer::Interface interface(window);
    interface.AddCameraInfo(&camManager);
    interface.AddObjectsInfo(&objs, &obj_positions);

    int ticks = 0;
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camManager.Think();
        camManager.Control(window, deltaTime);

        controller.ProcessInput();

        int state = glfwGetKey(window, GLFW_KEY_F2);

        if (state == GLFW_PRESS) {
        } else if (state == GLFW_RELEASE) {
            f2Pressed = false; // Reset when the key is released
        }

        if (mousePressed) {
            mousePressed = false;
        }

        GLuint query;
        glGenQueries(1, &query);

        // Начало измерения
        glBeginQuery(GL_TIME_ELAPSED, query);


        //for (Renderer::Object* obj : objs) {
            //obj->Render(camManager.mvp);
        //}
        cube->UpdatePositions(obj_positions);
        cube->pShader->useProgram();
        glBindVertexArray(cube->vao); // Bind the VAO containing VBO and IBO configurations
        cube->pShader->setMatrix4("mvp", camManager.mvp);
        glDrawElementsInstanced(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0, obj_positions.size());

        glEndQuery(GL_TIME_ELAPSED);

        // Получение результата
        GLuint64 elapsed_time;
        glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);

        interface.Render(elapsed_time);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
