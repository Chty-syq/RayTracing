//
// Created by syq on 23-8-23.
//

#pragma once
#include "common/defs.hpp"

class Ray {
public:
    glm::vec3 origin;
    glm::vec3 direction;
    Ray() = default;
    ~Ray() = default;
    Ray(const glm::vec3 origin, const glm::vec3 direction): origin(origin), direction(direction) {
        this->direction = glm::normalize(this->direction);
    }
    glm::vec3 PointAt(const float t) const {
        return origin + t * direction;
    }
};
