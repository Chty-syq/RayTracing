#include "core/tracer.hpp"

int main() {
    //Scene::LoadConfig(fs::current_path().parent_path() / "assets" / "scenes" / "cornell_box_0.json");
    //Scene::LoadConfig(fs::current_path().parent_path() / "assets" / "scenes" / "cornell_box_1.json");
    Scene::LoadConfig(fs::current_path().parent_path() / "assets" / "scenes" / "cornell_box_2.json");
    //Scene::LoadRandomSpheres();

    Tracer tracer;
    tracer.Render();
    return 0;
}
