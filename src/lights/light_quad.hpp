//
// Created by syq on 23-8-30.
//

#pragma once
#include "pdfs/pdf.hpp"

#define MESH_PATH (fs::current_path().parent_path() / "assets" / "mesh_triangle" / "basic" / "quad.obj")

class LightQuad: public Mesh {
private:
    float width;
    float height;

public:
    LightQuad(const shared_ptr<Material>& material, Transformation transformation);
    ~LightQuad() override = default;
    float PDFValue(glm::vec3 origin, glm::vec3 v) const override;
    glm::vec3 Random(glm::vec3 origin) const override;
};

LightQuad::LightQuad(const shared_ptr<Material>& material, Transformation transformation): Mesh(MESH_PATH, material, transformation) {
    this->width = transformation.size[0] * 2.0f;
    this->height = transformation.size[2] * 2.0f;
}

glm::vec3 LightQuad::Random(glm::vec3 origin) const {
    auto center = transformation.position;
    auto point = center + glm::vec3(MagicRandom::Float(-0.5, 0.5) * width, 0.0f, MagicRandom::Float(-0.5, 0.5) * height);
    return point - origin;
}

float LightQuad::PDFValue(glm::vec3 origin, glm::vec3 v) const {
    HitRecord hit;
    Ray ray(origin, v);
    if (this->Hit(ray, T_MIN, T_MAX, hit)) {
        float area = width * height;
        float cosine = fabs(glm::dot(hit.normal, v)) / glm::length(v);
        return glm::dot(v, v) / (area * cosine);
    }
    return 0.0f;
}




