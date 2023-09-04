//
// Created by syq on 23-8-23.
//

#pragma once
#include "hittable/hittable.hpp"
#include "common/utils.hpp"
#include "common/random.hpp"

class HittableList: public Hittable {
public:
    vector<shared_ptr<Hittable>> list;
    vector<shared_ptr<AABBBox>> bvh_tree;

    HittableList() = default;
    ~HittableList() override = default;

    void AddHittable(const shared_ptr<Hittable>& hittable);

    void BuildBVH();
    void Clear();
    bool Empty() const;

    bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const override;
    void GetAABBBox() override {};

private:
    int root = 1;
    bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit, int node, int l, int r) const;
    void BuildBVH(int node, int left, int right);

    float PDFValue(glm::vec3 origin, glm::vec3 v) const override;
    glm::vec3 Random(glm::vec3 origin) const override;
};

void HittableList::AddHittable(const shared_ptr<Hittable>& hittable) {
    hittable->GetAABBBox();
    list.push_back(hittable);
}

void HittableList::BuildBVH(int node, int l, int r) {
    if (l == r) {
        bvh_tree[node] = list[l]->box;
        return;
    }
    int mid = (l + r) >> 1;
    BuildBVH(node << 1, l, mid);
    BuildBVH(node << 1 | 1, mid + 1, r);
    auto b1 = bvh_tree[node << 1];
    auto b2 = bvh_tree[node << 1 | 1];
    auto box_left = utils::EleWiseMin(b1->box_left, b2->box_left);
    auto box_right = utils::EleWiseMax(b1->box_right, b2->box_right);
    bvh_tree[node] = std::make_shared<AABBBox>(box_left, box_right);
}

void HittableList::BuildBVH() {
    std::sort(list.begin(), list.end(), [](const shared_ptr<Hittable> &h1, const shared_ptr<Hittable> &h2) {
        return h1->box->GetCenter().x < h2->box->GetCenter().x;
    });
    bvh_tree.resize(list.size() << 2);
    BuildBVH(root, 0, (int)list.size() - 1);
}

void HittableList::Clear() {
    list.clear();
}

bool HittableList::Empty() const {
    return list.empty();
}

bool HittableList::Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit, int node, int l, int r) const {
    if (l == r) {
        return list[l]->Hit(ray, t_min, t_max, hit);
    }
    if (bvh_tree[node]->Hit(ray, t_min, t_max)) {
        HitRecord hit0, hit1;
        int mid = (l + r) >> 1;
        bool hit_l = Hit(ray, t_min, t_max, hit0, node << 1, l, mid);
        bool hit_r = Hit(ray, t_min, t_max, hit1, node << 1 | 1, mid + 1, r);
        if (hit_l && hit_r)     hit = hit0.t < hit1.t ? hit0 : hit1;
        else if (hit_l)         hit = hit0;
        else if (hit_r)         hit = hit1;
        return (hit_l || hit_r);
    }
    return false;
}

bool HittableList::Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const {
    return Hit(ray, t_min, t_max, hit, root, 0, (int)list.size() - 1);
//    bool hit_any = false;
//    float t_smallest = 1e9;
//    for(const auto& hittable: list) {
//        HitRecord temp;
//        if (hittable->Hit(ray, t_min, t_max, temp) && temp.t < t_smallest) {
//            t_smallest = temp.t;
//            hit = temp;
//            hit_any = true;
//        }
//    }
//    return hit_any;
}

float HittableList::PDFValue(glm::vec3 origin, glm::vec3 v) const {
    float sum = 0.0f;
    for(const auto &hittable: list) {
        sum += hittable->PDFValue(origin, v);
    }
    return sum / (float)list.size();
}

glm::vec3 HittableList::Random(glm::vec3 origin) const {
    int index = static_cast<int>(MagicRandom::Float(0, 0.99) * (float)list.size());
    return list[index]->Random(origin);
}
