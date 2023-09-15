//
// Created by syq on 23-9-13.
//

#pragma once
#include "hittable/hittable.hpp"

class Circle: public Hittable {
public:
    float radius;
    glm::vec3 center;
    glm::vec3 normal{};
    shared_ptr<Material> material;

    Circle(glm::vec3 center, float radius, glm::vec3 normal, const shared_ptr<Material>& material): center(center), radius(radius), material(material){
        this->normal = glm::normalize(normal);
        this->axis = std::make_shared<OrthoBases>(normal);
    }
    ~Circle() override = default;

    bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const override;
    void GetAABBBox() override;

protected:
    shared_ptr<OrthoBases> axis;
    glm::vec2 GetCircleUV(glm::vec3 position) const; //圆上一点的纹理坐标
};

bool Circle::Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const {
    if (glm::dot(ray.direction, normal) == 0.0f)  return false;
    float t = (glm::dot(center, normal) - glm::dot(ray.origin, normal)) / glm::dot(ray.direction, normal);
    if (t < t_min || t > t_max)  return false;
    auto p = ray.PointAt(t);
    if (glm::length(p - center) > radius) return false;
    hit = {
            .t = t,
            .position = p,
            .normal = normal,
            .tex_coord = this->GetCircleUV(p),
            .material = material
    };
    return true;
}

void Circle::GetAABBBox() {
    auto a = this->axis->basis[0];
    auto b = this->axis->basis[1];
    auto factor = sqrt(a * a + b * b);
    this->box = std::make_shared<AABBBox>(center - radius * factor, center + radius * factor);
}

glm::vec2 Circle::GetCircleUV(glm::vec3 position) const {
    auto local = (this->axis->GetLocal(position - center) / radius + 1.0f) * 0.5f;
    return { local.x, local.y };
}
