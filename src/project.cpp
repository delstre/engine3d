#include <boost/concept_check.hpp>
#include <icomponent.hpp>
#include <transform.hpp>
#include <project.hpp>

#include <fstream>
#include <filesystem>
#include <boost/json/src.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <cxxabi.h>
#include <thread>
#include <sys/stat.h>

using namespace Engine;

boost::json::object ObjectToJSON(Renderer::Object* obj) {
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

void Project::Init() {
    pScene = new Scene();    
    pWindow->SceneInit(pScene);
}

void Project::SetPath(std::string path) {
    this->path = path;
    pScene = new Scene();    
    pScene->SetPath(path);
    pWindow->SceneInit(pScene);
}

void Project::Create(std::string path) {
    try {
        std::filesystem::copy_file("./libengine.so", path + "/libengine.so", std::filesystem::copy_options::overwrite_existing);
        std::filesystem::copy("../include", path + "/include", std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error to copy: " << e.what() << std::endl;
        return;
    }

    std::ofstream outFile(path + "/Makefile", std::ios::out);
    if (!outFile) {
        std::cerr << "Error to create!" << std::endl;
        return;
    }

    outFile << "CXX = g++\n";
    outFile << "CXXFLAGS = -Iinclude -L. -lGL -lIL -lGLEW -lglfw -lengine -Wl,-rpath=. -fPIC\n";
    outFile << "SRCS = $(wildcard *.cpp)\n";
    outFile << "SOFILES = $(SRCS:.cpp=.so)\n";
    //outFile << "OBJS = $(SRCS:.cpp=.o)\n";
    //outFile << "all: $(TARGET)\n";
    //outFile << "$(TARGET): $(OBJS)\n";
    //outFile << "\t$(CXX) -shared -o $@ $^ $(CXXFLAGS)\n";
    //outFile << "%.o: %.cpp\n";
    outFile << "all: $(SOFILES)\n";
    outFile << "%.so: %.cpp\n";
    outFile << "\t$(CXX) -shared -o $@ $^ $(CXXFLAGS)\n";
    outFile << "clean:\n";
    //outFile << "\trm -f $(TARGET) $(OBJS)\n";
    outFile << "\trm -f $(SOFILES)\n";

    outFile.close();

    std::ofstream json_file(path + "/project.json", std::ios::out);
    if (json_file.is_open()) {
        json_file << "{}";  // Write an empty JSON object to the file
        json_file.close();
        std::cout << "JSON data has been saved to project.json" << std::endl;
    } else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }

    SetPath(path);
}

void Project::Save() {
    boost::json::object json_obj;

    boost::json::array jobjs;

    for (Renderer::Object* obj : pScene->GetObjects()) {
        jobjs.push_back(boost::json::object(ObjectToJSON(obj)));
    }
    json_obj["objects"] = jobjs;

    std::string json_str = boost::json::serialize(json_obj);

    std::ofstream json_file(path + "/project.json", std::ios::out);
    if (json_file.is_open()) {
        json_file << json_str;  // Write the serialized JSON to the file
        json_file.close();
        std::cout << "JSON data has been saved to project.json" << std::endl;
    } else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }
}

void Project::CompileFiles() {
    system(("make -j16 -C" + GetPath()).c_str());
    IncludeFiles(); 
}

void Project::CompileAndRunApplication() {
    std::thread t([&]() {
        std::ofstream outFile("compile_program.cpp");
        if (!outFile) {
            std::cerr << "Error to compile!" << std::endl;
            return 1;
        }

        outFile << "#include <window.hpp>\n";
        outFile << "int main() {\n";
        outFile << "Engine::Window window(1280, 720, \"Game Window\");\n";
        outFile << "window.Init();\n";
        outFile << "return 0;\n";
        outFile << "}\n";

        outFile.close();

        std::string command = "g++ -L. -I../include -lGL -lIL -lGLEW -lglfw -lengine -Wl,-rpath=. compile_program.cpp -o " + GetPath() + "/program && cd " + GetPath() + " && ./program";

        system(command.c_str());

        system("rm compile_program.cpp");
        return 0;
    });
    t.detach();
}

void Project::Load() {
    std::cout << "Loading project..." << std::endl;
    system("pwd");

    if (Load(".")) {
        std::cout << "Project loaded" << std::endl;
    }
}

