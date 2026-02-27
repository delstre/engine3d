#include <boost/concept_check.hpp>
#include <icomponent.hpp>
#include <model.hpp>
#include <transform.hpp>
#include <project.hpp>
#include <scene.hpp>
#include <window.hpp>
#include <componentmanager.hpp>
#include <modelmanager.hpp>
#include <executer.hpp>
#include <archive.hpp>

#include <fstream>
#include <filesystem>
#include <iostream>
#include <thread>
#include <map>

#include <dlfcn.h>
#include <cxxabi.h>
#include <sys/stat.h>

#include <boost/json/src.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

using namespace Engine;

boost::json::object ObjectToJSON(Engine::Object* obj) {
    boost::json::object json_obj;
    json_obj["name"] = boost::json::value(obj->name);
    
    auto transform = obj->GetComponent<Transform>();
    if (transform) {
        glm::vec3 pos = transform->GetPosition();
        glm::vec3 rot = transform->GetAngle();

        json_obj["position"] = boost::json::value({pos.x, pos.y, pos.z});
        json_obj["angle"] = boost::json::value({rot.x, rot.y, rot.z});
    }

    return json_obj;
}

Project::Project(Window* pWindow) {
    this->pWindow = pWindow;
}

/*
    Инициализация проекта
    Создается сцена и добавляется в окно
    
    std::string path : путь до проекта
*/
void Project::Init(const std::string& path) {
    ModelManager::SetPath(path);
    ModelManager::ImportAllModels();

    pScene = new Scene();    
    pScene->SetPath(path);
    pWindow->SceneInit(pScene);
}

