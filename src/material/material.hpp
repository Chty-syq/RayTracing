//
// Created by syq on 23-8-23.
//

#pragma once
#include "sprites/hittable.hpp"
#include "pdfs/pdf.hpp"
#include "common/utils.hpp"

struct ScatterRecord {
    Ray ray;                //散射光线
    glm::vec3 attenuation;  //颜色
    bool is_sample;         //是否采样
    shared_ptr<PDF> pdf;    //采样函数
};

class Material {
public:
    Material() = default;
    virtual ~Material() = default;

    virtual bool Scatter(const Ray &in, const HitRecord &hit, ScatterRecord &scatter) const { return false; }
    virtual glm::vec3 Emitted(const Ray &in, const HitRecord &hit) const { return glm::vec3(0.0f); }
    virtual float ScatterPDF(const Ray &in, const HitRecord &hit, const Ray &scattered) const { return 0.0f; }
};
