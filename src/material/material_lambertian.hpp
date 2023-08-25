//
// Created by syq on 23-8-23.
//

#pragma once

#include "material/material.hpp"
#include "common/utils.hpp"

class Lambertian: public Material { //理想散射材质
private:
    glm::vec3 albedo;

public:
    explicit Lambertian(glm::vec3 albedo): albedo(albedo) {}
    ~Lambertian() override = default;

    bool Scatter(const Ray &in, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const override;
};

bool Lambertian::Scatter(const Ray &in, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const {
    scattered = Ray(hit.position, hit.normal + utils::RandomForUnitSphere());
    attenuation = albedo;
    return true;
}
