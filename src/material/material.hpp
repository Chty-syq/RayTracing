//
// Created by syq on 23-8-23.
//

#pragma once
#include "framework/ray_tracing/hittable.hpp"

class Material {
public:
    Material() = default;
    virtual ~Material() = default;

    virtual bool Scatter(const Ray &in, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const = 0;
};
