//
// Created by syq on 23-8-25.
//

#pragma once

#include "common/defs.hpp"

class Texture {
public:
    Texture() = default;
    virtual ~Texture() = default;
    virtual glm::vec3 Sample(glm::vec2 tex_coord) const = 0;
};
