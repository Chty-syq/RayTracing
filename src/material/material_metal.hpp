//
// Created by syq on 23-8-23.
//

#pragma once

#include "framework/ray_tracing/material.hpp"
#include "common/utils.hpp"

class Metal: public Material { //镜面反射材质
private:
    float fuzz;  //扰动半径
    glm::vec3 albedo;

public:
    Metal(glm::vec3 albedo, float fuzz): albedo(albedo), fuzz(std::min(fuzz, 1.0f)) {}
    virtual ~Metal() = default;

    virtual bool Scatter(const Ray &in, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const;
};

bool Metal::Scatter(const Ray &in, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const {
    scattered = Ray(hit.position, glm::reflect(in.direction, hit.normal) + utils::RandomForUnitSphere() * fuzz);
    attenuation = albedo;
    return (glm::dot(scattered.direction, hit.normal) > 0.0f);
}
