//
// Created by syq on 23-8-31.
//

#pragma once
#include "pdfs/pdf.hpp"

class PDFMixture: public PDF {
private:
    shared_ptr<PDF> pdfs[2];
    float weight;
public:
    PDFMixture(const shared_ptr<PDF> &p0, const shared_ptr<PDF> &p1, float weight = 0.5f): weight(weight) {
        this->pdfs[0] = p0;
        this->pdfs[1] = p1;
    }
    float Value(glm::vec3 direction) const override;
    glm::vec3 Sample() const override;
};

float PDFMixture::Value(glm::vec3 direction) const {
    return weight * pdfs[0]->Value(direction) + (1 - weight) * pdfs[1]->Value(direction);
}

glm::vec3 PDFMixture::Sample() const {
    return (MagicRandom::Float(0, 1) < weight) ? pdfs[0]->Sample() : pdfs[1]->Sample();
}
