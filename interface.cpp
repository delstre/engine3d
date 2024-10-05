#include "interface.hpp"
#include "config.hpp"

#include <iostream>

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
}

void Interface::AddCameraInfo(Camera* camera) {
    pCamera = camera;
}

void Interface::AddObjectsInfo(std::vector<Object*>* objects) {
    pObjects = objects;
}

void Interface::SetModelManager(ModelManager* mdlManager) {
    pModelManager = mdlManager;
}

void Interface::SetResourceManager(ResourceManager* resManager) {
    pResourceManager = resManager;
}

void Interface::GetDebugInfo() const {
    if (pDebug == nullptr)
        return;

    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Debug")) {
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");

        ImGui::Text("Mouse capture: %b", io.WantCaptureMouse);
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

int size = 0;
float Color[3] = { 0.0f, 0.0f, 0.0f };
void Interface::GetObjectsInfo() const {
    if (pObjects == nullptr)
        return;

    if (ImGui::CollapsingHeader("Objects")) {
        int invisible = 0;

        ImGui::SliderInt("Number of objects", &size, 0, pObjects->size());
        for (int i = 0; i < size; i++) {
            if (pObjects->at(i)->IsActive == false) continue;
            ImGui::Text("Object %d", pObjects->at(i));
            ImGui::PushID(i);

            if (pObjects->at(i)->GetModelInstance() != nullptr) {
                if (ImGui::TreeNode("Model Instance")) {
                    for (glm::mat4 mat : pObjects->at(i)->GetModelInstance()->GetMatrixes()) {
                        ImGui::Text("Matrix %d", invisible);
                        ImGui::Text("%.6f %.6f %.6f %.6f", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
                        ImGui::Text("%.6f %.6f %.6f %.6f", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
                        ImGui::Text("%.6f %.6f %.6f %.6f", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
                        ImGui::Text("%.6f %.6f %.6f %.6f", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
                        invisible++;
                    }

                    ImGui::TreePop();
                }
            } else if (pObjects->at(i)->GetModel() != nullptr) {
                if (ImGui::TreeNode("Model")) {
                    for (Vertex vertex : pObjects->at(i)->GetModel()->GetVertices()) {
                        ImGui::Text("Vertex %d", invisible);
                        ImGui::Text("position: %.6f %.6f %.6f", vertex.position.x, vertex.position.y, vertex.position.z);
                        ImGui::Text("normal: %.6f %.6f %.6f", vertex.normal.x, vertex.normal.y, vertex.normal.z);
                        ImGui::Text("texcoord: %.6f %.6f", vertex.texCoord.x, vertex.texCoord.y);
                        invisible++;
                    }
                    ImGui::TreePop();
                }
            }

            if (ImGui::TreeNode("Position")) {
                ImGui::DragFloat("x", &pObjects->at(i)->position.x, 0.05f);
                ImGui::DragFloat("y", &pObjects->at(i)->position.y, 0.05f);
                ImGui::DragFloat("z", &pObjects->at(i)->position.z, 0.05f);
                pObjects->at(i)->UpdatePosition();
                ImGui::TreePop();
            }

            ImGui::PopID();
        }
    }
}

void Interface::GetConfigInfo() const {
    if (ImGui::CollapsingHeader("Config")) {
        ImGui::Checkbox("Interface", &Config::InterfaceDebugActive);
        if (ImGui::Checkbox("VSync", &Config::VSync)) {
            glfwSwapInterval(Config::VSync);
        };
         
    }
}

void Interface::GetModelManagerInfo() const {
    if (pModelManager == nullptr)
        return;

    if (ImGui::CollapsingHeader("ModelManager")) {
        if (ImGui::Button("Load Model (OBJ)")) {
            nfdchar_t *outPath = NULL;
            nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
            if (result == NFD_OKAY) {
                std::cout << "succ" << std::endl;
                free(outPath);
            } else if (result == NFD_CANCEL) {
                std::cout << "err" << std::endl;
            } else {
                std::cout << "unk err" << std::endl;
            }
        }
    }

}

void Interface::GetResourceManagerInfo() const {
    if (pResourceManager == nullptr)
        return;


}


void Interface::ShowExampleAppSimpleOverlay() const {
    static int location = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (location >= 0)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        //ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    else if (location == -2)
    {
        // Center window
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    if (ImGui::Begin("Example: Simple overlay", nullptr, window_flags)) {
        GetDebugInfo();
    }
    ImGui::End();
}

void Interface::Render(GLuint64 elapsed_time) {
    this->elapsed_time = elapsed_time;



    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ShowExampleAppSimpleOverlay();
    pDebug->CounterFPS();

    if (Config::InterfaceDebugActive) {
        ImGui::Begin("Render Engine 0.1");

        GetConfigInfo();
        ImGui::Separator();

        // Scene issue?
        GetCameraInfo();
        GetObjectsInfo();

        GetModelManagerInfo();
        GetResourceManagerInfo();
        
        ImGui::End();
    }


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



    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
