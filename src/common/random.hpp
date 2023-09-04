//
// Created by syq on 23-9-4.
//

#pragma once
#include <iostream>
#include <random>
#include <cmath>
#include <thread>
#include <chrono>
#include "common/defs.hpp"

class MagicRandom {
public:
    static inline std::mt19937& generator() {
        static thread_local std::mt19937 gen(std::random_device{}());
        return gen;
    }
    static int Int(int s, int t) {
        std::uniform_int_distribution<int> dist(s, t);
        return dist(generator());
    }

    static float Float(float s, float t) {
        std::uniform_real_distribution<float> dist(s, t);
        return dist(generator());
    }
    static glm::vec3 UnitVector() {
        glm::vec3 position;
        do {
            position = glm::vec3(Float(0, 1), Float(0, 1), Float(0, 1)) * 2.0f - glm::vec3(1.0f);
        } while(glm::length(position) >= 1.0f);
        return position;
    }
};
