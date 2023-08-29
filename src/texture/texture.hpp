//
// Created by syq on 23-8-25.
//

#pragma once

#include "common/defs.hpp"

class Texture {
public:
    Texture() = default;
    virtual ~Texture() = default;
    virtual glm::vec3 Sample(float u, float v) const = 0;
};
