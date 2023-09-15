//
// Created by syq on 23-9-15.
//

#pragma once
#include "hittable/circle.hpp"

class LightCircle: public Circle {
public:
    using Circle::Circle;
    ~LightCircle() override = default;

    float PDFValue(glm::vec3 origin, glm::vec3 v) const override;
    glm::vec3 Random(glm::vec3 origin) const override;
};

float LightCircle::PDFValue(glm::vec3 origin, glm::vec3 v) const {
    HitRecord hit;
    Ray ray(origin, v);
    if (this->Hit(ray, T_MIN, T_MAX, hit)) {
        float area = PI * pow(radius, 2.0f);
        float cosine = fabs(glm::dot(hit.normal, v)) / glm::length(v);
        return glm::dot(v, v) / (area * cosine);
    }
    return 0.0f;
}

glm::vec3 LightCircle::Random(glm::vec3 origin) const {
    float theta = MagicRandom::Float(0, 2.0f * PI);
    float r = sqrt(MagicRandom::Float(0, pow(radius, 2.0f)));
    auto point = center + r * (axis->basis[0] * sin(theta) + axis->basis[1] * cos(theta));
    return point - origin;
}

