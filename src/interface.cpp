#include <interface.hpp>
#include <config.hpp>
#include <framebuffer.hpp>
#include <scene.hpp>
#include <window.hpp>
#include <debug.hpp>
#include <camera.hpp>
#include <component.hpp>
#include <mesh.hpp>
#include <modelmanager.hpp>
#include <componentmanager.hpp>
#include <resourcemanager.hpp>
#include <executer.hpp>
#include <model.hpp>

#include <nfd.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <glm/gtc/type_ptr.hpp>

#include <thread>
#include <sstream>
#include <clocale>
#include <iostream>
#include <memory>
#include <filesystem>
#include <fstream>

// Helpers macros
// We normally try to not use many helpers in imgui_demo.cpp in order to make code easier to copy and paste,
// but making an exception here as those are largely simplifying code...
// In other imgui sources we can use nicer internal functions from imgui_internal.h (ImMin/ImMax) but not in the demo.
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

// Enforce cdecl calling convention for functions called by the standard library,
// in case compilation settings changed the default to e.g. __vectorcall
#ifndef IMGUI_CDECL
#ifdef _MSC_VER
#define IMGUI_CDECL __cdecl
#else
#define IMGUI_CDECL
#endif
#endif

using namespace Renderer;

Interface::Interface(Engine::Window* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //io.ConfigDebugIsDebuggerPresent = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    pWindow = window;
}


void Interface::SetProject(Engine::Project* pProject) {
    this->pProject = pProject;
    //pProject->Init();
}

Engine::Scene* Interface::GetScene() const {
    return pProject->GetScene();
}

