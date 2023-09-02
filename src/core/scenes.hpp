//
// Created by syq on 23-8-29.
//

#pragma once

namespace Scenes {

    shared_ptr<LightQuad> light_quad;
    shared_ptr<LightSphere> light_sphere;

    auto UseScene0(Camera &camera) {
        auto hittable_list = std::make_shared<HittableList>();
        for(int i = -11; i < 11; ++i) {
            for(int j = -11; j < 11; ++j) {
                auto center = glm::vec3((float)i + 0.9f * utils::RandomFloat(0, 1), 0.2f, (float)j + 0.9 * utils::RandomFloat(0, 1));
                if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
                    float choose_mat = utils::RandomFloat(0, 1);
                    if (choose_mat < 0.4f) {
                        hittable_list->AddHittable(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Lambertian>(glm::vec3(
                                utils::RandomFloat(0, 1) * utils::RandomFloat(0, 1),
                                utils::RandomFloat(0, 1) * utils::RandomFloat(0, 1),
                                utils::RandomFloat(0, 1) * utils::RandomFloat(0, 1)
                        ))));
                    }
                    else if (choose_mat < 0.6f) {
                        hittable_list->AddHittable(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Metal>(glm::vec3(
                                                                                                                          0.5f * (1.0f + utils::RandomFloat(0, 1)),
                                                                                                                          0.5f * (1.0f + utils::RandomFloat(0, 1)),
                                                                                                                          0.5f * (1.0f + utils::RandomFloat(0, 1))),
                                                                                                                  0.5f * utils::RandomFloat(0, 1))
                        ));
                    }
                    else {
                        hittable_list->AddHittable(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Dielectric>(1.5f)));
                    }
                }
            }
        }
        hittable_list->AddHittable(std::make_shared<Sphere>( //地面
                glm::vec3(0.0f, -1000.0f, 0.0f),
                1000.0f,
                std::make_shared<Lambertian>(glm::vec3(0.5f))
        ));
        hittable_list->AddHittable(std::make_shared<Sphere>(
                glm::vec3(-4.0f, 1.0f, 0.0f),
                1.0f,
                std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f))
        ));
        hittable_list->AddHittable(std::make_shared<Sphere>(
                glm::vec3(4.0f, 1.0f, 0.0f),
                1.0f,
                std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f)
        ));
        hittable_list->AddHittable(std::make_shared<Sphere>(
                glm::vec3(0.0f, 1.0f, 0.0f),
                1.0f,
                std::make_shared<Dielectric>(1.5f)
        ));
        hittable_list->AddHittable(std::make_shared<Mesh>(
                fs::current_path().parent_path() / "assets" / "objects" / "cbox_smallbox.obj",
                std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f)),
                Transformation()
        ));
        camera = Camera(glm::vec3(3.0f, 4.0f, 10.0f), glm::vec3(0.0f));
        return hittable_list;
    }

    auto UseScene1(Camera &camera) {
        auto hittable_list = std::make_shared<HittableList>();
        hittable_list->AddHittable(std::make_shared<Mesh>(
                fs::current_path().parent_path() / "assets" / "objects" / "cbox_luminaire.obj",
                std::make_shared<Lambertian>(glm::vec3(0.73f)),
                Transformation::FromSize(glm::vec3(0.01f))
        ));
        hittable_list->AddHittable(std::make_shared<Mesh>(
                fs::current_path().parent_path() / "assets" / "objects" / "cbox_floor.obj",
                std::make_shared<Lambertian>(glm::vec3(0.73f)),
                Transformation::FromSize(glm::vec3(0.01f))
        ));
        hittable_list->AddHittable(std::make_shared<Mesh>(
                fs::current_path().parent_path() / "assets" / "objects" / "cbox_ceiling.obj",
                std::make_shared<Lambertian>(glm::vec3(0.73f)),
                Transformation::FromSize(glm::vec3(0.01f))
        ));
        hittable_list->AddHittable(std::make_shared<Mesh>(
                fs::current_path().parent_path() / "assets" / "objects" / "cbox_back.obj",
                std::make_shared<Lambertian>(glm::vec3(0.73f)),
                Transformation::FromSize(glm::vec3(0.01f))
        ));
        hittable_list->AddHittable(std::make_shared<Mesh>(
                fs::current_path().parent_path() / "assets" / "objects" / "cbox_greenwall.obj",
                std::make_shared<Lambertian>(glm::vec3(0.12f, 0.45f, 0.15f)),
                Transformation::FromSize(glm::vec3(0.01f))
        ));
        hittable_list->AddHittable(std::make_shared<Mesh>(
                fs::current_path().parent_path() / "assets" / "objects" / "cbox_redwall.obj",
                std::make_shared<Lambertian>(glm::vec3(0.65f, 0.05f, 0.05f)),
                Transformation::FromSize(glm::vec3(0.01f))
        ));
        hittable_list->AddHittable(std::make_shared<Mesh>(
                fs::current_path().parent_path() / "assets" / "objects" / "cbox_smallbox.obj",
                std::make_shared<Lambertian>(glm::vec3(0.73f)),
                Transformation::FromSize(glm::vec3(0.01f))
        ));
        hittable_list->AddHittable(std::make_shared<Mesh>(
                fs::current_path().parent_path() / "assets" / "objects" / "cbox_largebox.obj",
                std::make_shared<Lambertian>(glm::vec3(0.73f)),
                Transformation::FromSize(glm::vec3(0.01f))
        ));
        hittable_list->AddHittable(std::make_shared<Sphere>( //玻璃球体
                glm::vec3(1.855f, 2.3f, 1.69f),
                0.65f,
                std::make_shared<Dielectric>(1.5f)
        ));
//        hittable_list->AddHittable(std::make_shared<Sphere>( //发光球体
//                glm::vec3(1.855f, 2.3f, 1.69f),
//                0.65f,
//                std::make_shared<DiffuseLight>(std::make_shared<TextureColor>(glm::vec3(1.0f, 0.85f, 0.73f)))
//        ));
        light_sphere = std::make_shared<LightSphere>(
                glm::vec3(1.855f, 2.3f, 1.69f),
                0.65f,
                std::make_shared<DiffuseLight>(std::make_shared<TextureColor>(glm::vec3(1.0f, 0.85f, 0.73f)))
                );
        //hittable_list->AddHittable(light_sphere);
        light_quad = std::make_shared<LightQuad>(
                std::make_shared<DiffuseLight>(std::make_shared<TextureColor>(glm::vec3(1.0f))),
                Transformation(glm::vec3(2.93f, 5.478f, 3.195f), Transformation::DEFAULT_ROTATE, glm::vec3(1.0f))
        );
        hittable_list->AddHittable(light_quad);
        camera = Camera(glm::vec3(2.78f, 2.73f, -8.0f), glm::vec3(2.78f, 2.73f, -7.99f));
        return hittable_list;
    }
}
