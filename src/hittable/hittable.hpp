//
// Created by syq on 23-8-23.
//

#pragma once
#include "core/ray.hpp"
#include "hittable/aabb_box.hpp"

struct Material;

struct HitRecord {
    float t;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;
    shared_ptr<Material> material;
};

class Hittable {
public:
    shared_ptr<AABBBox> box;
    Hittable() = default;
    virtual ~Hittable() = default;
    virtual bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const = 0;
    virtual void GetAABBBox() = 0;

    virtual float PDFValue(glm::vec3 origin, glm::vec3 v) const { return 0.0f; }  //计算光线的pdf值
    virtual glm::vec3 Random(glm::vec3 origin) const { return glm::vec3(0.0f); }  //对于给定一点，采样光线方向
};
