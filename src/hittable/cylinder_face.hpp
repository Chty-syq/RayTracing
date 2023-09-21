//
// Created by chty on 23-9-17.
//

#pragma once
#include "hittable/hittable.hpp"
#include "common/equation.hpp"

class CylinderFace: public Hittable { //圆柱面
public:
    float radius;
    float height;
    glm::vec3 center;
    glm::vec3 normal{};
    shared_ptr<Material> material;

    CylinderFace(glm::vec3 center, float radius, float height, glm::vec3 normal, const shared_ptr<Material>& material): center(center), radius(radius), height(height), material(material){
        this->normal = glm::normalize(normal);
        this->axis = std::make_shared<OrthoBases>(normal);
    }
    ~CylinderFace() override = default;

    bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const override;
    void GetAABBBox() override;

protected:
    shared_ptr<OrthoBases> axis;
    glm::vec2 GetCylinderFaceUV(glm::vec3 position) const;
};

bool CylinderFace::Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const {
    auto oc = ray.origin - center;
    float oc_dot_a = glm::dot(oc, this->axis->basis[0]);
    float oc_dot_b = glm::dot(oc, this->axis->basis[1]);
    float v_dot_a = glm::dot(ray.direction, this->axis->basis[0]);
    float v_dot_b = glm::dot(ray.direction, this->axis->basis[1]);
    float A = pow(v_dot_a, 2.0f) + pow(v_dot_b, 2.0f);
    float B = 2.0f * (oc_dot_a * v_dot_a + oc_dot_b * v_dot_b);
    float C = pow(oc_dot_a, 2.0f) + pow(oc_dot_b, 2.0f) - pow(radius, 2.0f);
    if (A == 0)  return false; //光线与法向平行

    vector<double> roots;
    equation::SolveQuadraticReal({A, B, C}, roots);
    std::sort(roots.begin(), roots.end());
    for(const auto &t: roots) if(t >= t_min && t <= t_max) {
        auto point = ray.PointAt((float)t);
        auto h = glm::dot(point - center, normal);
        if (h >= 0 && h <= height) {
            hit = {
                    .t = (float)t,
                    .position = point,
                    .normal = glm::normalize(point - center - h * normal),
                    .tex_coord = this->GetCylinderFaceUV(point),
                    .material = material
            };
            return true;
        }
    }
    return false;
}

void CylinderFace::GetAABBBox() {
    auto a = this->axis->basis[0];
    auto b = this->axis->basis[1];
    auto factor = sqrt(a * a + b * b);
    auto box_left = utils::EleWiseMin(center - radius * factor, center + normal * height - radius * factor);
    auto box_right = utils::EleWiseMax(center + radius * factor, center + normal * height + radius * factor);
    this->box = std::make_shared<AABBBox>(box_left, box_right);
}

glm::vec2 CylinderFace::GetCylinderFaceUV(glm::vec3 position) const {
    auto local = this->axis->GetLocal(position - center);
    auto theta = atan2(local[1], local[0]);
    return {
        theta / (2.0f * PI) + 0.5f,
        local[2] / height
    };
}