bool Project::Load(std::string path) {
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

    SetPath(path);

    #ifdef INTERFACE_DEBUG
        std::cout << "Project path: " << path << std::endl;

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
                        continue; // Если файл недоступен, продолжаем цикл
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
    #endif

    std::string json_str((std::istreambuf_iterator<char>(json_file)),
                         std::istreambuf_iterator<char>());

    boost::json::error_code ec;
    boost::json::value json_val = boost::json::parse(json_str, ec);

    if (ec) {
        std::cerr << "Failed to parse JSON: " << ec.message() << std::endl;
        return false;
    }

    boost::json::object json_obj = json_val.as_object();

    IncludeFiles();

    if (auto* objects_value = json_obj.if_contains("objects")) {
        if (objects_value->is_array()) {
            boost::json::array jobjs = objects_value->as_array();

            for (const auto& jobj : jobjs) {
                if (jobj.is_object()) {
                    boost::json::object rjobj = jobj.as_object();
                    Renderer::Object* obj = new Renderer::Object(rjobj["name"].as_string().c_str());
                    pScene->AddObject(obj);

                    boost::json::array pos = rjobj["position"].as_array();
                    obj->GetComponent<Transform>()->SetPosition(glm::vec3(pos[0].as_double(), pos[1].as_double(), pos[2].as_double()));
                    boost::json::array ang = rjobj["angle"].as_array();
                    obj->GetComponent<Transform>()->SetAngle(glm::vec3(ang[0].as_double(), ang[1].as_double(), ang[2].as_double()));
                }
            }
        }
    }


    std::ifstream recent_file("recently_used.json");
    if (!recent_file.is_open()) {
        boost::json::object obj;
        boost::json::array files;

        files.push_back(boost::json::value(path));
        obj["files"] = files;

        std::ofstream json_file("recently_used.json");
        if (json_file.is_open()) {
            json_file << boost::json::serialize(obj);
            json_file.close();
        } else {
            std::cerr << "Unable to open file for writing" << std::endl;
        }
    } else {
        boost::json::error_code ec;
        boost::json::value json_val = boost::json::parse(recent_file, ec);

        boost::json::object obj = json_val.as_object();
        
        if (obj.contains("files")) {
            boost::json::array files = obj["files"].as_array();

            for (const auto& file : files) {
                if (file.as_string() == path) {
                    recent_file.close();
                    return true;
                }
            }

            files.push_back(boost::json::value(path));
            obj["files"] = files;
        }

        std::ofstream json_file("recently_used.json");
        if (json_file.is_open()) {
            json_file << boost::json::serialize(obj);
            json_file.close();
        } else {
            std::cerr << "Unable to open file for writing" << std::endl;
        }
    }

    return true;
}

void Project::LoadLast() {
    std::ifstream recent_file("recently_used.json");
    if (recent_file.is_open()) {
        boost::json::error_code ec;
        boost::json::value json_val = boost::json::parse(recent_file, ec);

        boost::json::object obj = json_val.as_object();
        
        if (obj.contains("files")) {
            boost::json::array files = obj["files"].as_array();
            Load(files[files.size() - 1].as_string().c_str());
        }
    }
}

std::map<std::string, void*> handles;
bool Project::IncludeFile(std::string path) {
    if (handles.find(path) != handles.end()) {
        dlclose(handles[path]);
        std::cout << "Already loaded: " << path << std::endl;
    }

    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << std::endl;
        return false;
    } else {
        std::cout << "Opened library: " << path << std::endl;
    }

    dlerror();

    using create_t = IComponent* (*)();
    using destroy_t = void (*)(void*);

    create_t create = (create_t) dlsym(handle, "Create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol create: " << dlsym_error << std::endl;
        dlclose(handle);
        return false;
    }

    std::cout << create << std::endl;

    // Загружаем функцию destroy
    destroy_t destroy = (destroy_t) dlsym(handle, "Destroy");
    dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol destroy: " << dlsym_error << std::endl;
        dlclose(handle);
        return false;
    }

    if (pScene == nullptr) {
        std::cerr << "No scene!" << std::endl;
        dlclose(handle);
        return false;
    }

    if (pScene->pComponentManager == nullptr) {
        std::cerr << "No component manager!" << std::endl;
        dlclose(handle);
        return false;
    }

    IComponent* pComponent = create();
    char* demangledName = abi::__cxa_demangle(typeid(*pComponent).name(), nullptr, nullptr, nullptr);
    pScene->pComponentManager->RegisterComponent(std::string(demangledName), [create] {
        return create();
    }); 

    handles[path] = handle;

    return true;
}

void Project::IncludeFiles() {
    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        std::cout << "No path! : " << path << std::endl;
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
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

std::string Project::GetPath() {
    return path;
}
