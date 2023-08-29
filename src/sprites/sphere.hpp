//
// Created by syq on 23-8-23.
//

#pragma once

#include "sprites/hittable.hpp"

class Sphere: public Hittable {
public:
    float radius;
    glm::vec3 center;
    shared_ptr<Material> material;

    Sphere(glm::vec3 center, float radius, const shared_ptr<Material>& material): center(center), radius(radius), material(material) {}
    ~Sphere() override = default;

    bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const override;
    void GetAABBBox() override;

private:
    glm::vec2 GetSphereUV(glm::vec3 position) const; //球上一点的纹理坐标
};

bool Sphere::Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const {
    glm::vec3 oc = ray.origin - center;
    float a = glm::dot(ray.direction, ray.direction);
    float b = glm::dot(ray.direction, oc) * 2.0f;
    float c = glm::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4.0f * a * c;
    if (discriminant > 0) {
        float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + sqrt(discriminant)) / (2.0f * a);
        if (t1 >= t_min && t1 <= t_max) {
            hit = {
                    .t = t1,
                    .position = ray.PointAt(t1),
                    .normal = (ray.PointAt(t1) - center) / radius,
                    .tex_coord = this->GetSphereUV(ray.PointAt(t1)),
                    .material = material
            };
            return true;
        }
        if (t2 >= t_min && t2 <= t_max) {
            hit = {
                    .t = t2,
                    .position = ray.PointAt(t2),
                    .normal = (ray.PointAt(t2) - center) / radius,
                    .tex_coord = this->GetSphereUV(ray.PointAt(t2)),
                    .material = material
            };
            return true;
        }
    }
    return false;
}

void Sphere::GetAABBBox() {
    this->box = std::make_shared<AABBBox>(center - glm::vec3(radius), center + glm::vec3(radius));
}

glm::vec2 Sphere::GetSphereUV(glm::vec3 position) const {
    auto direction = glm::normalize(position - center);
    float phi = atan2(direction.z, direction.x);
    float theta = asin(direction.y);
    return {
        0.5f - (phi / (2 * PI)),
        0.5f + (theta / PI)
    };
}
