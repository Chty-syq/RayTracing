//
// Created by syq on 23-8-30.
//

#pragma once

#include "common/defs.hpp"

class OrthoBases {
public:
    glm::vec3 basis[3]{};
    explicit OrthoBases(glm::vec3 normal);
    glm::vec3 GetLocal(glm::vec3 position);
};

OrthoBases::OrthoBases(glm::vec3 normal) {
    this->basis[2] = glm::normalize(normal);
    auto world_up = (normal.x > 0.9f) ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f);
    this->basis[1] = glm::normalize(glm::cross(this->basis[2], world_up));
    this->basis[0] = glm::normalize(glm::cross(this->basis[2], this->basis[1]));
}

glm::vec3 OrthoBases::GetLocal(glm::vec3 position) {
    auto local = glm::vec3(0.0f);
    for(int i = 0; i < 3; ++i)  local += basis[i] * position[i];
    return local;
}