void Project::Update() {
    try {
        std::filesystem::copy_file(ENGINE_PATH "/libengine.so", folder_path + "/libengine.so", std::filesystem::copy_options::overwrite_existing);
        std::filesystem::copy(ENGINE_PATH "/include", folder_path + "/include", std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
        std::filesystem::copy(ENGINE_PATH "/shaders", folder_path + "/shaders", std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
        std::filesystem::copy(ENGINE_PATH "/models", folder_path + "/models", std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error to copy: " << e.what() << std::endl;
        return;
    }
}

void Project::Create(const std::string& path) {
    folder_path = path;
    Update();

    std::ofstream outFile(path + "/Makefile", std::ios::out);
    if (!outFile) {
        std::cerr << "Error to create!" << std::endl;
        return;
    }

    outFile << "CXX = g++\n";
    outFile << "CXXFLAGS = -Iinclude -L. -lGL -lIL -lGLEW -lglfw -lengine -Wl,-rpath=. -fPIC\n";
    outFile << "SRCS = $(wildcard *.cpp)\n";
    outFile << "SOFILES = $(SRCS:.cpp=.so)\n";
    outFile << "TARGET = program\n";
    outFile << "all: $(SOFILES)\n";
    outFile << "execute: $(TARGET)\n";
    outFile << "\t./$(TARGET)\n";
    outFile << "%.so: %.cpp\n";
    outFile << "\t$(CXX) -shared -o $@ $^ $(CXXFLAGS)\n";
    outFile << "$(TARGET): compile_program.cpp\n";
    outFile << "\t$(CXX) compile_program.cpp -o $(TARGET) $(CXXFLAGS) && ./$(TARGET)\n";
    outFile << "clean:\n";
    outFile << "\trm -f $(SOFILES)\n";

    outFile.close();

    std::ofstream json_file(path + "/project.json", std::ios::out);
    json_file.close();

    std::ofstream compile_file(path + "/compile_program.cpp");
    if (!compile_file) {
        std::cerr << "Error to compile!" << std::endl;
        return;
    }

    compile_file << "#include <window.hpp>\n";
    compile_file << "int main() {\n";
    compile_file << "\tEngine::Window window(1280, 720, \"Game Window\");\n";
    compile_file << "\twindow.Init();\n";
    compile_file << "\t// code ?;\n";
    compile_file << "\twindow.Render();\n";
    compile_file << "\treturn 0;\n";
    compile_file << "}\n";

    compile_file .close();

    Init(path);
}

void Project::Save() {
    std::ofstream outFile(folder_path + "/scene.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Error to save!" << std::endl;
        return;
    }

    OArchive ar(outFile);
    ar << *pScene;
    outFile.close();

    std::cout << "Saved" << std::endl;
}

void Project::CompileFiles(bool DeleteFile) {
    if (DeleteFile) {
        Engine::Execute(("make -C" + GetPath() + " clean").c_str());
    }
    Engine::Execute(("make -j16 -C" + GetPath()).c_str());
    IncludeFiles(); 
}

void Project::CompileAndRunApplication() {
    std::thread t([&]() {
        system(("make -C" + GetPath() + " execute ").c_str());
        return 0;
    });
    t.detach();
}

bool Project::Load(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << "No path! : " << path << std::endl;
        return false;
    }

    std::string project_file = path + "/project.json";

    if (!std::filesystem::exists(project_file)) {
        std::cerr << "No project file! : " << project_file << std::endl;
        return false;
    }

    std::ifstream json_file(project_file);
    if (!json_file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return false;
    }

    folder_path = path;

    Init(path);

    #ifdef INTERFACE_DEBUG
        std::cout << "Project path: " << path << std::endl;

        /*

        std::mutex mtx;
        std::thread check_files([&]() {
            std::map<std::string, struct stat> last_write_times;

            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                if (entry.is_regular_file() && entry.path().extension() == ".cpp") {
                    struct stat fileStat;
                    if (stat(entry.path().c_str(), &fileStat) != 0) {
                        std::cerr << "Не удалось получить информацию о файле: " << entry.path() << std::endl;
                        return;
                    }

                    last_write_times[entry.path()] = fileStat;
                }
            }

            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));

                for (auto& [filename, mod] : last_write_times) {
                    struct stat fileStat;

                    if (stat(filename.c_str(), &fileStat) != 0) {
                        std::cerr << "Не удалось получить информацию о файле: " << filename << std::endl;
                        continue;
                    }

                    if (mod.st_mtime != fileStat.st_mtime) {
                        std::cout << "File " << filename << " was modified" << std::endl;
                        mod = fileStat;
                        this->refresh_files = true;
                        break;
                    }
                }
            }
        });
        check_files.detach();
        */
    #endif


    #ifdef INTERFACE_DEBUG
    //CompileFiles(true);
    #endif

    std::ifstream scene_file(path + "/scene.bin", std::ios::binary);
    if (scene_file) {
        IArchive ia(scene_file);
        ia >> *pScene;
        scene_file.close();
    } else {
        std::cerr << "Failed to load scene" << std::endl;
    }

    return true;
}

static std::map<std::string, void*> handles;
bool Project::IncludeFile(const std::string& path) {
    setenv("LD_BIND_NOW", "1", 1);

    if (handles.find(path) != handles.end()) {
        dlclose(handles[path]);
        handles.erase(path);
        std::cout << "Closed library: " << path << std::endl;
    }

    void* handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL | RTLD_DEEPBIND);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << std::endl;
        return false;
    } else {
        std::cout << "Opened library: " << path << std::endl;
    }

    dlerror();

    using create_t = Component* (*)();
    using destroy_t = void (*)(void*);

    create_t create = (create_t) dlsym(handle, "Create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol create: " << dlsym_error << std::endl;
        dlclose(handle);
        handle = nullptr;
        return false;
    }

    std::cout << create << std::endl;

    if (pScene == nullptr) {
        std::cerr << "No scene!" << std::endl;
        dlclose(handle);
        return false;
    }

    Component* pComponent = create();
    if (pComponent == nullptr) {
        std::cerr << "Failed to create component" << std::endl;
        dlclose(handle);
        handle = nullptr;
        return false;
    }

    Engine::ComponentManager::RegisterComponent(pComponent->GetTypeName(), [create] {
        return create();
    }); 

    handles[path] = handle;

    delete pComponent;

    return true;
}

void Project::IncludeFiles() {
    if (!std::filesystem::exists(folder_path) || !std::filesystem::is_directory(folder_path)) {
        std::cout << "No path! : " << folder_path << std::endl;
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".so") {
            if (!IncludeFile(entry.path().string())) {
                std::cerr << "Failed to include file: " << entry.path().string() << std::endl;
            };
        }
    }
}

Scene* Project::GetScene() {
    return pScene;
}

const std::string& Project::GetPath() {
    return folder_path;
}
