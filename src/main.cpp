#include <tbb/parallel_for.h>
#include "hittable/hittable_list.hpp"
#include "hittable/sphere.hpp"
#include "hittable/mesh.hpp"
#include "hittable/quad.hpp"
#include "hittable/circle.hpp"
#include "hittable/cylinder.hpp"
#include "hittable/cone.hpp"
#include "material/material_lambertian.hpp"
#include "material/material_metal.hpp"
#include "material/material_dielectric.hpp"
#include "material/material_isotropic.hpp"
#include "material/diffuse_light.hpp"
#include "texture/texture_image.hpp"
#include "texture/texture_color.hpp"
#include "pdfs/pdf_cosine.hpp"
#include "pdfs/pdf_hittable.hpp"
#include "pdfs/pdf_mixture.hpp"
#include "pdfs/pdf_sphere.hpp"
#include "lights/light_quad.hpp"
#include "lights/light_sphere.hpp"
#include "lights/light_circle.hpp"
#include "core/ortho_bases.hpp"
#include "core/camera.hpp"
#include "core/scenes.hpp"
#include "core/tracer.hpp"
#include "common/utils.hpp"
#include "common/random.hpp"

int main() {
    //Scene::LoadConfig(fs::current_path().parent_path() / "assets" / "scenes" / "cornell_box_0.json");
    //Scene::LoadConfig(fs::current_path().parent_path() / "assets" / "scenes" / "cornell_box_1.json");
    Scene::LoadConfig(fs::current_path().parent_path() / "assets" / "scenes" / "cornell_box_2.json");
    //Scene::LoadRandomSpheres();

    Tracer tracer;
    tracer.Render();
    return 0;
}
