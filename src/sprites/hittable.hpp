//
// Created by syq on 23-8-23.
//

#pragma once
#include "core/ray.hpp"
#include "sprites/aabb_box.hpp"

struct Material;

struct HitRecord {
    float t;
    glm::vec3 position;
    glm::vec3 normal;
    shared_ptr<Material> material;
};

class Hittable {
public:
    shared_ptr<AABBBox> box;
    Hittable() = default;
    virtual ~Hittable() = default;
    virtual bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const = 0;
    virtual void GetAABBBox() = 0;
};
