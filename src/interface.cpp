#include "interface.hpp"
#include "config.hpp"
#include "framebuffer.hpp"
#include "scene.hpp"
#include "transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <sstream>

#include <clocale>
#include <iostream>
#include <memory>

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
    pProject->Init();
}

void Interface::LoadProject() {
    SetProject(new Engine::Project(pWindow));
    if (pProject->Load("project.json")) {
        std::cout << "Project loaded" << std::endl;
    } else {
        std::cout << "Failed to load project" << std::endl;
    }
}

Engine::Scene* Interface::GetScene() const {
    return pProject->GetScene();
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

void Interface::GetConfigInfo() const {
    if (ImGui::CollapsingHeader("Config")) {
        ImGui::Checkbox("Interface", &Config::InterfaceDebugActive);
        if (ImGui::Checkbox("VSync", &Config::VSync)) {
            glfwSwapInterval(Config::VSync);
        };
    }
}

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

            std::cout << xoffset << " " << yoffset << std::endl;

            glfwSetCursorPos(pWindow->GetWindow(), (windowPos.x + windowSize.x) / 2, (windowPos.y + windowSize.y) / 2);

            Camera* pCamera = scene->GetActiveCamera();
            pCamera->ProcessMouseMovement(-xoffset, yoffset);
        }

        if (ImGui::IsMouseReleased(1)) { // Right mouse button released
            mouseCapture = false;
            glfwSetInputMode(pWindow->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Show the cursor again
        }


        ImGui::BeginChild("GameRender");

        float width = ImGui::GetContentRegionAvail().x;
        float height = ImGui::GetContentRegionAvail().y;

        ImVec2 wsize = ImGui::GetWindowSize();
        pFbo->RescaleFrameBuffer(wsize.x, wsize.y);

        ImGui::Image(
            (ImTextureID)pFbo->getFrameTexture(),
            wsize,
            ImVec2(0, 1),
            ImVec2(1, 0)
        );


        ImGui::EndChild();

    ImGui::End();
}

