//
// Created by syq on 23-9-4.
//

#pragma once
#include "pdfs/pdf.hpp"

class PDFSphere: public PDF {
public:
    PDFSphere() = default;
    float Value(glm::vec3 direction) const override;
    glm::vec3 Sample() const override;
};

float PDFSphere::Value(glm::vec3 direction) const {
    return 1.0f / (4.0f * PI);
}

glm::vec3 PDFSphere::Sample() const {
    return utils::RandomUnitVector();
}
