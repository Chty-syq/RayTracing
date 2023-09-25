//
// Created by syq on 23-9-25.
//

#pragma once
#include "hittable/hittable.hpp"

class Ellipsoid: public Hittable { //椭球
public:
    glm::vec3 radius;
    glm::vec3 center;
    glm::vec3 normal{};
    shared_ptr<Material> material;

    Ellipsoid(glm::vec3 center, glm::vec3 radius, glm::vec3 normal, const shared_ptr<Material>& material): center(center), radius(radius), material(material){
        this->normal = glm::normalize(normal);
        this->axis = std::make_shared<OrthoBases>(normal);
    }
    ~Ellipsoid() override = default;

    bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const override;
    void GetAABBBox() override;

protected:
    shared_ptr<OrthoBases> axis;
    glm::vec2 GetEllipsoidUV(glm::vec3 position) const;
};

bool Ellipsoid::Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const {
    auto oc = ray.origin - center;
    auto oc_dot_v = glm::dot(oc, ray.direction);
    auto oc_dot_a = glm::dot(oc, axis->basis[0]);
    auto oc_dot_b = glm::dot(oc, axis->basis[1]);
    auto oc_dot_n = glm::dot(oc, axis->basis[2]);
    auto v_dot_a = glm::dot(ray.direction, axis->basis[0]);
    auto v_dot_b = glm::dot(ray.direction, axis->basis[1]);
    auto v_dot_n = glm::dot(ray.direction, axis->basis[2]);

    const double A = pow(v_dot_a / radius[0], 2.0f) + pow(v_dot_b / radius[1], 2.0f) + pow(v_dot_n / radius[2], 2.0f);
    const double B = 2.0f * (v_dot_a * oc_dot_a / pow(radius[0], 2.0f) + v_dot_b * oc_dot_b / pow(radius[1], 2.0f) + v_dot_n * oc_dot_n / pow(radius[2], 2.0f));
    const double C = pow(oc_dot_a / radius[0], 2.0f) + pow(oc_dot_b / radius[1], 2.0f) + pow(oc_dot_n / radius[2], 2.0f) - 1.0f;

    vector<double> roots;
    equation::SolveQuadraticReal({ A, B, C }, roots);
    std::sort(roots.begin(), roots.end());
    for(const auto &t : roots) if (t >= t_min && t <= t_max) {
        auto point = ray.PointAt(float(t));
        auto local = this->axis->GetLocal(point - center);
        auto normal_local = glm::vec3(local[0] / pow(radius[0], 2.0f), local[0] / pow(radius[1], 2.0f), local[0] / pow(radius[2], 2.0f));
        auto normal_global = this->axis->GetWorld(normal_local);
        hit = {
                .t = (float)t,
                .position = point,
                .normal = glm::normalize(normal_global),
                .tex_coord = this->GetEllipsoidUV(point),
                .material = material
        };
        return true;
    }
    return false;
}

void Ellipsoid::GetAABBBox() {
    auto a = axis->basis[0];
    auto b = axis->basis[1];
    auto box_left = center, box_right = center;
    for(int i = 0; i <= 1; ++i) {
        for(int j = 0; j <= 1; ++j) {
            for(int k = 0; k <= 1; ++k) {
                auto sign_i = (float)i * 2.0f - 1.0f;
                auto sign_j = (float)j * 2.0f - 1.0f;
                auto sign_k = (float)k * 2.0f - 1.0f;
                auto point = center + (radius[0] + radius[1]) * (sign_i * axis->basis[0] + sign_j * axis->basis[1]) + sign_k * radius[2] * normal;
                box_left = utils::EleWiseMin(box_left, point);
                box_right = utils::EleWiseMax(box_right, point);
            }
        }
    }
    this->box = std::make_shared<AABBBox>(box_left, box_right);
}

glm::vec2 Ellipsoid::GetEllipsoidUV(glm::vec3 position) const {
    return glm::vec2();
}
