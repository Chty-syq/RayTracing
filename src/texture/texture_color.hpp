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

    glm::vec3 Sample(float u, float v) const override;
};

glm::vec3 TextureColor::Sample(float u, float v) const {
    return color;
}
