//
// Created by syq on 23-8-29.
//

#pragma once

#include "material/material.hpp"
#include "texture/texture.hpp"

class DiffuseLight: public Material {
public:
    shared_ptr<Texture> texture;
    explicit DiffuseLight(const shared_ptr<Texture>& texture): texture(texture) {}

    bool Scatter(const Ray &in, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const override;
    glm::vec3 Emitted(const Ray &in, const HitRecord &hit) const override;
};

bool DiffuseLight::Scatter(const Ray &in, const HitRecord &hit, glm::vec3 &attenuation, Ray &scattered) const {
    return false;
}

glm::vec3 DiffuseLight::Emitted(const Ray &in, const HitRecord &hit) const {
    auto tex_coord = hit.tex_coord;
    return texture->Sample(tex_coord.x, tex_coord.y);
}
