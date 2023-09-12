//
// Created by syq on 23-8-30.
//

#pragma once
#include "pdfs/pdf.hpp"

class LightQuad: public Quad {
public:
    using Quad::Quad;
    ~LightQuad() override = default;

    float PDFValue(glm::vec3 origin, glm::vec3 v) const override;
    glm::vec3 Random(glm::vec3 origin) const override;
};


glm::vec3 LightQuad::Random(glm::vec3 origin) const {
    auto center = transformation.position;
    auto axis_x = this->bases->basis[0];
    auto axis_y = this->bases->basis[1];
    auto point = center + MagicRandom::Float(-1, 1) * axis_x + MagicRandom::Float(-1, 1) * axis_y;
    return point - origin;
}

float LightQuad::PDFValue(glm::vec3 origin, glm::vec3 v) const {
    HitRecord hit;
    Ray ray(origin, v);
    if (this->Hit(ray, T_MIN, T_MAX, hit)) {
        float area = width * height;
        float cosine = fabs(glm::dot(hit.normal, v)) / glm::length(v);
        return glm::dot(v, v) / (area * cosine);
    }
    return 0.0f;
}




