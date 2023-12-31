//
// Created by syq on 23-8-29.
//

#pragma once

namespace Scene {
    shared_ptr<Camera> camera;
    shared_ptr<HittableList> objects;
    shared_ptr<HittableList> lights;
    glm::vec3 background;

    shared_ptr<Material> ExtractMaterial(const json &material);
    shared_ptr<Texture> ExtractTexture(const json &albedo);
    Transformation ExtractTransform(const json &transform);
    shared_ptr<Hittable> ExtractLight(const json &item);
    shared_ptr<Hittable> ExtractObject(const json &item);

    void LoadConfig(const std::string &path);
    void LoadRandomSpheres();
};

void Scene::LoadConfig(const std::string &path) {
    std::ifstream f(path);
    json config = json::parse(f);

    camera = std::make_shared<Camera>(
            converter::Json2Vec3(config["camera"]["position"]),
            converter::Json2Vec3(config["camera"]["target"])
    );
    background = converter::Json2Vec3(config["background"]);

    objects = std::make_shared<HittableList>();
    lights = std::make_shared<HittableList>();
    for(const auto &item : config["lights"]) {
        auto light = ExtractLight(item);
        if (light != nullptr) {
            lights->AddHittable(light);
            objects->AddHittable(light);
        }
    }
    for(const auto &item: config["hittable"]) {
        auto object = ExtractObject(item);
        if (object != nullptr) {
            objects->AddHittable(object);
        }
    }
    objects->BuildBVH();
}

Transformation Scene::ExtractTransform(const json &transform) {
    auto position = transform.contains("position") ? converter::Json2Vec3(transform["position"]) : Transformation::DEFAULT_POSITION;
    auto rotate = transform.contains("rotate") ? converter::Json2Quat(transform["rotate"]) : Transformation::DEFAULT_ROTATE;
    auto size = transform.contains("size") ? converter::Json2Vec3(transform["size"]) : Transformation::DEFAULT_SIZE;
    return { position, rotate, size };
}

shared_ptr<Texture> Scene::ExtractTexture(const json &albedo) {
    if (albedo.is_array())
        return std::make_shared<TextureColor>(converter::Json2Vec3(albedo));
    else
        return std::make_shared<TextureImage>(fs::current_path().parent_path() / "assets" / "textures" / std::string(albedo));
}

shared_ptr<Material> Scene::ExtractMaterial(const json &material) {
    auto type = std::string(material["type"]);
    if (type == "Lambertian")           return std::make_shared<Lambertian>(ExtractTexture(material["albedo"]));
    else if (type == "Isotropic")       return std::make_shared<Isotropic>(ExtractTexture(material["albedo"]));
    else if (type == "Dielectric")      return std::make_shared<Dielectric>(float(material["refract_idx"]));
    else if (type == "Metal") {
        auto albedo = ExtractTexture(material["albedo"]);
        auto fuzz = material.contains("fuzz") ? float(material["fuzz"]) : 0.0f;
        return std::make_shared<Metal>(albedo, fuzz);
    }
    else if (type == "DiffuseLight")    return std::make_shared<DiffuseLight>(ExtractTexture(material["albedo"]));
    else                                return std::make_shared<Material>();
}

shared_ptr<Hittable> Scene::ExtractLight(const json &item) {
    auto type = std::string(item["type"]);
    if (type == "quad") {
        auto transform = ExtractTransform(item["transform"]);
        auto material = ExtractMaterial(item["material"]);
        return std::make_shared<LightQuad>(material, transform);
    }
    else if (type == "sphere") {
        auto center = converter::Json2Vec3(item["center"]);
        auto radius = float(item["radius"]);
        auto material = ExtractMaterial(item["material"]);
        return std::make_shared<LightSphere>(center, radius, material);
    }
    else if (type == "circle") {
        auto center = converter::Json2Vec3(item["center"]);
        auto radius = float(item["radius"]);
        auto normal = converter::Json2Vec3(item["normal"]);
        auto material = ExtractMaterial(item["material"]);
        return std::make_shared<LightCircle>(center, radius, normal, material);
    }
    return nullptr;
}

shared_ptr<Hittable> Scene::ExtractObject(const json &item) {
    auto type = std::string(item["type"]);
    if (type == "sphere") {
        auto center = converter::Json2Vec3(item["center"]);
        auto radius = float(item["radius"]);
        auto material = ExtractMaterial(item["material"]);
        return std::make_shared<Sphere>(center, radius, material);
    }
    else if (type == "mesh") {
        auto mesh_path = fs::current_path().parent_path() / "assets" / "mesh_triangle" / std::string(item["path"]);
        auto material = ExtractMaterial(item["material"]);
        auto transform = ExtractTransform(item["transform"]);
        return std::make_shared<Mesh>(mesh_path, material, transform);
    }
    else if (type == "quad") {
        auto material = ExtractMaterial(item["material"]);
        auto transform = ExtractTransform(item["transform"]);
        return std::make_shared<Quad>(material, transform);
    }
    else if (type == "circle") {
        auto center = converter::Json2Vec3(item["center"]);
        auto radius = float(item["radius"]);
        auto normal = converter::Json2Vec3(item["normal"]);
        auto material = ExtractMaterial(item["material"]);
        return std::make_shared<Circle>(center, radius, normal, material);
    }
    else if (type == "cylinder_face") {
        auto center = converter::Json2Vec3(item["center"]);
        auto radius = float(item["radius"]);
        auto height = float(item["height"]);
        auto normal = converter::Json2Vec3(item["normal"]);
        auto material = ExtractMaterial(item["material"]);
        return std::make_shared<CylinderFace>(center, radius, height, normal, material);
    }
    else if (type == "cylinder") {
        auto center = converter::Json2Vec3(item["center"]);
        auto radius = float(item["radius"]);
        auto height = float(item["height"]);
        auto normal = converter::Json2Vec3(item["normal"]);
        auto material = ExtractMaterial(item["material"]);
        return std::make_shared<Cylinder>(center, radius, height, normal, material);
    }
    else if (type == "cone_face") {
        auto center = converter::Json2Vec3(item["center"]);
        auto radius = float(item["radius"]);
        auto height = float(item["height"]);
        auto normal = converter::Json2Vec3(item["normal"]);
        auto material = ExtractMaterial(item["material"]);
        return std::make_shared<ConeFace>(center, radius, height, normal, material);
    }
    else if (type == "cone") {
        auto center = converter::Json2Vec3(item["center"]);
        auto radius = float(item["radius"]);
        auto height = float(item["height"]);
        auto normal = converter::Json2Vec3(item["normal"]);
        auto material = ExtractMaterial(item["material"]);
        return std::make_shared<Cone>(center, radius, height, normal, material);
    }
    else if (type == "torus") {
        auto center = converter::Json2Vec3(item["center"]);
        auto radius = float(item["radius"]);
        auto radius_cube = float(item["radius_cube"]);
        auto normal = converter::Json2Vec3(item["normal"]);
        auto material = ExtractMaterial(item["material"]);
        return std::make_shared<Torus>(center, radius, radius_cube, normal, material);
    }
    else if (type == "ellipsoid") {
        auto center = converter::Json2Vec3(item["center"]);
        auto radius = converter::Json2Vec3(item["radius"]);
        auto normal = converter::Json2Vec3(item["normal"]);
        auto material = ExtractMaterial(item["material"]);
        return std::make_shared<Ellipsoid>(center, radius, normal, material);
    }
    return nullptr;
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
