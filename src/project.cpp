#include <boost/concept_check.hpp>
#include <icomponent.hpp>
#include <transform.hpp>
#include <project.hpp>

#include <fstream>
#include <filesystem>
#include <boost/json/src.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <cxxabi.h>

using namespace Engine;

boost::json::object ObjectToJSON(Renderer::Object* obj) {
    boost::json::object json_obj;
    json_obj["name"] = boost::json::value(obj->name);
    
    Transform* transform = obj->GetComponent<Transform>();
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

void Project::Save() {
    boost::json::object json_obj;

    boost::json::array jobjs;

    for (Renderer::Object* obj : pScene->GetObjects()) {
        jobjs.push_back(boost::json::object(ObjectToJSON(obj)));
    }
    json_obj["objects"] = jobjs;

    std::string json_str = boost::json::serialize(json_obj);

    std::ofstream json_file(path + "/project.json");
    if (json_file.is_open()) {
        json_file << json_str;  // Write the serialized JSON to the file
        json_file.close();
        std::cout << "JSON data has been saved to project.json" << std::endl;
    } else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }
}

bool Project::Load(std::string path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << "No path! : " << path << std::endl;
        return false;
    }

    std::string project_file = path;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().filename() == "project.json") {
            project_file = entry.path().string();
            break;
        }
    }

    if (!std::filesystem::exists(project_file)) {
        std::cerr << "No project file! : " << project_file << std::endl;
        return false;
    }

    std::ifstream json_file(project_file);
    if (!json_file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return false;
    }

    this->path = path;

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

    return true;
}

void Project::LoadLast() {
    

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
