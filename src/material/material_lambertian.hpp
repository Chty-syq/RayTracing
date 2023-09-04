//
// Created by syq on 23-8-23.
//

#pragma once

#include "material/material.hpp"
#include "common/utils.hpp"
#include "pdfs/pdf_cosine.hpp"
#include "texture/texture_color.hpp"

class Lambertian: public Material { //理想散射材质
private:
    shared_ptr<Texture> albedo;
    //glm::vec3 albedo;

public:
    explicit Lambertian(glm::vec3 albedo): albedo(std::make_shared<TextureColor>(albedo)) {}
    explicit Lambertian(const shared_ptr<Texture>& albedo): albedo(albedo) {}
    ~Lambertian() override = default;

    bool Scatter(const Ray &in, const HitRecord &hit, ScatterRecord &scatter) const override;
    float ScatterPDF(const Ray &in, const HitRecord &hit, const Ray &scattered) const override;
};

bool Lambertian::Scatter(const Ray &in, const HitRecord &hit, ScatterRecord &scatter) const {
    scatter = {
            .ray = Ray(hit.position, hit.normal + MagicRandom::UnitVector()),
            .attenuation = albedo->Sample(hit.tex_coord),
            .is_sample = true,
            .pdf = std::make_shared<PDFCosine>(hit.normal)
    };
    return true;
}

float Lambertian::ScatterPDF(const Ray &in, const HitRecord &hit, const Ray &scattered) const {
    float cosine = glm::dot(glm::normalize(scattered.direction), hit.normal);
    return std::max(cosine / PI, 0.0f);
}
