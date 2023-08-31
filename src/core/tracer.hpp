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
#include "material/diffuse_light.hpp"
#include "texture/texture_image.hpp"
#include "texture/texture_color.hpp"
#include "pdfs/pdf_cosine.hpp"
#include "pdfs/pdf_hittable.hpp"
#include "pdfs/pdf_mixture.hpp"
#include "lights/light_quad.hpp"
#include "lights/light_sphere.hpp"
#include "core/ortho_bases.hpp"
#include "core/camera.hpp"
#include "core/scenes.hpp"
#include "common/utils.hpp"

class Tracer {
private:
    int depth_limit{};
    int width, height, channel{};
public:
    unsigned char* image;
    Tracer();
    ~Tracer() = default;
    glm::vec3 Tracing(const Ray& ray, const shared_ptr<HittableList>& world, int depth);
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

void Tracer::Render() {
    Camera camera;
    auto world = Scenes::UseScene1(camera);

    world->BuildBVH();
    auto time_start = std::chrono::high_resolution_clock::now();

    int samples = 1000;
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
                color += glm::vec4(Tracing(ray, world, 0), 1.0f);
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

glm::vec3 Tracer::Tracing(const Ray &ray, const shared_ptr<HittableList>& world, int depth) {
    HitRecord hit;
    if (world->Hit(ray, T_MIN, T_MAX, hit)) {
        glm::vec3 attenuation;
        Ray scattered{};
        auto emitted = hit.material->Emitted(ray, hit);
        if (depth < depth_limit && hit.material->Scatter(ray, hit, attenuation, scattered)) {
            if (dynamic_cast<Lambertian*>(hit.material.get()) != nullptr) {
                auto p0 = std::make_shared<PDFCosine>(hit.normal);
                auto p1 = std::make_shared<PDFHittable>(Scenes::light_quad, hit.position);
                auto p2 = std::make_shared<PDFHittable>(Scenes::light_sphere, hit.position);
                PDFMixture pdf(p0, p2, 0.5f);
                //PDFHittable pdf(Scenes::light_sphere, hit.position);
                //PDFCosine pdf(hit.normal);
                auto direction = pdf.Sample();
                auto prob = pdf.Value(direction);
                scattered = Ray(hit.position, direction);
                float scattering_pdf = hit.material->ScatterPDF(ray, hit, scattered);
                //utils::PrintVec3(glm::vec3(scattering_pdf, prob, 0.0f));
                return emitted + attenuation * scattering_pdf * Tracing(scattered, world, depth + 1) / prob;
            }
            return emitted + attenuation * Tracing(scattered, world, depth + 1);
        }
        else return emitted;
    }
    else {
//        float t = 0.5f * (ray.direction.y + 1.0f);
//        auto color = glm::vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + glm::vec3(0.5f, 0.7f, 1.0f) * t;
//        return color;
        return glm::vec3(0.0f);
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


