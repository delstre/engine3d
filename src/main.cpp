#include <window.hpp>

int main(int argc, char *argv[]) {
    Engine::Window window(1280, 720, "Engine Window");
    window.Init(argc, argv);
    window.Render();

    return 0;
}
