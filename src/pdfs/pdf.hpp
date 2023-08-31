//
// Created by syq on 23-8-30.
//

#pragma once

#include "common/defs.hpp"

class PDF { //probability density function
public:
    virtual float Value(glm::vec3 direction) const = 0;  //函数值
    virtual glm::vec3 Sample() const = 0;   //采样
};
