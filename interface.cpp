#include "interface.hpp"
#include "config.hpp"

using namespace Renderer;

Interface::Interface(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigDebugIsDebuggerPresent = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    pDebug = Engine::Debug::GetInstance();
}

void Interface::AddCameraInfo(Camera* camera) {
    pCamera = camera;
}

void Interface::AddObjectsInfo(std::vector<Object>* objects) {
    pObjects = objects;
}

void Interface::GetDebugInfo(GLuint64 elapsed_time) const {
    if (pDebug == nullptr)
        return;

    if (ImGui::CollapsingHeader("Debug")) {
        ImGui::Text("Mouse capture: %b", ImGui::GetIO().WantCaptureMouse);
        ImGui::Text("Render Time %f", elapsed_time / 1e6);
        ImGui::Text("GPU Usage %f", pDebug->GetGPUMemoryUsage());
        ImGui::Text("Memory Usage %d", pDebug->GetMemoryUsage());
        ImGui::Text("FPS %f", pDebug->GetFPS());
    }
}

void Interface::GetCameraInfo() const {
    if (pCamera == nullptr)
        return;

    if (ImGui::CollapsingHeader("Camera")) {
        if (ImGui::TreeNode("Position")) {
            ImGui::SliderFloat("x", &pCamera->position.x, -100.0f, 100.0f);
            ImGui::SliderFloat("y", &pCamera->position.y, -100.0f, 100.0f);
            ImGui::SliderFloat("z", &pCamera->position.z, -100.0f, 100.0f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Eye Position")) {
            ImGui::SliderFloat("x", &pCamera->front.x, -100.0f, 100.0f);
            ImGui::SliderFloat("y", &pCamera->front.y, -100.0f, 100.0f);
            ImGui::SliderFloat("z", &pCamera->front.z, -100.0f, 100.0f);
            ImGui::SliderFloat("yaw", &pCamera->yaw, -180.0f, 180.0f);
            ImGui::SliderFloat("pitch", &pCamera->pitch, -180.0f, 180.0f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Up Vector")) {
            ImGui::SliderFloat("x", &pCamera->up.x, -100.0f, 100.0f);
            ImGui::SliderFloat("y", &pCamera->up.y, -100.0f, 100.0f);
            ImGui::SliderFloat("z", &pCamera->up.z, -100.0f, 100.0f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Right Vector")) {
            ImGui::SliderFloat("x", &pCamera->right.x, -100.0f, 100.0f);
            ImGui::SliderFloat("y", &pCamera->right.y, -100.0f, 100.0f);
            ImGui::SliderFloat("z", &pCamera->right.z, -100.0f, 100.0f);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("World Up Vector")) {
            ImGui::SliderFloat("x", &pCamera->worldUp.x, -100.0f, 100.0f);
            ImGui::SliderFloat("y", &pCamera->worldUp.y, -100.0f, 100.0f);
            ImGui::SliderFloat("z", &pCamera->worldUp.z, -100.0f, 100.0f);
            ImGui::TreePop();
        }
    }
}

void Interface::GetObjectsInfo() const {
    if (pObjects == nullptr)
        return;

    if (ImGui::CollapsingHeader("Objects")) {
        for (int i = 0; i < pObjects->size(); i++) {
            ImGui::Separator();
            ImGui::Text("Object %d", i);
            ImGui::PushID(i);
            if (ImGui::TreeNode("Position")) {
                ImGui::DragFloat("x", &pObjects->at(i).position.x, 0.05f);
                ImGui::DragFloat("y", &pObjects->at(i).position.y, 0.05f);
                ImGui::DragFloat("z", &pObjects->at(i).position.z, 0.05f);
                pObjects->at(i).UpdatePosition();
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }
}

void Interface::GetConfigInfo() const {
    if (ImGui::CollapsingHeader("Config")) {
        ImGui::Checkbox("Interface", &Config::InterfaceDebugActive);
    }
}

void Interface::Render(GLuint64 elapsed_time) {
    if (!Config::InterfaceDebugActive)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Render Engine 0.1");

    GetDebugInfo(elapsed_time);
    GetConfigInfo();
    ImGui::Separator();
    GetCameraInfo();
    GetObjectsInfo();
    
    //pDebug->CounterFPS();

    //double mouseX, mouseY;
    //glfwGetCursorPos(window, &mouseX, &mouseY);
    //glm::vec3 pos = camManager.getCursor3DPos(mouseX, mouseY);
    //ImGui::Text("Cursor Position");
    //ImGui::Text("x: %f", pos.x);
    //ImGui::Text("y: %d", pos.y);
    //ImGui::Text("z: %f", pos.z);

    //if (ImGui::CollapsingHeader("Test")) {
        //ImGui::InputInt("Side", &in);
    //}

    //if (ImGui::CollapsingHeader("Texture Manipulation")) {
        //ImGui::Text("Textures %d", resManager.textures.size());

        //for (const auto& pair : resManager.textures) {
            //GLuint i = pair.second;
            //ImGui::Text("TextureID: %d", i);
            //ImGui::Image((void*)(intptr_t)i, ImVec2(64, 64));
        //}
    //}


    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
