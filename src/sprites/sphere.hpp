//
// Created by syq on 23-8-23.
//

#pragma once

#include "framework/ray_tracing/hittable.hpp"

class Sphere: public Hittable {
public:
    float radius;
    glm::vec3 center;
    shared_ptr<Material> material;

    Sphere(glm::vec3 center, float radius, shared_ptr<Material> material): center(center), radius(radius), material(material) {}
    ~Sphere() {}

    virtual bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &result) const;
};

bool Sphere::Hit(const Ray &ray, float t_min, float t_max, HitRecord &result) const {
    glm::vec3 oc = ray.origin - center;
    float a = glm::dot(ray.direction, ray.direction);
    float b = glm::dot(ray.direction, oc) * 2.0f;
    float c = glm::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4.0f * a * c;
    if (discriminant > 0) {
        float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + sqrt(discriminant)) / (2.0f * a);
        if (t1 >= t_min && t1 <= t_max) {
            result = {
                    .t = t1,
                    .position = ray.PointAt(t1),
                    .normal = (ray.PointAt(t1) - center) / radius,
                    .material = material
            };
            return true;
        }
        if (t2 >= t_min && t2 <= t_max) {
            result = {
                    .t = t2,
                    .position = ray.PointAt(t2),
                    .normal = (ray.PointAt(t2) - center) / radius,
                    .material = material
            };
            return true;
        }
    }
    return false;
}
