#include <boost/concept_check.hpp>
#include <project.hpp>
#include <cscomponent.hpp>
#include <transform.hpp>

#include "fstream"
#include <boost/json/src.hpp>
#include <boost/property_tree/json_parser.hpp>

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

void Project::Save(std::string path) {
    boost::json::object json_obj;

    boost::json::array jfiles;

    for (std::string file : files) {
        jfiles.push_back(boost::json::value(file));
    }
    json_obj["files"] = jfiles;

    boost::json::array jobjs;

    for (Renderer::Object* obj : pScene->GetObjects()) {
        jobjs.push_back(boost::json::object(ObjectToJSON(obj)));
    }
    json_obj["objects"] = jobjs;

    std::string json_str = boost::json::serialize(json_obj);

    std::ofstream json_file("project.json");
    if (json_file.is_open()) {
        json_file << json_str;  // Write the serialized JSON to the file
        json_file.close();
        std::cout << "JSON data has been saved to project.json" << std::endl;
    } else {
        std::cerr << "Unable to open file for writing" << std::endl;
    }
}

bool Project::Load(std::string path) {
    std::ifstream json_file(path);
    if (!json_file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return false;
    }

    std::string json_str((std::istreambuf_iterator<char>(json_file)),
                         std::istreambuf_iterator<char>());

    boost::json::error_code ec;
    boost::json::value json_val = boost::json::parse(json_str, ec);

    if (ec) {
        std::cerr << "Failed to parse JSON: " << ec.message() << std::endl;
        return false;
    }

    boost::json::object json_obj = json_val.as_object();
    boost::json::array jfiles = json_obj["files"].as_array();

    for (const auto& file : jfiles) {
        if (!IncludeFile(file.as_string().c_str())) {
            return false;
        };
    }

    if (auto* objects_value = json_obj.if_contains("objects")) {
        if (objects_value->is_array()) {
            boost::json::array jobjs = objects_value->as_array();

            for (const auto& jobj : jobjs) {
                if (jobj.is_object()) { // Ensure each item is an object
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

bool Project::IncludeFile(std::string path) {
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << std::endl;
        return false;
    } else {
        std::cout << "Opened library: " << path << std::endl;
    }

    using create_t = void* (*)();
    using destroy_t = void (*)(void*);

    // Загружаем функцию create
    create_t create = (create_t) dlsym(handle, "Create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "Cannot load symbol create: " << dlsym_error << std::endl;
        dlclose(handle);
        return false;
    }

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
        return false;
    }

    if (pScene->pComponentManager == nullptr) {
        std::cerr << "No component manager!" << std::endl;
        return false;
    }

    pScene->pComponentManager->RegisterComponent("CustomComponent", [handle, create](Renderer::Object* obj) {
        CSComponent* comp = new CSComponent;
        comp->SetHandler(handle, create());
        comp->SetParent(obj);
        return comp;
    });

    files.push_back(path);

    return true;
}

Scene* Project::GetScene() {
    return pScene;
}


