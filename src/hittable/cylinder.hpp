//
// Created by chty on 23-9-17.
//

#pragma once
#include "hittable/hittable_list.hpp"
#include "hittable/cylinder_face.hpp"
#include "hittable/circle.hpp"

class Cylinder: public HittableList { //圆柱
public:
    float radius;
    float height;
    glm::vec3 center;
    glm::vec3 normal{};
    shared_ptr<Material> material;

    shared_ptr<CylinderFace> cylinder_face;
    shared_ptr<Circle> circle[2];

    Cylinder(glm::vec3 center, float radius, float height, glm::vec3 normal, const shared_ptr<Material>& material): center(center), radius(radius), height(height), material(material){
        this->normal = glm::normalize(normal);
        this->cylinder_face = std::make_shared<CylinderFace>(center, radius, height, this->normal, material);
        this->circle[0] = std::make_shared<Circle>(center, radius, -this->normal, material);
        this->circle[1] = std::make_shared<Circle>(center + this->normal * height, radius, this->normal, material);
        this->AddHittable(cylinder_face);
        this->AddHittable(circle[0]);
        this->AddHittable(circle[1]);
        this->box = cylinder_face->box;
        this->BuildBVH();
    }
    ~Cylinder() override = default;
};

