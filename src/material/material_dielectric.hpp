//
// Created by syq on 23-8-24.
//

#pragma once

#include "framework/ray_tracing/material.hpp"

class Dielectric: public Material { //透明折射材质
private:
    float refract_idx;  //折射率

public:
    Dielectric(float refract_idx): refract_idx(refract_idx) {}
    virtual ~Dielectric() = default;

    float schlick(float cosine) const;
    virtual bool Scatter(const Ray &in, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const;
};

float Dielectric::schlick(float cosine) const { //菲涅尔现象
    float f0 = pow((1.0f - refract_idx) / (1.0f + refract_idx), 2.0f);
    return f0 + (1.0f - f0) * pow(1.0f - cosine, 5.0f);
}

bool Dielectric::Scatter(const Ray &in, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const {
    auto ray_reflected = glm::reflect(in.direction, hit.normal); //反射光
    auto ray_refracted = glm::dot(in.direction, hit.normal) > 0.0f ? //光线由物体内部射向外部
            glm::refract(in.direction, -hit.normal, refract_idx) :
            glm::refract(in.direction, hit.normal, 1.0f / refract_idx);

    if (glm::length(ray_refracted) > 0.0f) {
        float cosine = abs(glm::dot(in.direction, hit.normal));
        float probs = schlick(cosine);
        scattered = utils::RandomFloat(0, 1) < probs ?
                Ray(hit.position, ray_reflected) :
                Ray(hit.position, ray_refracted);
    }
    else { //全反射
        scattered = Ray(hit.position, ray_reflected);
    }
    attenuation = glm::vec3(1.0f);
    return true;
}




