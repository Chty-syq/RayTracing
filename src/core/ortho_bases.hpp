//
// Created by syq on 23-8-30.
//

#pragma once

#include "common/defs.hpp"

class OrthoBases { //正交基
public:
    glm::vec3 basis[3]{};
    explicit OrthoBases(glm::vec3 normal);
    OrthoBases(glm::vec3 x, glm::vec3 y, glm::vec3 z);
    glm::vec3 GetWorld(glm::vec3 position);
    glm::vec3 GetLocal(glm::vec3 position);
};

OrthoBases::OrthoBases(glm::vec3 normal) {
    this->basis[2] = glm::normalize(normal);
    auto world_up = (fabs(this->basis[2].x) > 0.9f) ? glm::vec3(0.0f, 1.0f, 0.0f) : glm::vec3(1.0f, 0.0f, 0.0f);
    this->basis[1] = glm::normalize(glm::cross(this->basis[2], world_up));
    this->basis[0] = glm::normalize(glm::cross(this->basis[2], this->basis[1]));
}

OrthoBases::OrthoBases(glm::vec3 x, glm::vec3 y, glm::vec3 z) {
    this->basis[0] = glm::normalize(x);
    this->basis[1] = glm::normalize(y);
    this->basis[2] = glm::normalize(z);
}


glm::vec3 OrthoBases::GetWorld(glm::vec3 position) {
    auto world = glm::vec3(0.0f);
    for(int i = 0; i < 3; ++i)  world += basis[i] * position[i];
    return world;
}

glm::vec3 OrthoBases::GetLocal(glm::vec3 position) {
    auto local = glm::vec3(0.0f);
    for(int i = 0; i < 3; ++i)  local[i] = glm::dot(position, basis[i]);
    return local;
}

