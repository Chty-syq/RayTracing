//
// Created by syq on 23-8-18.
//

#pragma once

#include "common/defs.hpp"

class Transformation {
public:
    glm::vec3 position;
    glm::quat rotate;
    glm::vec3 size;
    glm::mat4 model{};

    static constexpr glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f);
    static constexpr glm::quat DEFAULT_ROTATE = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 DEFAULT_SIZE = glm::vec3(1.0f);

    Transformation(glm::vec3 position, glm::quat rotate, glm::vec3 size): position(position), rotate(rotate), size(size) {
        auto m_translate = glm::translate(glm::mat4(1.0f), position);
        auto m_rotate = glm::mat4_cast(rotate);
        auto m_scale = glm::scale(glm::mat4(1.0f), size);
        this->model = m_translate * m_rotate * m_scale;
    }
    Transformation(): Transformation(DEFAULT_POSITION, DEFAULT_ROTATE, DEFAULT_SIZE) {}
    static Transformation FromPosition(glm::vec3 position) {
        return { position, DEFAULT_ROTATE, DEFAULT_SIZE };
    }
    static Transformation FromSize(glm::vec3 size) {
        return { DEFAULT_POSITION, DEFAULT_ROTATE, size };
    }
    static Transformation FromRotate(glm::quat rotate) {
        return { DEFAULT_POSITION, rotate, DEFAULT_SIZE };
    }

    void Apply(Vertex &vertex) const {
        vertex.position = glm::vec3(this->model * glm::vec4(vertex.position, 1.0f));
        vertex.normal = glm::mat3(glm::transpose(glm::inverse(model))) * vertex.normal;
    }
};
