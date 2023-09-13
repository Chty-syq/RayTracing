//
// Created by syq on 23-8-25.
//

#pragma once

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <igl/ray_mesh_intersect.h>
#include <igl/AABB.h>
#include <queue>
#include "hittable/hittable.hpp"
#include "core/transformation.hpp"
#include "common/structs.hpp"
#include "common/utils.hpp"
#include "common/convert.hpp"

class Mesh: public Hittable {
private:
    igl::AABB<Eigen::MatrixXd, 3> tree;
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    void ProcessNode(aiNode *node, const aiScene *scene);
    void ProcessMesh(aiMesh *mesh, const aiScene *scene);

public:
    shared_ptr<Material> material;
    VertexArr vertices;
    IndiceArr indices;
    Transformation transformation;

    Mesh(const std::string& path, const shared_ptr<Material>& material, Transformation transformation);
    ~Mesh() override = default;

    bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const override;
    void GetAABBBox() override;
};

Mesh::Mesh(const std::string &path, const shared_ptr<Material> &material, Transformation transformation): material(material), transformation(transformation) {
    Assimp::Importer importer;
    auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_FixInfacingNormals | aiProcess_OptimizeMeshes);
    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        throw std::runtime_error(importer.GetErrorString());
    }
    //this->directory = path.substr(0, path.find_last_of('/'));
    this->ProcessNode(scene->mRootNode, scene);
    this->V = converter::VertexArr2Eigen(this->vertices);
    this->F = converter::IndiceArr2Eigen(this->indices);
    this->tree.init(this->V, this->F);
    importer.FreeScene();
}

void Mesh::ProcessNode(aiNode *node, const aiScene *scene) { //bfs扫描结点树
    for(int i = 0; i < node->mNumMeshes; ++i)
        this->ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene);
    for(int i = 0; i < node->mNumChildren; ++i)
        this->ProcessNode(node->mChildren[i], scene);
}

void Mesh::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
    VertexArr m_vertices;
    IndiceArr m_indices;
    for(int i = 0; i < mesh->mNumVertices; ++i) {
        auto position = mesh->mVertices[i];
        auto normal = mesh->mNormals[i];
        auto tex_coord = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i] : aiVector3D(0.0f);
        Vertex vertex(
                glm::vec3(position.x, position.y, position.z),
                glm::vec2(tex_coord.x, tex_coord.y),
                glm::normalize(glm::vec3(normal.x, normal.y, normal.z))
        );
        transformation.Apply(vertex);
        m_vertices.emplace_back(vertex);
    }
    for(int i = 0; i < mesh->mNumFaces; ++i) {
        auto face = mesh->mFaces[i];
        for(int k = 0; k < face.mNumIndices; ++k) {
            m_indices.push_back(face.mIndices[k] + vertices.size());
        }
    }
    utils::VectorMerge(vertices, m_vertices);
    utils::VectorMerge(indices, m_indices);
}

bool Mesh::Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const {
    auto hits = vector<igl::Hit>();
    auto o = converter::Vec2Eigen(ray.origin);
    auto v = converter::Vec2Eigen(ray.direction);

    tree.intersect_ray(this->V, this->F, o, v, hits);
    std::sort(hits.begin(), hits.end(), [](auto &h1, auto &h2) { return h1.t < h2.t; });

    for(const auto& point: hits) if(point.t >= t_min && point.t <= t_max) {
        float w1 = point.u;
        float w2 = point.v;
        float w0 = 1 - w1 - w2;
        auto p0 = vertices[this->F(point.id, 0)];
        auto p1 = vertices[this->F(point.id, 1)];
        auto p2 = vertices[this->F(point.id, 2)];
        hit = {
                .t = point.t,
                .position = ray.PointAt(point.t),
                .normal = glm::normalize(p0.normal * w0 + p1.normal * w1 + p2.normal * w2),
                .tex_coord = p0.tex_coord * w0 + p1.tex_coord * w1 + p2.tex_coord * w2,
                .material = material
        };
        if (glm::dot(hit.normal, ray.direction) > 0.0f)
            hit.normal = -hit.normal;
        return true;
    }
    return false;
}

void Mesh::GetAABBBox() {
    auto box_left = glm::vec3(1e9);
    auto box_right = glm::vec3(0.0f);
    for(const auto &vertex: vertices) {
        box_left = utils::EleWiseMin(box_left, vertex.position);
        box_right = utils::EleWiseMax(box_right, vertex.position);
    }
    this->box = std::make_shared<AABBBox>(box_left, box_right);
}
