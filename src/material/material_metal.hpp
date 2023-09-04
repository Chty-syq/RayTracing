//
// Created by syq on 23-8-23.
//

#pragma once

#include "material/material.hpp"
#include "common/utils.hpp"

class Metal: public Material { //镜面反射材质
private:
    float fuzz;  //扰动半径
    glm::vec3 albedo;

public:
    Metal(glm::vec3 albedo, float fuzz): albedo(albedo), fuzz(std::min(fuzz, 1.0f)) {}
    ~Metal() override = default;

    bool Scatter(const Ray &in, const HitRecord &hit, ScatterRecord &scatter) const override;
};

bool Metal::Scatter(const Ray &in, const HitRecord &hit, ScatterRecord &scatter) const {
    scatter = {
            .ray = Ray(hit.position, glm::reflect(in.direction, hit.normal) + MagicRandom::UnitVector() * fuzz),
            .attenuation = albedo,
            .is_sample = false,
            .pdf = nullptr
    };
    return true;
    //return (glm::dot(scattered.direction, hit.normal) > 0.0f);
}
