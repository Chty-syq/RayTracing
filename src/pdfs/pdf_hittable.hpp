//
// Created by syq on 23-8-31.
//

#pragma once
#include "sprites/hittable.hpp"
#include "pdfs/pdf.hpp"

class PDFHittable: public PDF {
private:
    shared_ptr<Hittable> hittable;
    glm::vec3 origin;
public:
    PDFHittable(const shared_ptr<Hittable> &hittable, glm::vec3 origin): hittable(hittable), origin(origin) {}
    float Value(glm::vec3 direction) const override {
        return hittable->PDFValue(origin, direction);
    };
    glm::vec3 Sample() const override {
        return hittable->Random(origin);
    };
};
