//
// Created by syq on 23-7-4.
//
#pragma once
#include <iostream>
#include <random>
#include <cmath>

namespace utils {
    inline std::mt19937& generator() {
        static thread_local std::mt19937 gen(std::random_device{}());
        return gen;
    }

    int RandomInt(int s, int t) {
        std::uniform_int_distribution<int> dist(s, t);
        return dist(generator());
    }

    float RandomFloat(float s, float t) {
        std::uniform_real_distribution<float> dist(s, t);
        return dist(generator());
    }

    glm::vec3 RandomForUnitSphere() {
        glm::vec3 position;
        do {
            position = glm::vec3(RandomFloat(0, 1), RandomFloat(0, 1), RandomFloat(0, 1)) * 2.0f - glm::vec3(1.0f);
        } while(glm::length(position) >= 1.0f);
        return position;
    }
}