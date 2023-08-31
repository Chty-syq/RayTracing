//
// Created by chty on 23-7-3.
//
#pragma once
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <GLFW/glfw3.h>
#include <cstdio>
#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <filesystem>
#include <stb_image.h>
#include <stb_image_write.h>
#include "common/structs.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

namespace fs = std::filesystem;

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::pair;
using std::unique_ptr;
using std::shared_ptr;

using VertexArr = std::vector<Vertex>;
using IndiceArr = std::vector<GLuint>;

const float PI = (float)acos(-1);

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

const float T_MIN = 0.001f;
const float T_MAX = 100.0f;

const float CAMERA_PITCH = 0.0;
const float CAMERA_YAW = -90.0;
const float CAMERA_SPEED = 3.0;
const float CAMERA_SENSITIVITY = 0.1;
const float CAMERA_FOV = 39.0;
const float CAMERA_FOCUS = 10.0f;

enum Direction {
    forward, backward, left, right, up, down
};
