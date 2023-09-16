//
// Created by syq on 23-8-30.
//

#pragma once

#include "hittable/sphere.hpp"
#include "pdfs/pdf.hpp"

class LightSphere: public Sphere {
public:
    using Sphere::Sphere;

    float PDFValue(glm::vec3 origin, glm::vec3 v) const override;
    glm::vec3 Random(glm::vec3 origin) const override;
};

float LightSphere::PDFValue(glm::vec3 origin, glm::vec3 v) const {
    HitRecord hit;
    if (this->Hit(Ray(origin, v), T_MIN, T_MAX, hit)) {
        auto oc = this->center - origin;
        float cos_theta_max = sqrt(1.0f - pow(radius, 2.0f) / glm::dot(oc, oc));
        float omega = 2.0f * PI * (1 - cos_theta_max);
        return 1.0f / omega;
    }
    return 0.0f;
}

glm::vec3 LightSphere::Random(glm::vec3 origin) const {
    auto oc = this->center - origin;
    float r1 = MagicRandom::Float(0, 1);
    float phi = 2.0f * PI * r1;
    float r2 = MagicRandom::Float(0, 1);

    float cos_theta_max = sqrt(1.0f - pow(radius, 2.0f) / glm::dot(oc, oc));
    float cos_theta = 1.0f + r2 * (cos_theta_max - 1.0f);
    float sin_theta = sqrt(1 - pow(cos_theta, 2.0f));

    float x = cos(phi) * sin_theta;
    float y = sin(phi) * sin_theta;
    float z = cos_theta;

    OrthoBases bases(oc);
    return bases.GetWorld(glm::vec3(x, y, z));
}
