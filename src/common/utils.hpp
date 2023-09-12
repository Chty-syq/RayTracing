//
// Created by syq on 23-7-4.
//
#pragma once
#include <iostream>
#include <random>
#include <cmath>
#include <thread>
#include <chrono>
#include "core/transformation.hpp"

namespace utils {
    template<class T> void VectorMerge(vector<T>& v1, vector<T> v2) {
        v1.insert(v1.end(), v2.begin(), v2.end());
    }

    glm::vec3 Json2Vec3(const json &content) {
        if (!content.is_array() || content.size() != 3)  throw std::runtime_error("Unexpected Behavior Converting Json");
        return { float(content[0]), float(content[1]), float(content[2]) };
    }

    glm::quat Json2Quat(const json &content) {
        if (!content.is_array() || content.size() != 4)  throw std::runtime_error("Unexpected Behavior Converting Json");
        auto angle = glm::radians(float(content[0]));
        auto axis = glm::vec3(float(content[1]), float(content[2]), float(content[3]));
        return glm::angleAxis(angle, axis);
    }

    glm::vec3 EleWiseMin(glm::vec3 v1, glm::vec3 v2) {
        return { std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z) };
    }

    glm::vec3 EleWiseMax(glm::vec3 v1, glm::vec3 v2) {
        return { std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z) };
    };

    void PrintVec3(glm::vec3 v) {
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        cout << endl << v.x << " " << v.y << " " << v.z << endl;
    }

    void ShowProgressRate(float rate) {
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);

        int bar_width = 70;
        cout << "[";
        int position = int((float)bar_width * rate);
        for (int i = 0; i < bar_width; ++i) {
            if (i < position)           cout << "=";
            else if (i == position)     cout << ">";
            else                        cout << " ";
        }
        cout << "] " << int(rate * 100.0) << " %\r";
        cout.flush();
    }
}