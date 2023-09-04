//
// Created by syq on 23-8-29.
//
#pragma once
#include "texture/texture.hpp"

class TextureColor: public Texture {
private:
    glm::vec3 color{};
public:
    TextureColor() = default;
    explicit TextureColor(glm::vec3 color): color(color) {}
    ~TextureColor() override = default;

    glm::vec3 Sample(glm::vec2 tex_coord) const override;
};

glm::vec3 TextureColor::Sample(glm::vec2 tex_coord) const {
    return color;
}
