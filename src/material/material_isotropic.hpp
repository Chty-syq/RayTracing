//
// Created by syq on 23-9-4.
//

#pragma once
#include "material/material.hpp"
#include "pdfs/pdf_sphere.hpp"

class Isotropic: public Material {
public:
private:
    glm::vec3 albedo;

public:
    explicit Isotropic(glm::vec3 albedo): albedo(albedo) {}
    ~Isotropic() override = default;

    bool Scatter(const Ray &in, const HitRecord &hit, ScatterRecord &scatter) const override;
    float ScatterPDF(const Ray &in, const HitRecord &hit, const Ray &scattered) const override;
};

bool Isotropic::Scatter(const Ray &in, const HitRecord &hit, ScatterRecord &scatter) const {
    scatter = {
            .ray = Ray(hit.position, MagicRandom::UnitVector()),
            .attenuation = albedo,
            .is_sample = true,
            .pdf = std::make_shared<PDFSphere>()
    };
    return true;
}

float Isotropic::ScatterPDF(const Ray &in, const HitRecord &hit, const Ray &scattered) const {
    return 1.0f / (4.0f * PI);
}
