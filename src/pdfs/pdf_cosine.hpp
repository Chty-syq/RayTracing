//
// Created by syq on 23-8-30.
//

#pragma once

#include "pdfs/pdf.hpp"
#include "core/ortho_bases.hpp"
#include "common/random.hpp"

class PDFCosine: public PDF {
private:
    glm::vec3 normal;
    shared_ptr<OrthoBases> axis;

public:
    explicit PDFCosine(glm::vec3 normal);
    float Value(glm::vec3 direction) const override;  //函数值
    glm::vec3 Sample() const override;   //采样
};

PDFCosine::PDFCosine(glm::vec3 normal): normal(normal) {
    this->axis = std::make_shared<OrthoBases>(normal);
}

float PDFCosine::Value(glm::vec3 direction) const {
    direction = glm::normalize(direction);
    float cosine = glm::dot(direction, this->normal);
    return std::max(cosine / PI, 0.0f);
}

glm::vec3 PDFCosine::Sample() const {
    float r1 = MagicRandom::Float(0, 1);
    float phi = 2.0f * PI * r1;
    float r2 = MagicRandom::Float(0, 1);
    float cos_theta = sqrt(1.0f - r2);
    float sin_theta = sqrt(r2);
    float x = cos(phi) * sin_theta;
    float y = sin(phi) * sin_theta;
    float z = cos_theta;
    return axis->GetLocal(glm::vec3(x, y, z));
}
