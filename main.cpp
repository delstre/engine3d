#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <IL/il.h>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "modelmanager.hpp"
#include "camera.hpp"
#include "resourcemanager.hpp"

#include <GL/glext.h>  // Заголовок для расширений OpenGL

#include <sys/resource.h>

bool showImGui = true;

GLfloat logGPUMemoryUsage() {
    GLint totalMemory = 0;
    GLint freeMemory = 0;

    glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &totalMemory);
    glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &freeMemory);

    return (1 - ((float)freeMemory / (float)totalMemory)) * 100;
}


struct rusage logMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    //"Memory usage: " << usage.ru_maxrss << " KB" << std::endl;
    return usage;
}

static int frameCount = 0;
static double lastTime = glfwGetTime();
static double fps = 0;
void logFPS() {
    // Увеличиваем количество кадров
    frameCount++;

    // Получаем текущее время
    double currentTime = glfwGetTime();
    double elapsedTime = currentTime - lastTime;

    // Если прошла 1 секунда
    if (elapsedTime >= 0.01) {
        // Вычисляем FPS
        fps = frameCount / elapsedTime;

        // Сбрасываем количество кадров и время
        frameCount = 0;
        lastTime = currentTime;
    }
}

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !ImGui::GetIO().WantCaptureMouse) {
        mouseCapture = !mouseCapture;
        if (mouseCapture) {
            glfwSetCursorPos(window, g_windowSize.x / 2, g_windowSize.y / 2);
        }

        glfwSetInputMode(window, GLFW_CURSOR, mouseCapture ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
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


    GLFWwindow* window = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "APPF", NULL, NULL);
    if (!window) {
        std::cerr << "Window failed" << std::endl;
        glfwTerminate();
        return -1;
    } 
     glfwSwapInterval(1); // Enable vsync


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    glfwMakeContextCurrent(window);
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // context

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    // callbacks
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    //glfwSetMouseButtonCallback(window, mouseButtonCallback);
    //glfwSetScrollCallback(window, scrollCallback);
    //glfwSetWindowSizeCallback(window, windowSizeCallback);

    const char* vertex_shader = 
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "layout(location = 2) in vec2 vertex_uv;"
        "out vec3 color;"
        "out vec2 uv;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "void main() {"
        "color = vertex_color;"
        "uv = vertex_uv;"
        "gl_Position = projection * view * model * vec4(vertex_position, 1.0);"
        "}";

    const char* fragment_shader = 
        "#version 460\n"
        "in vec2 uv;"
        "in vec3 color;"
        "out vec4 frag_color;"
        "uniform sampler2D my_texture;"
        "void main() {"
        "vec4 texColor = texture(my_texture, uv);"
        "frag_color = texColor * vec4(color, 1.0);"
        "}";

    std::string vertexShader(vertex_shader);
    std::string fragmentShader(fragment_shader);

    Renderer::ShaderProgram shaderProgram(vertexShader, fragmentShader);

    if (!shaderProgram.isCompiled()) {
        std::cerr << "shader program error!" << std::endl;
    }

    Renderer::Camera camManager(&shaderProgram, &g_windowSize);
    camManager.yaw = 0.0f;
    camManager.pitch = 0.0f;
    camManager.speed = 10.5f;
    camManager.sensitivity = 0.2f;

    glfwSetWindowUserPointer(window, &camManager);
    //camManager.position = glm::vec3(0.0, 3.0f, -7.0f);

    Renderer::ModelManager mdlManager(&shaderProgram, &camManager);
    //mdlManager.isWireFrame = true;

    //for (int i = 0; i<4; i++) {
        //for (int j = 0; j<4; j++) {
            //for (int t = 0; t<4; t++) {
                //Renderer::Cube c(glm::vec3(i*2, t*2, j*2), "dirt.jpg");
                //mdlManager.AddModel(c);
            //}
        //}
    //}

    Renderer::ResourceManager resManager;
    resManager.CreateTexture("dirt.jpg");

    Renderer::Cube c(glm::vec3(0, 0, 0), resManager.GetTexture("dirt.jpg"));
    mdlManager.AddModel(c);

    //Game::Map gameMap(&mdlManager, 8, 8, 8);

    float deltaTime, lastFrame;
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camManager.Render();
        camManager.Control(window, deltaTime);

        int state = glfwGetKey(window, GLFW_KEY_F2);

        if (state == GLFW_PRESS) {
            if (!f2Pressed) { // If it wasn't pressed before
                showImGui = !showImGui;  // Toggle the showImGui variable
                f2Pressed = true;         // Mark as pressed
            }
        } else if (state == GLFW_RELEASE) {
            f2Pressed = false; // Reset when the key is released
        }

        GLuint query;
        glGenQueries(1, &query);

        // Начало измерения
        glBeginQuery(GL_TIME_ELAPSED, query);
        mdlManager.Render();
        glEndQuery(GL_TIME_ELAPSED);

        // Получение результата
        GLuint64 elapsed_time;
        glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);


        if (showImGui) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Render Engine 0.1");

            ImGui::Text("Mouse capture: %b", ImGui::GetIO().WantCaptureMouse);
            ImGui::Text("GPU Usage %f", logGPUMemoryUsage());
            ImGui::Text("Memory Usage %d", logMemoryUsage().ru_maxrss);
            ImGui::Text("Render Time %f", elapsed_time / 1e6);
            ImGui::Text("FPS %f", fps);
            ImGui::Checkbox("Wireframe", &mdlManager.isWireFrame);

            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            glm::vec3 pos = camManager.getCursor3DPos(mouseX, mouseY);
            ImGui::Text("Cursor Position");
            ImGui::Text("x: %f", pos.x);
            ImGui::Text("y: %d", pos.y);
            ImGui::Text("z: %f", pos.z);

            if (ImGui::CollapsingHeader("Models Manipulation", mdlManager.vecModels.size())) {
                ImGui::Text("Models %d", mdlManager.vecModels.size());
                if (ImGui::Button("New Model")) {
                    mdlManager.AddModel(Renderer::Cube(glm::vec3(0.0, 0.0, 0.0), resManager.GetTexture("dirt.jpg")));
                }

                for (size_t i = 0; i < mdlManager.vecModels.size(); i++) {
                    Renderer::Cube& mdl = mdlManager.vecModels[i];

                    ImGui::PushID(i);
                    ImGui::Text("ModelID: %d", i);
                    if (ImGui::Button("Delete Model")) {
                        mdlManager.RemoveModel(i);
                    }
                    ImGui::SliderFloat("x", &mdl.position.x, -100.0f, 100.0f);
                    ImGui::SliderFloat("y", &mdl.position.y, -100.0f, 100.0f);
                    ImGui::SliderFloat("z", &mdl.position.z, -100.0f, 100.0f);
                    ImGui::PopID();
                }
            }


            ImGui::Text("Camera");
            ImGui::SliderFloat("c_x", &camManager.position.x, -100.0f, 100.0f);
            ImGui::SliderFloat("c_y", &camManager.position.y, -100.0f, 100.0f);
            ImGui::SliderFloat("c_z", &camManager.position.z, -100.0f, 100.0f);

            ImGui::Text("Eye Position");
            ImGui::SliderFloat("e_x", &camManager.front.x, -100.0f, 100.0f);
            ImGui::SliderFloat("e_y", &camManager.front.y, -100.0f, 100.0f);
            ImGui::SliderFloat("e_z", &camManager.front.z, -100.0f, 100.0f);
            ImGui::SliderFloat("e_yaw", &camManager.yaw, -180.0f, 180.0f);
            ImGui::SliderFloat("e_pitch", &camManager.pitch, -180.0f, 180.0f);

            ImGui::Text("Up Vector");
            ImGui::SliderFloat("u_x", &camManager.up.x, -100.0f, 100.0f);
            ImGui::SliderFloat("u_y", &camManager.up.y, -100.0f, 100.0f);
            ImGui::SliderFloat("u_z", &camManager.up.z, -100.0f, 100.0f);

            ImGui::Text("Right Vector");
            ImGui::SliderFloat("r_x", &camManager.right.x, -100.0f, 100.0f);
            ImGui::SliderFloat("r_y", &camManager.right.y, -100.0f, 100.0f);
            ImGui::SliderFloat("r_z", &camManager.right.z, -100.0f, 100.0f);

            ImGui::Text("World Up Vector");
            ImGui::SliderFloat("w_x", &camManager.worldUp.x, -100.0f, 100.0f);
            ImGui::SliderFloat("w_y", &camManager.worldUp.y, -100.0f, 100.0f);
            ImGui::SliderFloat("w_z", &camManager.worldUp.z, -100.0f, 100.0f);

            //for (Renderer::Cube& mdl : mdlManager.vecModels) {
                //ImGui::Text("Model %d", i);
                //ImGui::PushID(i);
                //ImGui::SliderFloat("s", &mdl.Scale, 0.0f, 10.0f);
                //ImGui::SliderFloat("x", &mdl.Pos.x, -10.0f, 10.0f);
                //ImGui::SliderFloat("y", &mdl.Pos.y, -10.0f, 10.0f);
                //ImGui::SliderFloat("z", &mdl.Pos.z, -10.0f, 10.0f);
                //ImGui::Checkbox("Wireframe", &mdl.isWireFrame);
                //ImGui::PopID();
                //i++;
            //}

            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }


        glfwPollEvents();
        glfwSwapBuffers(window);

        logFPS();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
