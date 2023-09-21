//
// Created by syq on 23-9-18.
//

#pragma once
#include "hittable/hittable.hpp"
#include "common/equation.hpp"
#include <unsupported/Eigen/Polynomials>

class Torus: public Hittable {
public:
    float radius; // the radius of the tube
    float radius_cube; // the radius from the center of the hole to the center of the torus tube
    glm::vec3 center;
    glm::vec3 normal{};
    shared_ptr<Material> material;

    Torus(glm::vec3 center, float radius, float radius_cube, glm::vec3 normal, const shared_ptr<Material>& material): center(center), radius(radius), radius_cube(radius_cube), material(material){
        this->normal = glm::normalize(normal);
        this->axis = std::make_shared<OrthoBases>(normal);
    }
    ~Torus() override = default;

    bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const override;
    void GetAABBBox() override;

protected:
    shared_ptr<OrthoBases> axis;
    glm::vec2 GetTorusUV(glm::vec3 position) const;
};

bool Torus::Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const {
    auto origin = ray.origin;
    auto distance = glm::dot(center - ray.origin, ray.direction);
    auto diff = 0.0f;
    if (distance > radius + radius_cube) {
        origin += (distance - radius - radius_cube) * ray.direction;
        diff = (distance - radius - radius_cube);
    }
    auto oc = origin - center;
    auto oc_dot_v = glm::dot(oc, ray.direction);
    auto oc_dot_a = glm::dot(oc, axis->basis[0]);
    auto oc_dot_b = glm::dot(oc, axis->basis[1]);
    auto v_dot_a = glm::dot(ray.direction, axis->basis[0]);
    auto v_dot_b = glm::dot(ray.direction, axis->basis[1]);
    auto R2 = pow(radius, 2.0f);
    auto factor = pow(radius, 2.0f) - pow(radius_cube, 2.0f) + glm::dot(oc, oc);

    const double A = 1.0f;
    const double B = 4.0f * oc_dot_v;
    const double C = 4.0f * pow(oc_dot_v, 2.0f) + 2.0f * factor - 4 * R2 * (pow(v_dot_a, 2.0f) + pow(v_dot_b, 2.0f));
    const double D = 4.0f * oc_dot_v * factor - 8.0f * R2 * (v_dot_a * oc_dot_a + v_dot_b * oc_dot_b);
    const double E = pow(factor, 2.0f) - 4.0f * R2 * (pow(oc_dot_a, 2.0f) + pow(oc_dot_b, 2.0f));

    vector<double> roots;
    equation::SolveQuarticReal({ A, B, C, D, E }, roots);
    std::sort(roots.begin(), roots.end());
    for(const auto &root : roots) {
        auto t = (float)root + diff;
        if (t >= t_min && t <= t_max) {
            auto point = ray.PointAt(float(t));
            auto local = this->axis->GetLocal(point - center);
            auto direction = glm::normalize(glm::vec3(local.x, local.y, 0.0f));
            auto center_cube = this->axis->GetWorld(direction * radius) + center;
            hit = {
                    .t = (float) t,
                    .position = point,
                    .normal = glm::normalize(point - center_cube),
                    .tex_coord = this->GetTorusUV(point),
                    .material = material
            };
            return true;
        }
    }
    return false;
}

void Torus::GetAABBBox() {
    auto a = axis->basis[0];
    auto b = axis->basis[1];
    auto box_left = center, box_right = center;
    for(int i = 0; i <= 1; ++i) {
        for(int j = 0; j <= 1; ++j) {
            for(int k = 0; k <= 1; ++k) {
                auto sign_i = (float)i * 2.0f - 1.0f;
                auto sign_j = (float)j * 2.0f - 1.0f;
                auto sign_k = (float)k * 2.0f - 1.0f;
                auto point = center + (radius + radius_cube) * (sign_i * axis->basis[0] + sign_j * axis->basis[1]) + sign_k * radius_cube * normal;
                box_left = utils::EleWiseMin(box_left, point);
                box_right = utils::EleWiseMax(box_right, point);
            }
        }
    }
    this->box = std::make_shared<AABBBox>(box_left, box_right);
}

glm::vec2 Torus::GetTorusUV(glm::vec3 position) const {
    auto local = this->axis->GetLocal(position - center);
    auto theta = atan2(local[1], local[0]);
    return {
            theta / (2.0f * PI) + 0.5f,
            local[2] / (2.0f * radius_cube) + 0.5f
    };
}
