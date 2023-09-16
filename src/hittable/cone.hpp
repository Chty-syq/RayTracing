//
// Created by chty on 23-9-17.
//

#pragma once
#include "hittable/hittable_list.hpp"
#include "hittable/cone_face.hpp"
#include "hittable/circle.hpp"

class Cone: public HittableList { //圆锥
public:
    float radius;
    float height;
    glm::vec3 center;
    glm::vec3 normal{};
    shared_ptr<Material> material;

    shared_ptr<ConeFace> cone_face;
    shared_ptr<Circle> circle;

    Cone(glm::vec3 center, float radius, float height, glm::vec3 normal, const shared_ptr<Material>& material): center(center), radius(radius), height(height), material(material){
        this->normal = glm::normalize(normal);
        this->cone_face = std::make_shared<ConeFace>(center, radius, height, this->normal, material);
        this->circle = std::make_shared<Circle>(center, radius, -this->normal, material);
        this->AddHittable(cone_face);
        this->AddHittable(circle);
        this->box = cone_face->box;
        this->BuildBVH();
    }
    ~Cone() override = default;
};