void Interface::GetCameraInfo(Engine::Scene* scene) const {
    ImGui::Begin("Camera");

    Camera* pCamera = scene->GetActiveCamera();

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

    if (ImGui::Button("Create object")) {
        scene->AddObject(new Renderer::Object("new object"));
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("List of objects")) {
        for (Renderer::Object* obj : scene->GetObjects()) {
            ImGui::PushID(obj);
            
            std::stringstream addressStream;
            addressStream << std::hex << reinterpret_cast<std::uintptr_t>(obj);
            std::string addressString = addressStream.str();

            std::string name = "Object: " + obj->name + " (Address: " + addressString + ")";

            if (ImGui::Selectable(name.c_str(), pSelectedObject == obj)) {
                pSelectedObject = obj;
            }
            ImGui::PopID();
        }

        ImGui::TreePop();
    }


    //if (ImGui::Button("Create object")) {
        //Renderer::Object* obj = new Renderer::Object("new object");
        //scene->AddObject(obj);
    //}

    //std::vector<Renderer::Object*> pObjects = scene->GetObjects();
    //for (int i = 0; i < pObjects.size(); i++) {
        //ImGui::Text("Object %d", pObjects.at(i));
        //ImGui::PushID(i);

        //if (pObjects.at(i)->GetModelInstance() != nullptr) {
            //if (ImGui::TreeNode("Model Instance")) {
                //for (glm::mat4 mat : pObjects.at(i)->GetModelInstance()->GetMatrixes()) {
                    //ImGui::Text("Matrix %d", invisible);
                    //ImGui::Text("%.6f %.6f %.6f %.6f", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
                    //ImGui::Text("%.6f %.6f %.6f %.6f", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
                    //ImGui::Text("%.6f %.6f %.6f %.6f", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
                    //ImGui::Text("%.6f %.6f %.6f %.6f", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
                    //invisible++;
                //}

                //ImGui::TreePop();
            //}
        //} else if (pObjects.at(i)->GetModel() != nullptr) {
            //if (ImGui::TreeNode("Model")) {
                //for (Vertex vertex : pObjects.at(i)->GetModel()->GetVertices()) {
                    //ImGui::Text("Vertex %d", invisible);
                    //ImGui::Text("position: %.6f %.6f %.6f", vertex.position.x, vertex.position.y, vertex.position.z);
                    //ImGui::Text("normal: %.6f %.6f %.6f", vertex.normal.x, vertex.normal.y, vertex.normal.z);
                    //ImGui::Text("texcoord: %.6f %.6f", vertex.texCoord.x, vertex.texCoord.y);
                    //invisible++;
                //}
                //ImGui::TreePop();
            //}
        //}

        //if (ImGui::TreeNode("Position")) {
            //ImGui::DragFloat("x", &pObjects.at(i)->position.x, 0.05f);
            //ImGui::DragFloat("y", &pObjects.at(i)->position.y, 0.05f);
            //ImGui::DragFloat("z", &pObjects.at(i)->position.z, 0.05f);
            //pObjects.at(i)->UpdatePosition();
            //ImGui::TreePop();
        //}

        //ImGui::PopID(); 
    //}

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

        if (ImGui::Button("Add component"))
            ImGui::OpenPopup("my_select_popup");

        if (ImGui::BeginPopup("my_select_popup")) {
            for (const auto& [key, value] : scene->pComponentManager->GetComponents()) {
                if (ImGui::Selectable(key.c_str())) {
                    pSelectedObject->AddComponent(key);
                }
            }

            ImGui::EndPopup();
        }

        for (Engine::Component* pComponent : pSelectedObject->GetComponents()) {
            ImGui::SetNextItemOpen(true, ImGuiCond_Once);

            std::stringstream addressStream;
            addressStream << std::hex << reinterpret_cast<std::uintptr_t>(pComponent);
            std::string addressString = addressStream.str();

            std::string name = pComponent->GetTypeName() + " (Address: " + addressString + ")";

            if (ImGui::TreeNode(name.c_str())) {
                for (const auto& var : pComponent->variables) {
                    if (var->type_name == "bool") {
                        ImGui::Checkbox(var->name, static_cast<bool*>(var->ptr()));
                    } else if (var->type_name == "glm::vec3") {
                        ImGui::SliderFloat3(var->name, glm::value_ptr(*static_cast<glm::vec3*>(var->ptr())), -180.0f, 180.0f);
                    } else if (var->type_name == "int") {
                        ImGui::DragInt(var->name, static_cast<int*>(var->ptr()));
                    } else if (var->type_name == "float") {
                        ImGui::DragFloat(var->name, static_cast<float*>(var->ptr()), 0.01f);
                    } else {
                        ImGui::Text((var->type_name + var->name).c_str());
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
    Renderer::ModelManager* pModelManager = scene->GetModelManager();

    ImGui::Begin("ModelManager", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            //ImGui::MenuItem("Load Model", nullptr, false, false);
            if (ImGui::MenuItem("Load Model")) {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
                if (result == NFD_OKAY) {
                    pModelManager->ImportModel(outPath);
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
    Renderer::ResourceManager* pResourceManager = scene->GetResourceManager();

    ImGui::Begin("ResourceManager", nullptr, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            //ImGui::MenuItem("Load Model", nullptr, false, false);
            if (ImGui::MenuItem("Load Texture")) {
                nfdchar_t *outPath = NULL;
                nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
                if (result == NFD_OKAY) {
                    pResourceManager->CreateTexture(outPath);
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

    for (auto const [key, value] : pResourceManager->MapTextures) {
        ImGui::Text("Texture [%d]: %s", value, key.c_str());
    }

    ImGui::End();
}

void Interface::GetFiles() const {
    ImGui::Begin("Files");

    for (const auto& file : pProject->GetFiles()) {
        ImGui::Text(file.c_str());
    }

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

struct ExampleSelectionWithDeletion : ImGuiSelectionBasicStorage
{
    // Find which item should be Focused after deletion.
    // Call _before_ item submission. Retunr an index in the before-deletion item list, your item loop should call SetKeyboardFocusHere() on it.
    // The subsequent ApplyDeletionPostLoop() code will use it to apply Selection.
    // - We cannot provide this logic in core Dear ImGui because we don't have access to selection data.
    // - We don't actually manipulate the ImVector<> here, only in ApplyDeletionPostLoop(), but using similar API for consistency and flexibility.
    // - Important: Deletion only works if the underlying ImGuiID for your items are stable: aka not depend on their index, but on e.g. item id/ptr.
    // FIXME-MULTISELECT: Doesn't take account of the possibility focus target will be moved during deletion. Need refocus or scroll offset.
    int ApplyDeletionPreLoop(ImGuiMultiSelectIO* ms_io, int items_count)
    {
        if (Size == 0)
            return -1;

        // If focused item is not selected...
        const int focused_idx = (int)ms_io->NavIdItem;  // Index of currently focused item
        if (ms_io->NavIdSelected == false)  // This is merely a shortcut, == Contains(adapter->IndexToStorage(items, focused_idx))
        {
            ms_io->RangeSrcReset = true;    // Request to recover RangeSrc from NavId next frame. Would be ok to reset even when NavIdSelected==true, but it would take an extra frame to recover RangeSrc when deleting a selected item.
            return focused_idx;             // Request to focus same item after deletion.
        }

        // If focused item is selected: land on first unselected item after focused item.
        for (int idx = focused_idx + 1; idx < items_count; idx++)
            if (!Contains(GetStorageIdFromIndex(idx)))
                return idx;

        // If focused item is selected: otherwise return last unselected item before focused item.
        for (int idx = IM_MIN(focused_idx, items_count) - 1; idx >= 0; idx--)
            if (!Contains(GetStorageIdFromIndex(idx)))
                return idx;

        return -1;
    }

    // Rewrite item list (delete items) + update selection.
    // - Call after EndMultiSelect()
    // - We cannot provide this logic in core Dear ImGui because we don't have access to your items, nor to selection data.
    template<typename ITEM_TYPE>
    void ApplyDeletionPostLoop(ImGuiMultiSelectIO* ms_io, ImVector<ITEM_TYPE>& items, int item_curr_idx_to_select)
    {
        // Rewrite item list (delete items) + convert old selection index (before deletion) to new selection index (after selection).
        // If NavId was not part of selection, we will stay on same item.
        ImVector<ITEM_TYPE> new_items;
        new_items.reserve(items.Size - Size);
        int item_next_idx_to_select = -1;
        for (int idx = 0; idx < items.Size; idx++)
        {
            if (!Contains(GetStorageIdFromIndex(idx)))
                new_items.push_back(items[idx]);
            if (item_curr_idx_to_select == idx)
                item_next_idx_to_select = new_items.Size - 1;
        }
        items.swap(new_items);

        // Update selection
        Clear();
        if (item_next_idx_to_select != -1 && ms_io->NavIdSelected)
            SetItemSelected(GetStorageIdFromIndex(item_next_idx_to_select), true);
    }
};

struct Asset
{
    ImGuiID ID;
    int     Type;

    Asset(ImGuiID id, int type) { ID = id; Type = type; }

    static const ImGuiTableSortSpecs* s_current_sort_specs;

    static void SortWithSortSpecs(ImGuiTableSortSpecs* sort_specs, Asset* items, int items_count)
    {
        s_current_sort_specs = sort_specs; // Store in variable accessible by the sort function.
        if (items_count > 1)
            qsort(items, (size_t)items_count, sizeof(items[0]), Asset::CompareWithSortSpecs);
        s_current_sort_specs = NULL;
    }

    // Compare function to be used by qsort()
    static int CompareWithSortSpecs(const void* lhs, const void* rhs)
    {
        const Asset* a = (const Asset*)lhs;
        const Asset* b = (const Asset*)rhs;
        for (int n = 0; n < s_current_sort_specs->SpecsCount; n++)
        {
            const ImGuiTableColumnSortSpecs* sort_spec = &s_current_sort_specs->Specs[n];
            int delta = 0;
            if (sort_spec->ColumnIndex == 0)
                delta = ((int)a->ID - (int)b->ID);
            else if (sort_spec->ColumnIndex == 1)
                delta = (a->Type - b->Type);
            if (delta > 0)
                return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? +1 : -1;
            if (delta < 0)
                return (sort_spec->SortDirection == ImGuiSortDirection_Ascending) ? -1 : +1;
        }
        return ((int)a->ID - (int)b->ID);
    }
};
const ImGuiTableSortSpecs* Asset::s_current_sort_specs = NULL;

struct ExampleAssetsBrowser
{
    // Options
    bool            ShowTypeOverlay = true;
    bool            AllowSorting = true;
    bool            AllowDragUnselected = false;
    bool            AllowBoxSelect = true;
    float           IconSize = 32.0f;
    int             IconSpacing = 10;
    int             IconHitSpacing = 4;         // Increase hit-spacing if you want to make it possible to clear or box-select from gaps. Some spacing is required to able to amend with Shift+box-select. Value is small in Explorer.
    bool            StretchSpacing = true;

    // State
    ImVector<Asset> Items;               // Our items
    ExampleSelectionWithDeletion Selection;     // Our selection (ImGuiSelectionBasicStorage + helper funcs to handle deletion)
    ImGuiID         NextItemId = 0;             // Unique identifier when creating new items
    bool            RequestDelete = false;      // Deferred deletion request
    bool            RequestSort = false;        // Deferred sort request
    float           ZoomWheelAccum = 0.0f;      // Mouse wheel accumulator to handle smooth wheels better

    // Calculated sizes for layout, output of UpdateLayoutSizes(). Could be locals but our code is simpler this way.
    ImVec2          LayoutItemSize;
    ImVec2          LayoutItemStep;             // == LayoutItemSize + LayoutItemSpacing
    float           LayoutItemSpacing = 0.0f;
    float           LayoutSelectableSpacing = 0.0f;
    float           LayoutOuterPadding = 0.0f;
    int             LayoutColumnCount = 0;
    int             LayoutLineCount = 0;

    // Functions
    ExampleAssetsBrowser()
    {
        AddItems(10000);
    }
    void AddItems(int count)
    {
        if (Items.Size == 0)
            NextItemId = 0;
        Items.reserve(Items.Size + count);
        for (int n = 0; n < count; n++, NextItemId++)
            Items.push_back(Asset(NextItemId, (NextItemId % 20) < 15 ? 0 : (NextItemId % 20) < 18 ? 1 : 2));
        RequestSort = true;
    }
    void ClearItems()
    {
        Items.clear();
        Selection.Clear();
    }

    // Logic would be written in the main code BeginChild() and outputing to local variables.
    // We extracted it into a function so we can call it easily from multiple places.
    void UpdateLayoutSizes(float avail_width)
    {
        // Layout: when not stretching: allow extending into right-most spacing.
        LayoutItemSpacing = (float)IconSpacing;
        if (StretchSpacing == false)
            avail_width += floorf(LayoutItemSpacing * 0.5f);

        // Layout: calculate number of icon per line and number of lines
        LayoutItemSize = ImVec2(floorf(IconSize), floorf(IconSize));
        LayoutColumnCount = IM_MAX((int)(avail_width / (LayoutItemSize.x + LayoutItemSpacing)), 1);
        LayoutLineCount = (Items.Size + LayoutColumnCount - 1) / LayoutColumnCount;

        // Layout: when stretching: allocate remaining space to more spacing. Round before division, so item_spacing may be non-integer.
        if (StretchSpacing && LayoutColumnCount > 1)
            LayoutItemSpacing = floorf(avail_width - LayoutItemSize.x * LayoutColumnCount) / LayoutColumnCount;

        LayoutItemStep = ImVec2(LayoutItemSize.x + LayoutItemSpacing, LayoutItemSize.y + LayoutItemSpacing);
        LayoutSelectableSpacing = IM_MAX(floorf(LayoutItemSpacing) - IconHitSpacing, 0.0f);
        LayoutOuterPadding = floorf(LayoutItemSpacing * 0.5f);
    }

    void Draw(const char* title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(IconSize * 25, IconSize * 15), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::End();
            return;
        }

        // Menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Add 10000 items"))
                    AddItems(10000);
                if (ImGui::MenuItem("Clear items"))
                    ClearItems();
                ImGui::Separator();
                if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                    *p_open = false;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Delete", "Del", false, Selection.Size > 0))
                    RequestDelete = true;
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Options"))
            {
                ImGui::PushItemWidth(ImGui::GetFontSize() * 10);

                ImGui::SeparatorText("Contents");
                ImGui::Checkbox("Show Type Overlay", &ShowTypeOverlay);
                ImGui::Checkbox("Allow Sorting", &AllowSorting);

                ImGui::SeparatorText("Selection Behavior");
                ImGui::Checkbox("Allow dragging unselected item", &AllowDragUnselected);
                ImGui::Checkbox("Allow box-selection", &AllowBoxSelect);

                ImGui::SeparatorText("Layout");
                ImGui::SliderFloat("Icon Size", &IconSize, 16.0f, 128.0f, "%.0f");
                ImGui::SliderInt("Icon Spacing", &IconSpacing, 0, 32);
                ImGui::SliderInt("Icon Hit Spacing", &IconHitSpacing, 0, 32);
                ImGui::Checkbox("Stretch Spacing", &StretchSpacing);
                ImGui::PopItemWidth();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Show a table with ONLY one header row to showcase the idea/possibility of using this to provide a sorting UI
        if (AllowSorting)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            ImGuiTableFlags table_flags_for_sort_specs = ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders;
            if (ImGui::BeginTable("for_sort_specs_only", 2, table_flags_for_sort_specs, ImVec2(0.0f, ImGui::GetFrameHeight())))
            {
                ImGui::TableSetupColumn("Index");
                ImGui::TableSetupColumn("Type");
                ImGui::TableHeadersRow();
                if (ImGuiTableSortSpecs* sort_specs = ImGui::TableGetSortSpecs())
                    if (sort_specs->SpecsDirty || RequestSort)
                    {
                        Asset::SortWithSortSpecs(sort_specs, Items.Data, Items.Size);
                        sort_specs->SpecsDirty = RequestSort = false;
                    }
                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        }

        ImGuiIO& io = ImGui::GetIO();
        ImGui::SetNextWindowContentSize(ImVec2(0.0f, LayoutOuterPadding + LayoutLineCount * (LayoutItemSize.x + LayoutItemSpacing)));
        if (ImGui::BeginChild("Assets", ImVec2(0.0f, -ImGui::GetTextLineHeightWithSpacing()), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoMove))
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            const float avail_width = ImGui::GetContentRegionAvail().x;
            UpdateLayoutSizes(avail_width);

            // Calculate and store start position.
            ImVec2 start_pos = ImGui::GetCursorScreenPos();
            start_pos = ImVec2(start_pos.x + LayoutOuterPadding, start_pos.y + LayoutOuterPadding);
            ImGui::SetCursorScreenPos(start_pos);

            // Multi-select
            ImGuiMultiSelectFlags ms_flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_ClearOnClickVoid;

            // - Enable box-select (in 2D mode, so that changing box-select rectangle X1/X2 boundaries will affect clipped items)
            if (AllowBoxSelect)
                ms_flags |= ImGuiMultiSelectFlags_BoxSelect2d;

            // - This feature allows dragging an unselected item without selecting it (rarely used)
            if (AllowDragUnselected)
                ms_flags |= ImGuiMultiSelectFlags_SelectOnClickRelease;

            // - Enable keyboard wrapping on X axis
            // (FIXME-MULTISELECT: We haven't designed/exposed a general nav wrapping api yet, so this flag is provided as a courtesy to avoid doing:
            //    ImGui::NavMoveRequestTryWrapping(ImGui::GetCurrentWindow(), ImGuiNavMoveFlags_WrapX);
            // When we finish implementing a more general API for this, we will obsolete this flag in favor of the new system)
            ms_flags |= ImGuiMultiSelectFlags_NavWrapX;

            ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(ms_flags, Selection.Size, Items.Size);

            // Use custom selection adapter: store ID in selection (recommended)
            Selection.UserData = this;
            Selection.AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self_, int idx) { ExampleAssetsBrowser* self = (ExampleAssetsBrowser*)self_->UserData; return self->Items[idx].ID; };
            Selection.ApplyRequests(ms_io);

            const bool want_delete = (ImGui::Shortcut(ImGuiKey_Delete, ImGuiInputFlags_Repeat) && (Selection.Size > 0)) || RequestDelete;
            const int item_curr_idx_to_focus = want_delete ? Selection.ApplyDeletionPreLoop(ms_io, Items.Size) : -1;
            RequestDelete = false;

            // Push LayoutSelectableSpacing (which is LayoutItemSpacing minus hit-spacing, if we decide to have hit gaps between items)
            // Altering style ItemSpacing may seem unnecessary as we position every items using SetCursorScreenPos()...
            // But it is necessary for two reasons:
            // - Selectables uses it by default to visually fill the space between two items.
            // - The vertical spacing would be measured by Clipper to calculate line height if we didn't provide it explicitly (here we do).
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(LayoutSelectableSpacing, LayoutSelectableSpacing));

            // Rendering parameters
            const ImU32 icon_type_overlay_colors[3] = { 0, IM_COL32(200, 70, 70, 255), IM_COL32(70, 170, 70, 255) };
            const ImU32 icon_bg_color = ImGui::GetColorU32(ImGuiCol_MenuBarBg);
            const ImVec2 icon_type_overlay_size = ImVec2(4.0f, 4.0f);
            const bool display_label = (LayoutItemSize.x >= ImGui::CalcTextSize("999").x);

            const int column_count = LayoutColumnCount;
            ImGuiListClipper clipper;
            clipper.Begin(LayoutLineCount, LayoutItemStep.y);
            if (item_curr_idx_to_focus != -1)
                clipper.IncludeItemByIndex(item_curr_idx_to_focus / column_count); // Ensure focused item line is not clipped.
            if (ms_io->RangeSrcItem != -1)
                clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem / column_count); // Ensure RangeSrc item line is not clipped.
            while (clipper.Step())
            {
                for (int line_idx = clipper.DisplayStart; line_idx < clipper.DisplayEnd; line_idx++)
                {
                    const int item_min_idx_for_current_line = line_idx * column_count;
                    const int item_max_idx_for_current_line = IM_MIN((line_idx + 1) * column_count, Items.Size);
                    for (int item_idx = item_min_idx_for_current_line; item_idx < item_max_idx_for_current_line; ++item_idx)
                    {
                        Asset* item_data = &Items[item_idx];
                        ImGui::PushID((int)item_data->ID);

                        // Position item
                        ImVec2 pos = ImVec2(start_pos.x + (item_idx % column_count) * LayoutItemStep.x, start_pos.y + line_idx * LayoutItemStep.y);
                        ImGui::SetCursorScreenPos(pos);

                        ImGui::SetNextItemSelectionUserData(item_idx);
                        bool item_is_selected = Selection.Contains((ImGuiID)item_data->ID);
                        bool item_is_visible = ImGui::IsRectVisible(LayoutItemSize);
                        ImGui::Selectable("", item_is_selected, ImGuiSelectableFlags_None, LayoutItemSize);

                        // Update our selection state immediately (without waiting for EndMultiSelect() requests)
                        // because we use this to alter the color of our text/icon.
                        if (ImGui::IsItemToggledSelection())
                            item_is_selected = !item_is_selected;

                        // Focus (for after deletion)
                        if (item_curr_idx_to_focus == item_idx)
                            ImGui::SetKeyboardFocusHere(-1);

                        // Drag and drop
                        if (ImGui::BeginDragDropSource())
                        {
                            // Create payload with full selection OR single unselected item.
                            // (the later is only possible when using ImGuiMultiSelectFlags_SelectOnClickRelease)
                            if (ImGui::GetDragDropPayload() == NULL)
                            {
                                ImVector<ImGuiID> payload_items;
                                void* it = NULL;
                                ImGuiID id = 0;
                                if (!item_is_selected)
                                    payload_items.push_back(item_data->ID);
                                else
                                    while (Selection.GetNextSelectedItem(&it, &id))
                                        payload_items.push_back(id);
                                ImGui::SetDragDropPayload("ASSETS_BROWSER_ITEMS", payload_items.Data, (size_t)payload_items.size_in_bytes());
                            }

                            // Display payload content in tooltip, by extracting it from the payload data
                            // (we could read from selection, but it is more correct and reusable to read from payload)
                            const ImGuiPayload* payload = ImGui::GetDragDropPayload();
                            const int payload_count = (int)payload->DataSize / (int)sizeof(ImGuiID);
                            ImGui::Text("%d assets", payload_count);

                            ImGui::EndDragDropSource();
                        }

                        // Render icon (a real app would likely display an image/thumbnail here)
                        // Because we use ImGuiMultiSelectFlags_BoxSelect2d, clipping vertical may occasionally be larger, so we coarse-clip our rendering as well.
                        if (item_is_visible)
                        {
                            ImVec2 box_min(pos.x - 1, pos.y - 1);
                            ImVec2 box_max(box_min.x + LayoutItemSize.x + 2, box_min.y + LayoutItemSize.y + 2); // Dubious
                            draw_list->AddRectFilled(box_min, box_max, icon_bg_color); // Background color
                            if (ShowTypeOverlay && item_data->Type != 0)
                            {
                                ImU32 type_col = icon_type_overlay_colors[item_data->Type % IM_ARRAYSIZE(icon_type_overlay_colors)];
                                draw_list->AddRectFilled(ImVec2(box_max.x - 2 - icon_type_overlay_size.x, box_min.y + 2), ImVec2(box_max.x - 2, box_min.y + 2 + icon_type_overlay_size.y), type_col);
                            }
                            if (display_label)
                            {
                                ImU32 label_col = ImGui::GetColorU32(item_is_selected ? ImGuiCol_Text : ImGuiCol_TextDisabled);
                                char label[32];
                                sprintf(label, "%d", item_data->ID);
                                draw_list->AddText(ImVec2(box_min.x, box_max.y - ImGui::GetFontSize()), label_col, label);
                            }
                        }

                        ImGui::PopID();
                    }
                }
            }
            clipper.End();
            ImGui::PopStyleVar(); // ImGuiStyleVar_ItemSpacing

            // Context menu
            if (ImGui::BeginPopupContextWindow())
            {
                ImGui::Text("Selection: %d items", Selection.Size);
                ImGui::Separator();
                if (ImGui::MenuItem("Delete", "Del", false, Selection.Size > 0))
                    RequestDelete = true;
                ImGui::EndPopup();
            }

            ms_io = ImGui::EndMultiSelect();
            Selection.ApplyRequests(ms_io);
            if (want_delete)
                Selection.ApplyDeletionPostLoop(ms_io, Items, item_curr_idx_to_focus);

            // Zooming with CTRL+Wheel
            if (ImGui::IsWindowAppearing())
                ZoomWheelAccum = 0.0f;
            if (ImGui::IsWindowHovered() && io.MouseWheel != 0.0f && ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsAnyItemActive() == false)
            {
                ZoomWheelAccum += io.MouseWheel;
                if (fabsf(ZoomWheelAccum) >= 1.0f)
                {
                    // Calculate hovered item index from mouse location
                    // FIXME: Locking aiming on 'hovered_item_idx' (with a cool-down timer) would ensure zoom keeps on it.
                    const float hovered_item_nx = (io.MousePos.x - start_pos.x + LayoutItemSpacing * 0.5f) / LayoutItemStep.x;
                    const float hovered_item_ny = (io.MousePos.y - start_pos.y + LayoutItemSpacing * 0.5f) / LayoutItemStep.y;
                    const int hovered_item_idx = ((int)hovered_item_ny * LayoutColumnCount) + (int)hovered_item_nx;
                    //ImGui::SetTooltip("%f,%f -> item %d", hovered_item_nx, hovered_item_ny, hovered_item_idx); // Move those 4 lines in block above for easy debugging

                    // Zoom
                    IconSize *= powf(1.1f, (float)(int)ZoomWheelAccum);
                    IconSize = IM_CLAMP(IconSize, 16.0f, 128.0f);
                    ZoomWheelAccum -= (int)ZoomWheelAccum;
                    UpdateLayoutSizes(avail_width);

                    // Manipulate scroll to that we will land at the same Y location of currently hovered item.
                    // - Calculate next frame position of item under mouse
                    // - Set new scroll position to be used in next ImGui::BeginChild() call.
                    float hovered_item_rel_pos_y = ((float)(hovered_item_idx / LayoutColumnCount) + fmodf(hovered_item_ny, 1.0f)) * LayoutItemStep.y;
                    hovered_item_rel_pos_y += ImGui::GetStyle().WindowPadding.y;
                    float mouse_local_y = io.MousePos.y - ImGui::GetWindowPos().y;
                    ImGui::SetScrollY(hovered_item_rel_pos_y - mouse_local_y);
                }
            }
        }
        ImGui::EndChild();

        ImGui::Text("Selected: %d/%d items", Selection.Size, Items.Size);
        ImGui::End();
    }
};

void ShowExampleAppAssetsBrowser(bool* p_open)
{
    static ExampleAssetsBrowser assets_browser;
    assets_browser.Draw("Example: Assets Browser", p_open);
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

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New project")) {
                SetProject(new Engine::Project(pWindow));
            }

            if (ImGui::MenuItem("Save project")) {
                pProject->Save("project.json");
            }

            if (ImGui::MenuItem("Load project")) {
                SetProject(new Engine::Project(pWindow));
                if (pProject->Load("project.json")) {
                    std::cout << "Loaded project project.json" << std::endl;
                }
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

    bool v = true;
    ShowExampleAppAssetsBrowser(&v);

    if (scene != nullptr) {
        GetSceneInfo(scene);
        GetCameraInfo(scene);
        GetObjectsInfo(scene);
        ObjectInspector(scene);
        GetModelManager(scene);
    }

    GetFiles();


    if (Config::InterfaceDebugActive) {
        ImGui::Begin("Debug");

        GetDebugInfo();
        pDebug->CounterFPS();
        GetConfigInfo();
        ImGui::Separator();

        // Scene issue?

        //GetResourceManagerInfo();
        
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
