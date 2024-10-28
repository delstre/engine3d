#include <window.hpp>

int main() {
    Engine::Window window(1280, 720, "Engine Window");
    window.Init();
    window.Render();

    return 0;
}
