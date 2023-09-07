//
// Created by syq on 23-8-29.
//

#pragma once

namespace Scene {
    shared_ptr<Camera> camera;
    shared_ptr<HittableList> objects;
    shared_ptr<HittableList> lights;
    glm::vec3 background;

    shared_ptr<Material> ExtractMaterial(const json& content);
    Transformation ExtractTransform(const json& content);

    void LoadConfig(const std::string &path);
    void LoadRandomSpheres();
};

Transformation Scene::ExtractTransform(const json& content) {
    auto position = content.contains("position") ? utils::Json2Vec3(content["position"]) : Transformation::DEFAULT_POSITION;
    auto rotate = content.contains("rotate") ? utils::Json2Vec3(content["rotate"]) : Transformation::DEFAULT_ROTATE;
    auto size = content.contains("size") ? utils::Json2Vec3(content["size"]) : Transformation::DEFAULT_SIZE;
    return { position, rotate, size };
}

shared_ptr<Material> Scene::ExtractMaterial(const json& content) {
    auto type = std::string(content["type"]);
    if (type == "Lambertian") {
        auto albedo = utils::Json2Vec3(content["albedo"]);
        return std::make_shared<Lambertian>(albedo);
    }
    else if (type == "Isotropic") {
        auto albedo = utils::Json2Vec3(content["albedo"]);
        return std::make_shared<Isotropic>(albedo);
    }
    else if (type == "Dielectric") {
        auto refract_idx = float(content["refract_idx"]);
        return std::make_shared<Dielectric>(refract_idx);
    }
    else if (type == "Metal") {
        auto albedo = utils::Json2Vec3(content["albedo"]);
        auto fuzz = content.contains("fuzz") ? float(content["fuzz"]) : 0.0f;
        return std::make_shared<Metal>(albedo, fuzz);
    }
    return std::make_shared<Material>();
}

void Scene::LoadConfig(const std::string &path) {
    std::ifstream f(path);
    json content = json::parse(f);

    camera = std::make_shared<Camera>(
            utils::Json2Vec3(content["camera"]["position"]),
            utils::Json2Vec3(content["camera"]["target"])
            );
    background = utils::Json2Vec3(content["background"]);

    objects = std::make_shared<HittableList>();
    lights = std::make_shared<HittableList>();
    for(const auto &content_light : content["lights"]) {
        auto type = std::string(content_light["type"]);
        shared_ptr<Hittable> light;
        if (type == "quad") {
            auto color = utils::Json2Vec3(content_light["color"]);
            light = std::make_shared<LightQuad>(std::make_shared<DiffuseLight>(color), ExtractTransform(content_light));
        }
        else if (type == "sphere") {
            auto center = utils::Json2Vec3(content_light["position"]);
            auto radius = float(content_light["radius"]);
            auto color = utils::Json2Vec3(content_light["color"]);
            light = std::make_shared<LightSphere>(center, radius, std::make_shared<DiffuseLight>(color));
        }
        else continue;
        lights->AddHittable(light);
        objects->AddHittable(light);
    }
    for(const auto &content_obj: content["hittable"]) {
        auto type = std::string(content_obj["type"]);
        shared_ptr<Hittable> object;
        if (type == "sphere") {
            auto center = utils::Json2Vec3(content_obj["position"]);
            auto radius = float(content_obj["radius"]);
            auto material = ExtractMaterial(content_obj["material"]);
            object = std::make_shared<Sphere>(center, radius, material);
        }
        else if (type == "mesh") {
            auto mesh_path = fs::path(path).parent_path() / std::string(content_obj["path"]);
            auto material = ExtractMaterial(content_obj["material"]);
            auto transform = ExtractTransform(content_obj);
            object = std::make_shared<Mesh>(mesh_path, material, transform);
        }
        else continue;
        objects->AddHittable(object);
    }
    objects->BuildBVH();
}

void Scene::LoadRandomSpheres() {
    camera = std::make_shared<Camera>(glm::vec3(3.0f, 4.0f, 10.0f), glm::vec3(0.0f));
    background = glm::vec3(0.5f, 0.7f, 1.0f);

    objects = std::make_shared<HittableList>();
    lights = std::make_shared<HittableList>();
    for(int i = -11; i < 11; ++i) {
        for(int j = -11; j < 11; ++j) {
            auto center = glm::vec3((float)i + 0.9f * MagicRandom::Float(0, 1), 0.2f, (float)j + 0.9 * MagicRandom::Float(0, 1));
            if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
                float choose_mat = MagicRandom::Float(0, 1);
                if (choose_mat < 0.4f) {
                    objects->AddHittable(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Lambertian>(glm::vec3(
                            MagicRandom::Float(0, 1) * MagicRandom::Float(0, 1),
                            MagicRandom::Float(0, 1) * MagicRandom::Float(0, 1),
                            MagicRandom::Float(0, 1) * MagicRandom::Float(0, 1)
                    ))));
                }
                else if (choose_mat < 0.6f) {
                    objects->AddHittable(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Metal>(glm::vec3(
                                                                                                              0.5f * (1.0f + MagicRandom::Float(0, 1)),
                                                                                                              0.5f * (1.0f + MagicRandom::Float(0, 1)),
                                                                                                              0.5f * (1.0f + MagicRandom::Float(0, 1))),
                                                                                                      0.5f * MagicRandom::Float(0, 1))
                    ));
                }
                else {
                    objects->AddHittable(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Dielectric>(1.5f)));
                }
            }
        }
    }
    objects->AddHittable(std::make_shared<Sphere>( //地面
            glm::vec3(0.0f, -1000.0f, 0.0f),
            1000.0f,
            std::make_shared<Lambertian>(glm::vec3(0.5f))
    ));
    objects->AddHittable(std::make_shared<Sphere>(
            glm::vec3(-4.0f, 1.0f, 0.0f),
            1.0f,
            std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f))
    ));
    objects->AddHittable(std::make_shared<Sphere>(
            glm::vec3(4.0f, 1.0f, 0.0f),
            1.0f,
            std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f)
    ));
    objects->AddHittable(std::make_shared<Sphere>(
            glm::vec3(0.0f, 1.0f, 0.0f),
            1.0f,
            std::make_shared<Dielectric>(1.5f)
    ));
    objects->BuildBVH();
}
