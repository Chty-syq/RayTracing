//
// Created by syq on 23-9-12.
//

#pragma once
#include "common/defs.hpp"

namespace converter {
    auto Vec2Eigen(glm::vec3 src);
    auto VertexArr2Eigen(const VertexArr &arr);
    auto IndiceArr2Eigen(const IndiceArr &arr);
}

auto converter::Vec2Eigen(glm::vec3 src) {
    return Eigen::Vector3d(src[0], src[1], src[2]);
}

auto converter::VertexArr2Eigen(const VertexArr &arr) {
    auto v = Eigen::MatrixXd(arr.size(), 3);
    for(int i = 0; i < arr.size(); ++i) {
        v(i, 0) = arr[i].position.x;
        v(i, 1) = arr[i].position.y;
        v(i, 2) = arr[i].position.z;
    }
    return v;
}

auto converter::IndiceArr2Eigen(const IndiceArr &arr) {
    auto f = Eigen::MatrixXi(arr.size() / 3, 3);
    for(int i = 0; i < arr.size(); i += 3)  {
        f(i / 3, 0) = arr[i + 0];
        f(i / 3, 1) = arr[i + 1];
        f(i / 3, 2) = arr[i + 2];
    }
    return f;
}