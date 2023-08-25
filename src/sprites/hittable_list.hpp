//
// Created by syq on 23-8-23.
//

#pragma once
#include "sprites/hittable.hpp"

class HittableList: public Hittable {
public:
    vector<shared_ptr<Hittable>> list;

    HittableList() = default;
    ~HittableList() override = default;

    void AddHittable(const shared_ptr<Hittable>& hittable);
    void Clear();

    bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &result) const override;
};

void HittableList::AddHittable(const shared_ptr<Hittable>& hittable) {
    list.push_back(hittable);
}

void HittableList::Clear() {
    list.clear();
}

bool HittableList::Hit(const Ray &ray, float t_min, float t_max, HitRecord &result) const {
    bool hit_any = false;
    float t_smallest = 1e9;
    for(const auto& hittable: list) {
        HitRecord temp;
        if (hittable->Hit(ray, t_min, t_max, temp) && temp.t < t_smallest) {
            t_smallest = temp.t;
            result = temp;
            hit_any = true;
        }
    }
    return hit_any;
}
