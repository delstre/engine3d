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

void Interface::AddObjectsInfo(std::vector<Object*>* objects, std::vector<glm::mat4*>* positions) {
    pObjects = objects;
    pObjectsPositions = positions;
}

void Interface::GetDebugInfo(GLuint64 elapsed_time) const {
    if (pDebug == nullptr)
        return;

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
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

            //ImGui::BeginChild("Scrollable Region", ImVec2(0, 100), true);

            if (ImGui::Button("Delete")) {
                pObjects->at(i+1)->IsActive = false;
                pObjectsPositions->erase(pObjectsPositions->begin() + i+1);
            }

            if (ImGui::TreeNode("Position")) {
                ImGui::DragFloat("x", &pObjects->at(i)->position.x, 0.05f);
                ImGui::DragFloat("y", &pObjects->at(i)->position.y, 0.05f);
                ImGui::DragFloat("z", &pObjects->at(i)->position.z, 0.05f);
                pObjects->at(i)->UpdatePosition();
                ImGui::TreePop();
            }
            
            if (ImGui::TreeNode("Color")) {
                ImGui::ColorEdit3("Color", Color);
                if (ImGui::Button("Apply")) {
                    std::cout << Color[0] << " " << Color[1] << " " << Color[2] << std::endl;
                    pObjects->at(i)->SetModelColor(Color[0], Color[1], Color[2]);
                }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Faces")) {
                if (ImGui::TreeNode("Front")) {
                    ImGui::Text("ID %d", pObjects->at(i)->activeFaces[FRONT]);
                    if (pObjects->at(i)->activeFaces[FRONT] != nullptr && ImGui::Button("Apply")) {
                        pObjects->at(i)->activeFaces[FRONT]->IsActive = !pObjects->at(i)->activeFaces[FRONT]->IsActive;
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Back")) {
                    ImGui::Text("ID %d", pObjects->at(i)->activeFaces[BACK]);
                    if (pObjects->at(i)->activeFaces[BACK] != nullptr && ImGui::Button("Apply")) {
                        pObjects->at(i)->activeFaces[BACK]->IsActive = !pObjects->at(i)->activeFaces[BACK]->IsActive;
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Left")) {
                    ImGui::Text("ID %d", pObjects->at(i)->activeFaces[LEFT]);
                    if (pObjects->at(i)->activeFaces[LEFT] != nullptr && ImGui::Button("Apply")) {
                        pObjects->at(i)->activeFaces[LEFT]->IsActive = !pObjects->at(i)->activeFaces[LEFT]->IsActive;
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Right")) {
                    ImGui::Text("ID %d", pObjects->at(i)->activeFaces[RIGHT]);
                    if (pObjects->at(i)->activeFaces[RIGHT] != nullptr && ImGui::Button("Apply")) {
                        pObjects->at(i)->activeFaces[RIGHT]->IsActive = !pObjects->at(i)->activeFaces[RIGHT]->IsActive;
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Top")) {
                    ImGui::Text("ID %d", pObjects->at(i)->activeFaces[TOP]);
                    if (pObjects->at(i)->activeFaces[TOP] != nullptr && ImGui::Button("Apply")) {
                        pObjects->at(i)->activeFaces[TOP]->IsActive = !pObjects->at(i)->activeFaces[TOP]->IsActive;
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNode("Bottom")) {
                    ImGui::Text("ID %d", pObjects->at(i)->activeFaces[BOTTOM]);
                    if (pObjects->at(i)->activeFaces[BOTTOM] != nullptr && ImGui::Button("Apply")) {
                        pObjects->at(i)->activeFaces[BOTTOM]->IsActive = !pObjects->at(i)->activeFaces[BOTTOM]->IsActive;
                    }
                    ImGui::TreePop();
                }

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
    
    pDebug->CounterFPS();

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
