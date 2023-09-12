//
// Created by syq on 23-9-11.
//

#pragma once
#include "hittable/mesh.hpp"
#include "core/transformation.hpp"
#include "core/ortho_bases.hpp"
#include "common/defs.hpp"

class Quad: public Mesh {
private:
    static const std::string MESH_PATH;

public:
    float width;
    float height;
    shared_ptr<OrthoBases> bases;
    Quad(const shared_ptr<Material>& material, Transformation transformation);
    ~Quad() override = default;
};

const std::string Quad::MESH_PATH = fs::current_path().parent_path() / "assets" / "mesh_triangle" / "basic" / "quad.obj";

Quad::Quad(const shared_ptr<Material>& material, Transformation transformation): Mesh(MESH_PATH, material, transformation) {
    this->width = transformation.size[0] * 2.0f;
    this->height = transformation.size[2] * 2.0f;
    auto rotate = glm::mat4_cast(transformation.rotate);
    auto x = glm::mat3(rotate) * glm::vec3(1.0f, 0.0f, 0.0f);
    auto y = glm::mat3(rotate) * glm::vec3(0.0f, 1.0f, 0.0f);
    auto z = glm::mat3(rotate) * glm::vec3(0.0f, 0.0f, 1.0f);
    this->bases = std::make_shared<OrthoBases>(x, y, z);
}

