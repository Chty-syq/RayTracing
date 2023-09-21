//
// Created by syq on 23-8-23.
//

#pragma once

#include "hittable/hittable.hpp"
#include "common/equation.hpp"

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
    auto oc = ray.origin - center;
    float A = glm::dot(ray.direction, ray.direction);
    float B = glm::dot(ray.direction, oc) * 2.0f;
    float C = glm::dot(oc, oc) - radius * radius;

    vector<double> roots;
    equation::SolveQuadraticReal({ A, B, C }, roots);
    std::sort(roots.begin(), roots.end());
    for(const auto &t : roots) if (t >= t_min && t <= t_max) {
        auto point = ray.PointAt(float(t));
        hit = {
                .t = (float)t,
                .position = point,
                .normal = glm::normalize(point - center),
                .tex_coord = this->GetSphereUV(point),
                .material = material
        };
        return true;
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
