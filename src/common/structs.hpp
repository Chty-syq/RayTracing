//
// Created by syq on 23-7-13.
//

#pragma once

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include <vector>

struct Vertex {
    glm::vec3   position;
    glm::vec2   tex_coord;
    glm::vec3   normal;
    Vertex(glm::vec3 position, glm::vec2 tex_coord, glm::vec3 normal): position(position), tex_coord(tex_coord), normal(normal) {}
};