void Interface::GetDebugInfo() const {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Debug")) {
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");

        ImGui::Text("Mouse capture: %b", io.WantCaptureMouse);
        ImGui::Text("Render Time %f", elapsed_time / 1e6);
        ImGui::Text("GPU Usage %f", Engine::Debug::GetGPUMemoryUsage());
        ImGui::Text("Memory Usage %d", Engine::Debug::GetMemoryUsage());
        ImGui::Text("FPS %f", Engine::Debug::GetFPS());
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

static float window_width;
static float window_height;
bool mouseCapture = false;
void Interface::GetSceneInfo(Engine::Scene* scene) const {
    Renderer::FrameBuffer* pFbo = scene->GetFrameBuffer();

    if (pFbo == nullptr)
        return;

    ImGui::Begin("Scene");

    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();

    // Check if the mouse is within the panel bounds
    bool isMouseInsidePanel = 
        mousePos.x >= windowPos.x && 
        mousePos.x <= windowPos.x + windowSize.x && 
        mousePos.y >= windowPos.y && 
        mousePos.y <= windowPos.y + windowSize.y;

        if (isMouseInsidePanel) {
            if (ImGui::IsMouseClicked(1)) {
                mouseCapture = true;
                glfwSetInputMode(pWindow->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                glfwSetCursorPos(pWindow->GetWindow(), (windowPos.x + windowSize.x) / 2, (windowPos.y + windowSize.y) / 2);
            }
        }

        if (mouseCapture) {
            double cursorX, cursorY;
            glfwGetCursorPos(pWindow->GetWindow(), &cursorX, &cursorY);

            float xoffset = ((windowPos.x + windowSize.x) / 2) - cursorX;
            float yoffset = ((windowPos.y + windowSize.y) / 2) - cursorY; // Invert Y-axis

            glfwSetCursorPos(pWindow->GetWindow(), (windowPos.x + windowSize.x) / 2, (windowPos.y + windowSize.y) / 2);

            Engine::Camera* pCamera = scene->GetActiveCamera();
            pCamera->ProcessMouseMovement(-xoffset, yoffset);
        }

        if (ImGui::IsMouseReleased(1)) { // Right mouse button released
            mouseCapture = false;
            glfwSetInputMode(pWindow->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Show the cursor again
        }


        ImGui::BeginChild("GameRender");

        ImVec2 wsize = ImGui::GetWindowSize();
        ImVec2 pos = ImGui::GetCursorScreenPos();

        window_width = ImGui::GetContentRegionAvail().x;
        window_height = ImGui::GetContentRegionAvail().y;

        pFbo->RescaleFrameBuffer(window_width, window_height);
        glViewport(0, 0, window_width, window_height);

        ImGui::GetWindowDrawList()->AddImage(
            (ImTextureID)pFbo->getFrameTexture(),
            ImVec2(pos.x, pos.y),
            ImVec2(pos.x + window_width, pos.y + window_height),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );


        ImGui::EndChild();

    ImGui::End();
}

void Interface::GetCameraInfo(Engine::Scene* scene) const {
    ImGui::Begin("Camera");

    Engine::Camera* pCamera = scene->GetActiveCamera();

    ImGui::SliderFloat("Zoom", &pCamera->zoom, 0.0f, 180.0f);

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

    ImGui::End();
}

void Interface::GetObjectsInfo(Engine::Scene* scene) {
    ImGui::Begin("Objects");

    if (ImGui::Button("Create object"))
        ImGui::OpenPopup("my_select_popup");

    if (ImGui::BeginPopup("my_select_popup")) {
        for (const auto& entry : std::filesystem::directory_iterator(pProject->GetPath())) {
            if (entry.is_regular_file() && entry.path().extension() == ".bin") {
                if (ImGui::Selectable(entry.path().filename().string().c_str())) {
                    Engine::Object* obj = new Engine::Object(entry.path().filename().string());
                    obj->LoadFromPrefab(entry.path().string());
                    scene->AddObject(obj);
                }
            }
        }

        if (ImGui::Selectable("Empty Object")) {
            scene->AddObject(new Engine::Object("empty object"));
        }
        ImGui::EndPopup();
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("List of objects")) {
        for (Engine::Object* obj : scene->GetObjects()) {
            ImGui::PushID(obj);

            Engine::Camera* pCamera = scene->GetActiveCamera();
            ImVec2 mousePos = ImGui::GetMousePos();

            Engine::Model* pModel = obj->GetComponent<Engine::Model>();
            if (pModel) {
                //double x, y;
                //int w, h;
                //glfwGetCursorPos(pWindow->GetWindow(), &x, &y);
                //glm::vec3 dir = pCamera->GetCursorDirection(x, y, window_width, window_height);
                //if (obj->IsHitByRay(pCamera->position, dir)) {
                    //pModel->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
                //} else {
                    //pModel->SetColor(glm::vec3(0.5f, 0.5f, 0.5f));
                //}

                //std::cout << "Front:" << std::endl;
                //std::cout << pCamera->front.x << " " << pCamera->front.y << " " << pCamera->front.z << std::endl;
                //std::cout << "Mouse" << std::endl;
                //std::cout << dir.x << " " << dir.y << " " << dir.z << std::endl;
            }
            
            std::stringstream addressStream;
            addressStream << std::hex << reinterpret_cast<std::uintptr_t>(obj);
            std::string addressString = addressStream.str();

            std::string name = "Object: " + obj->name + " (Address: " + addressString + ")";

            if (ImGui::Selectable(name.c_str(), pSelectedObject == obj)) {
                if (ImGui::IsMouseReleased(1) && ImGui::IsItemHovered()) {
                    // Открываем попап при отпускании правой кнопки мыши
                    ImGui::OpenPopup("my_select");
                } else if (ImGui::IsMouseReleased(0)) {
                    pSelectedObject = obj;
                }

                if (ImGui::BeginPopup("my_select")) {
                    if (ImGui::Selectable("Delete")) {
                        scene->DeleteObject(obj);
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::PopID();
        }

        ImGui::TreePop();
    }

    ImGui::End();
}

template <typename T>
void print_bases()
{
}


void Interface::ObjectInspector(Engine::Scene* scene) const {
    ImGui::Begin("Object inspector");

    if (pSelectedObject != nullptr) {
        ImGui::Text("Name: %s", pSelectedObject->name.c_str());

        ImGui::SameLine();

        if (ImGui::Button("Delete object"))
            scene->DeleteObject(pSelectedObject);
        

        if (ImGui::Button("Save as prefab"))
            pSelectedObject->SaveAsPrefab(pProject->GetPath());


        if (ImGui::Button("Add component"))
            ImGui::OpenPopup("my_select_popup");

        if (ImGui::BeginPopup("my_select_popup")) {
            for (const auto& [key, value] : Engine::ComponentManager::GetConstructors()) {
                if (ImGui::Selectable(key.c_str())) {
                    pSelectedObject->AddComponent(key);
                }
            }

            ImGui::EndPopup();
        }

        for (Engine::Component* pComponent : pSelectedObject->GetComponents()) {
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);

            if (pComponent == nullptr)
                continue;

            std::stringstream addressStream;
            addressStream << std::hex << reinterpret_cast<std::uintptr_t>(pComponent);
            std::string addressString = addressStream.str();

            std::string name = "Component: " + pComponent->GetTypeName() + " (Address: " + addressString + ")";
            

            if (ImGui::TreeNode(name.c_str())) {
                if (ImGui::Button("Remove component"))
                    pSelectedObject->RemoveComponent(pComponent->GetTypeName());

                for (const auto& var : pComponent->variables) {

                    if (var->type_name == "bool") {
                        ImGui::Checkbox(var->name, static_cast<bool*>(var->ptr()));
                    } else if (var->type_name == "glm::vec3") {
                        ImGui::InputFloat3(var->name, glm::value_ptr(*static_cast<glm::vec3*>(var->ptr())));
                    } else if (var->type_name == "int") {
                        ImGui::InputInt(var->name, static_cast<int*>(var->ptr()));
                    } else if (var->type_name == "float") {
                        ImGui::InputFloat(var->name, static_cast<float*>(var->ptr()), 0.01f);
                    } else if (var->type_name == "Renderer::Mesh*") {
                        //ImGui::Text("Mesh: %s", *static_cast<Renderer::Mesh**>(var->ptr()));
                        if (ImGui::Button("Select Mesh"))
                            ImGui::OpenPopup("my_select_popup");

                        if (ImGui::BeginPopup("my_select_popup")) {
                            for (const auto& [key, value] : Engine::ModelManager::GetModels()) {
                                if (ImGui::Selectable(key.c_str())) {
                                    *static_cast<Renderer::Mesh**>(var->ptr()) = value;
                                }
                            }

                            ImGui::EndPopup();
                        }

                    } else {
                        //ImGui::Text((var->type_name + var->name).c_str());
                    }
                }

                pComponent->UpdateInterface();
                ImGui::TreePop();
            }
        }
    }

    ImGui::End();
}

void Interface::GetModelManager(Engine::Scene* scene) const {
    ImGui::Begin("ModelManager", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            //ImGui::MenuItem("Load Model", nullptr, false, false);
            if (ImGui::MenuItem("Load Model")) {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
                if (result == NFD_OKAY) {
                    Engine::ModelManager::ImportModel(outPath);
                    free(outPath);
                } else if (result == NFD_CANCEL) {
                    std::cout << "err" << std::endl;
                } else {
                    std::cout << "unk err" << std::endl;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    //for (auto const [key, value] : pModelManager->GetModels()) {
        //ImGui::Text("Model [%d]: %s", value, key.c_str());
    //}

    ImGui::End();
}

void Interface::GetResourceManager(Engine::Scene* scene) const {
    ImGui::Begin("ResourceManager", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            //ImGui::MenuItem("Load Model", nullptr, false, false);
            if (ImGui::MenuItem("Load Texture")) {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
                if (result == NFD_OKAY) {
                    Engine::ResourceManager::CreateTexture(outPath);
                    free(outPath);
                } else if (result == NFD_CANCEL) {
                    std::cout << "err" << std::endl;
                } else {
                    std::cout << "unk err" << std::endl;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    for (auto const [key, value] : Engine::ResourceManager::MapTextures) {
        ImGui::Text("Texture [%d]: %s", value, key.c_str());
    }

    ImGui::End();
}

void Interface::Console(Engine::Scene* scene) const {
    ImGui::Begin("Console", nullptr, ImGuiWindowFlags_MenuBar);

    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    for (const std::string& log : Engine::GetLogs()) {
        ImGui::TextWrapped("%s", log.c_str());
    }

    ImGui::EndChild();

    ImGui::End();
}

void Interface::GetFiles() const {
    ImGui::Begin("Files");

    //static int selected = -1;
    //for (const auto& entry : std::filesystem::directory_iterator(pProject->GetPath())) {
        //if (ImGui::Selectable(entry.path().filename().string().c_str())) {
            ////system(("xdg-open " + entry.path().string()).c_str());
        //}

        //if (ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
        //{
            //selected = (int) entry.path().filename().string().size();
            //ImGui::Text("This a popup for \"%s\"!", entry.path().filename().string().c_str());
            //if (ImGui::Button("Close"))
                //ImGui::CloseCurrentPopup();
            //ImGui::EndPopup();
        //}
        //ImGui::SetItemTooltip("Right-click to open popup");
    //}


    ImGui::End();
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

    bool ope = true;

    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    if (opt_fullscreen) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    } else {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace Demo", nullptr, window_flags);

    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    Engine::Scene* scene = GetScene();

    if (pProject->refresh_files) {
        pProject->CompileFiles();
        std::cout << "Refresh files" << std::endl;
        pProject->refresh_files = false;
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New project")) {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_PickFolder(NULL, &outPath);
                if (result == NFD_OKAY) {
                    pProject = new Engine::Project(pWindow);
                    pProject->Init();
                    pProject->Create(outPath);
                    SetProject(pProject);

                    free(outPath);
                } else if (result == NFD_CANCEL) {
                    std::cout << "err" << std::endl;
                } else {
                    std::cout << "unk err" << std::endl;
                }
            }

            if (ImGui::MenuItem("Save project")) {
                pProject->Save();
            }

            if (ImGui::MenuItem("Load project")) {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_PickFolder(NULL, &outPath);
                if (result == NFD_OKAY) {
                    pProject = new Engine::Project(pWindow);
                    pProject->Init();
                    pProject->Load(outPath);
                    SetProject(pProject);
                    free(outPath);
                } else if (result == NFD_CANCEL) {
                    std::cout << "err" << std::endl;
                } else {
                    std::cout << "unk err" << std::endl;
                }
            }

            if (ImGui::MenuItem("Components Refresh")) {
                pProject->CompileFiles();
            }

            if (ImGui::MenuItem("Run")) {
                pProject->CompileAndRunApplication();
            }

            if (scene && ImGui::MenuItem("Load so file")) {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
                if (result == NFD_OKAY) {
                    pProject->IncludeFile(outPath);
                    free(outPath);
                } else if (result == NFD_CANCEL) {
                    std::cout << "err" << std::endl;
                } else {
                    std::cout << "unk err" << std::endl;
                }
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();

    //bool v = true;
    //ShowExampleAppAssetsBrowser(&v);

    if (scene != nullptr) {
        GetSceneInfo(scene);
        //GetCameraInfo(scene);
        GetObjectsInfo(scene);
        ObjectInspector(scene);
        Console(scene);
        //GetModelManager(scene);
    }

    //GetFiles();


        //ImGui::Begin("Debug");

        //GetDebugInfo();
        //Engine::Debug::CounterFPS();
        //GetConfigInfo();
        //ImGui::Separator();

        //// Scene issue?

        ////GetResourceManagerInfo();
        
        //ImGui::End();

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
