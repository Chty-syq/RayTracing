//
// Created by syq on 23-8-23.
//

#pragma once

#include <tbb/parallel_for.h>
#include "sprites/hittable_list.hpp"
#include "sprites/sphere.hpp"
#include "sprites/mesh.hpp"
#include "material/material_lambertian.hpp"
#include "material/material_metal.hpp"
#include "material/material_dielectric.hpp"
#include "texture/texture_image.hpp"
#include "core/camera.hpp"
#include "common/utils.hpp"

class Tracer {
private:
    int depth_limit{};
    int width, height, channel{};
public:
    unsigned char* image;
    Tracer();
    ~Tracer() = default;
    static auto RandomScene();
    glm::vec4 Tracing(const Ray& ray, const shared_ptr<HittableList>& world, int depth);
    void DrawPixel(int row, int col, glm::vec4 color) const;
    void Render();
};

Tracer::Tracer() {
    this->width = SCREEN_WIDTH;
    this->height = SCREEN_HEIGHT;
    this->channel = 4;
    this->depth_limit = 50;
    this->image = new unsigned char[width * height * channel];
}

auto Tracer::RandomScene() {
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
            std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f))
            ));
    return hittable_list;
}

void Tracer::Render() {
    auto world = RandomScene();
    world->BuildBVH();
    auto camera_pos = glm::vec3(3.0f, 4.0f, 10.0f);
    Camera camera(camera_pos, glm::vec3(0.0f));

    auto time_start = std::chrono::high_resolution_clock::now();

    int samples = 100;
    std::atomic<size_t> counter = 0;
    cout << "Ray-tracing is running" << endl;
    tbb::parallel_for(tbb::blocked_range<int>(0, height * width, 692), [&](tbb::blocked_range<int>& r) {
        for(int index = r.begin(); index != r.end(); ++index) {
            int i = index / width;
            int j = index % width;
            auto color = glm::vec4(0.0f);
            for(int sps = 0; sps < samples; ++sps) {
                float u = ((float)j + utils::RandomFloat(0, 1)) / (float)width;
                float v = ((float)i + utils::RandomFloat(0, 1)) / (float)height;
                Ray ray = camera.GetRay(u, v);
                color += Tracing(ray, world, 0);
            }
            color /= (float)samples;
            color.w = 1.0f;
            color = glm::vec4(sqrt(color.x), sqrt(color.y), sqrt(color.z), color.w);  //gamma校正
            DrawPixel(i, j, color);
            utils::ShowProgressRate((float)(++counter) / (float)(height * width));
        }
    });
    cout << endl;

    auto save_dir = fs::current_path().parent_path() / "ray_tracing.png";
    stbi_flip_vertically_on_write(true);
    stbi_write_png(save_dir.c_str(), width, height, channel, image, width * channel);

    auto time_end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(time_end - time_start);
    cout << "Time cost: " << duration.count() << "s" << endl;
}

glm::vec4 Tracer::Tracing(const Ray &ray, const shared_ptr<HittableList>& world, int depth) {
    HitRecord hit;
    if (world->Hit(ray, 0.001f, 100.0f, hit)) {
        glm::vec3 attenuation;
        Ray scattered{};
        if (depth < depth_limit && hit.material->Scatter(ray, hit, attenuation, scattered)) {
            return glm::vec4(attenuation, 1.0f) * Tracing(scattered, world, depth + 1);
        }
        else return {0.0f, 0.0f, 0.0f, 1.0f};
    }
    else {
        float t = 0.5f * (ray.direction.y + 1.0f);
        auto color = glm::vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + glm::vec3(0.5f, 0.7f, 1.0f) * t;
        return {color, 1.0f};
    }
}

void Tracer::DrawPixel(int row, int col, glm::vec4 color) const {
    if (row < 0 || row >= height || col < 0 || col >= width) return;
    int index = (row * width + col) * channel;
    image[index + 0] = (unsigned char )(255.0f * color.x);
    image[index + 1] = (unsigned char )(255.0f * color.y);
    image[index + 2] = (unsigned char )(255.0f * color.z);
    image[index + 3] = (unsigned char )(255.0f * color.w);
}


