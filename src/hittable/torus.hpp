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
    auto oc = ray.origin - center;
    auto oc_dot_v = glm::dot(oc, ray.direction);
    auto oc_dot_a = glm::dot(oc, axis->basis[0]);
    auto oc_dot_b = glm::dot(oc, axis->basis[1]);
    auto v_dot_a = glm::dot(ray.direction, axis->basis[0]);
    auto v_dot_b = glm::dot(ray.direction, axis->basis[1]);
    auto oc2 = glm::dot(oc, oc);
    auto R2 = pow(radius, 2.0f);
    auto r2 = pow(radius_cube, 2.0f);
    float A = 1.0f;
    float B = 4.0f * oc_dot_v;
    float C = 4.0f * pow(oc_dot_v, 2.0f) + 2.0f * (R2 - r2 + oc2) - 4 * R2 * (pow(v_dot_a, 2.0f) + pow(v_dot_b, 2.0f));
    float D = 4.0f * oc_dot_v * (R2 - r2 + oc2) - 8.0f * R2 * (v_dot_a * oc_dot_a + v_dot_b * oc_dot_b);
    float E = pow(R2 - r2 + oc2, 2.0f) - 4.0f * R2 * (pow(oc_dot_a, 2.0f) + pow(oc_dot_b, 2.0f));

    vector<double> roots;
    equation::SolveQuarticReal({ A, B, C, D, E }, roots);
    std::sort(roots.begin(), roots.end());
    for(const auto &t : roots) if (t >= t_min && t <= t_max) {
        auto point = ray.PointAt(float(t));
        auto local = this->axis->GetLocal(point - center);
        auto direction = glm::normalize(glm::vec3(local.x, local.y, 0.0f));
        auto center_cube = this->axis->GetWorld(direction * radius) + center;
        hit = {
                .t = (float)t,
                .position = point,
                .normal = glm::normalize(point - center_cube),
                .tex_coord = this->GetTorusUV(point),
                .material = material
        };
        return true;
    }
    return false;
}

void Torus::GetAABBBox() {
    auto a = axis->basis[0];
    auto b = axis->basis[1];
    this->box = std::make_shared<AABBBox>(center - radius - radius_cube, center + radius + radius_cube);
}

glm::vec2 Torus::GetTorusUV(glm::vec3 position) const {
    auto local = this->axis->GetLocal(position - center);
    auto theta = atan2(local[1], local[0]);
    return {
            theta / (2.0f * PI) + 0.5f,
            local[2] / (2.0f * radius_cube) + 0.5f
    };
}
