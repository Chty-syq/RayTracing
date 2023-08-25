//
// Created by syq on 23-8-25.
//

#pragma once

class Texture {
public:
    Texture() = default;
    virtual ~Texture() = default;
    virtual glm::vec3 Sample(float u, float v, glm::vec3 p) const = 0;
};
