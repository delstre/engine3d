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
#include "thirdparty/imgui/imgui.h"
#include "interface.hpp"

#include "modelinstance.hpp"

#include <algorithm>

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

    std::cout << resManager.GetTexture("dirt.jpg") << " " << resManager.GetTexture("grass.png") << std::endl;
    std::vector<GLuint> textures = { resManager.GetTexture("dirt.jpg"), resManager.GetTexture("grass.png") };

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


        // Bottom face
        1.0f, 1.0f, // up left
        1.0f, 0.0f, // up right
        1.0f, 0.0f, // bottom right
        0.0f, 0.0f, // bottom left
        // Top face
        0.0f, 1.0f, 
        0.0f, 0.0f, 
        1.0f, 0.0f, 
        1.0f, 1.0f, 
    };

    models.AddModel("cube", new Renderer::Model(new Renderer::ShaderProgram("shaders/model.vert", "shaders/model.frag"), points, faces, texture_points));

    int grid = 0;
    std::vector<Renderer::Object*> objs(grid*grid*grid);

    clock_t start_time = clock();

    // Сделать отдельный класс для кубов наследованный от Renderer::Object
    // Также сделать отдельный класс для хранения кубов (может в октодереве)
    for (int i = 0; i<grid; i++) {
        for (int j = 0; j<grid; j++) {
            for (int t = 0; t<grid; t++) {
                Renderer::Object* o = new Renderer::Object(i*2, t*2, j*2);
                o->SetModel(models.GetModel("cube"));
                o->SetTexture(resManager.GetTexture("dirt.jpg"));

                int index = i*grid*grid + j*grid + t;
                objs[index] = o;
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



    //Renderer::Model* cube = models.GetModel("cube");

    Renderer::ModelInstance* cube = new Renderer::ModelInstance(new Renderer::ShaderProgram("shaders/model.vert", "shaders/model.frag"), points, faces, texture_points);

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

    //obj_positions.erase(obj_positions.begin());
    cube->UpdatePositions(obj_positions);
    //obj_positions.push_back(&objs[0]->matmodel);

    //cube->UpdateTextures({1, 0, 0, 1, 1, 1});

    int maxcubes = 32;
    int cubes = 0;
    int x = 0;
    int y = 0;
    int z = 0;

    struct min_max {
        glm::vec3 min;
        glm::vec3 max;
    };

    std::vector<min_max> cubes_min_max;
    std::thread updateThread = std::thread([&]() {
        while (cubes < maxcubes * maxcubes * maxcubes) {
            glm::vec3 vpos = glm::vec3(x, y, z);
            glm::mat4* pos = new glm::mat4(glm::translate(glm::mat4(1.0f), vpos));
            glm::vec3 min = vpos - glm::vec3(1, 1, 1);
            glm::vec3 max = vpos + glm::vec3(1, 1, 1);
            std::lock_guard<std::mutex> lock(cubeMutex);
            obj_positions.push_back(pos);
            cubes_min_max.push_back({min, max});
            //std::this_thread::sleep_for(std::chrono::milliseconds(6));

            x += 2;
            if (x/2 >= maxcubes) {
                y += 2;
                x = 0;
            }
            if (y/2 >= maxcubes) {
                z += 2;
                y = 0;
            }
            if (z/2 > maxcubes) {
                break;
            }
            cubes++;
            
        }
    });

    updateThread.join();

    std::thread whileThread = std::thread([&]() {
        while (!glfwWindowShouldClose(window)) {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            for (size_t i = 0; i < obj_positions.size(); i++) {
                if (camManager.RayIntersectsBox(camManager.position, camManager.GetMouseRay(mouseX, mouseY, g_windowSize.x, g_windowSize.y, camManager.projection, camManager.view), cubes_min_max[i].min, cubes_min_max[i].max)) {
                   cube->active_id = i; 
                   break;
                }
                //if (camManager.IsHitByRay(camManager.position, camManager.GetMouseRay(mouseX, mouseY, g_windowSize.x, g_windowSize.y, camManager.projection, camManager.view), glm::vec3((*obj_positions[i])[3]))) {
                    //active_id = i;
                    //break;
                //}
            }  

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });


    Engine::Controller controller(window);
    controller.AddCallback(GLFW_MOUSE_BUTTON_LEFT, true, true, [&cube]() {
        std::cout << "Left mouse button pressed" << std::endl;
        cube->call_id = cube->active_id;
    });

    whileThread.detach();

    Renderer::Interface interface(window);
    interface.AddCameraInfo(&camManager);
    interface.AddObjectsInfo(&objs, &obj_positions, &cube->active_id);

    cube->UpdatePositions(obj_positions);
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

        cube->Render(camManager.mvp, textures);

        //for (Renderer::Object* obj : objs) {
            //obj->Render(camManager.mvp);
        //}

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
