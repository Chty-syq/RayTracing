//
// Created by syq on 23-7-4.
//
#pragma once
#include <iostream>
#include <random>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>
#include "core/transformation.hpp"

namespace utils {
    template<class T> void VectorMerge(vector<T>& v1, vector<T> v2) {
        v1.insert(v1.end(), v2.begin(), v2.end());
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