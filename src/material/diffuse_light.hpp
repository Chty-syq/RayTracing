//
// Created by syq on 23-8-29.
//

#pragma once

#include "material/material.hpp"
#include "texture/texture.hpp"

class DiffuseLight: public Material {
public:
    shared_ptr<Texture> albedo;
    explicit DiffuseLight(const shared_ptr<Texture>& albedo): albedo(albedo) {}
    explicit DiffuseLight(glm::vec3 albedo): albedo(std::make_shared<TextureColor>(albedo)) {}
    glm::vec3 Emitted(const Ray &in, const HitRecord &hit) const override;
};

glm::vec3 DiffuseLight::Emitted(const Ray &in, const HitRecord &hit) const {
    auto tex_coord = hit.tex_coord;
    if (glm::dot(hit.normal, in.direction) < 0.0f)
        return albedo->Sample(tex_coord);
    else
        return glm::vec3(0.0f);
}
