//
// Created by syq on 23-8-25.
//

#pragma once

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <queue>
#include "sprites/hittable.hpp"
#include "common/structs.hpp"
#include "common/utils.hpp"

class Mesh: public Hittable {
private:
    void ProcessNode(aiNode *node, const aiScene *scene);
    void ProcessMesh(aiMesh *mesh, const aiScene *scene);
public:
    shared_ptr<Material> material;
    VertexArr vertices;
    IndiceArr indices;

    Mesh(const std::string& path, const shared_ptr<Material>& material);
    ~Mesh() override = default;

    bool HitTriangle(const Ray &ray, float t_min, float t_max, HitRecord &hit, Vertex p0, Vertex p1, Vertex p2, glm::vec3 normal) const;
    bool Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const override;
};

Mesh::Mesh(const std::string &path, const shared_ptr<Material> &material): material(material) {
    Assimp::Importer importer;
    auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_FixInfacingNormals | aiProcess_OptimizeMeshes);
    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        throw std::runtime_error(importer.GetErrorString());
    }
    //this->directory = path.substr(0, path.find_last_of('/'));
    this->ProcessNode(scene->mRootNode, scene);
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
        m_vertices.emplace_back(
                glm::vec3(position.x / 100.0f, position.y / 100.0f, position.z / 100.0f),
                glm::vec2(tex_coord.x, tex_coord.y),
                glm::vec3(normal.x, normal.y, normal.z)
        );
    }
    for(int i = 0; i < mesh->mNumFaces; ++i) {
        auto face = mesh->mFaces[i];
        for(int k = 0; k < face.mNumIndices; ++k) {
            m_indices.push_back(face.mIndices[k] + vertices.size());
        }
    }
    utils::VectorMerge(vertices, m_vertices);
    utils::VectorMerge(indices, m_indices);
    //vertices.insert(vertices.end(), m_vertices.begin(), m_vertices.end());
//    std::string diffuse_map = "empty";
//    std::string specular_map = "empty";
//    if(mesh->mMaterialIndex >= 0) {
//        auto material = scene->mMaterials[mesh->mMaterialIndex];
//        diffuse_map = this->ProcessTexture(material, aiTextureType_DIFFUSE);
//        specular_map = this->ProcessTexture(material, aiTextureType_SPECULAR);
//    }
//    this->meshes.emplace_back(vertices, indices, diffuse_map, specular_map);
}

bool Mesh::HitTriangle(const Ray &ray, float t_min, float t_max, HitRecord &hit, Vertex p0, Vertex p1, Vertex p2, glm::vec3 normal) const {
    if (glm::dot(ray.direction, normal) == 0.0f)  return false;
    float t = (glm::dot(p0.position, normal) - glm::dot(ray.origin, normal)) / glm::dot(ray.direction, normal);
    if (t < t_min || t > t_max)  return false;

    auto r = ray.PointAt(t) - p0.position;
    auto q1 = p1.position - p0.position;
    auto q2 = p2.position - p0.position;
    auto q1q1 = glm::dot(q1, q1);
    auto q2q2 = glm::dot(q2, q2);
    auto q1q2 = glm::dot(q1, q2);
    float determinant = 1.0f / (q1q1 * q2q2 - q1q2 * q1q2);
    float w1 = determinant * (q2q2 * glm::dot(r, q1) - q1q2 * glm::dot(r, q2));
    float w2 = determinant * (q1q1 * glm::dot(r, q2) - q1q2 * glm::dot(r, q1));
    float w0 = 1.0f - w1 - w2;

    if (w1 < 0.0f || w2 < 0.0f || w1 + w2 > 1.0f)  return false;
    hit = {
            .t = t,
            .position = ray.PointAt(t),
            .normal = p0.normal * w0 + p1.normal * w1 + p2.normal * w2,
            .material = material
    };
    if (glm::dot(hit.normal, ray.direction) > 0.0f)
        hit.normal = -hit.normal;
    return true;
}

bool Mesh::Hit(const Ray &ray, float t_min, float t_max, HitRecord &hit) const {
    bool hit_any = false;
    float t_smallest = 1e9;
    for(int i = 0; i < indices.size(); i += 3) {
        auto p0 = vertices[indices[i + 0]];
        auto p1 = vertices[indices[i + 1]];
        auto p2 = vertices[indices[i + 2]];
        auto normal = glm::normalize(glm::cross(p1.position - p0.position, p2.position - p0.position)); //face normal
        HitRecord temp;
        if (this->HitTriangle(ray, t_min, t_max, temp, p0, p1, p2, normal) && temp.t < t_smallest) {
            hit = temp;
            t_smallest = temp.t;
            hit_any = true;
        }
    }
    return hit_any;
}
