//
// Created by syq on 23-8-28.
//

#pragma once

class AABBBox {
public:
    glm::vec3 box_left;
    glm::vec3 box_right;
    AABBBox(glm::vec3 box_left, glm::vec3 box_right): box_left(box_left), box_right(box_right) {}
    glm::vec3 GetCenter() const {
        return (box_left + box_right) / 2.0f;
    }
    bool Hit(const Ray &ray, float t_min, float t_max);
};

bool AABBBox::Hit(const Ray &ray, float t_min, float t_max) {
    for(int i = 0; i < 3; ++i) {
        float t0 = (box_left[i] - ray.origin[i]) / ray.direction[i];
        float t1 = (box_right[i] - ray.origin[i]) / ray.direction[i];
        if (t0 > t1) {
            std::swap(t0, t1);
        }
        t_min = std::max(t_min, t0);
        t_max = std::min(t_max, t1);
    }
    return (t_min < t_max);
}
