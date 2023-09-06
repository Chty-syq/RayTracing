//
// Created by chty on 23-7-4.
//
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <common/defs.hpp>
#include "core/ray.hpp"

class Camera {
private:
    glm::vec3 lower_left_corner{};    //视锥左下角
    glm::vec3 horizontal{};   //视锥平面水平方向跨度
    glm::vec3 vertical{};     //视锥平面垂直方向跨度

public:
    glm::vec3 camera_pos{};
    glm::vec3 camera_front{};
    glm::vec3 camera_up{};
    glm::vec3 camera_right{};
    glm::vec3 world_up{};
    float fov{};      //视锥大小
    float aspect{};   //屏幕宽高比
    float focus{};    //焦距
    Camera() = default;
    Camera(glm::vec3 camera_pos, glm::vec3 target);
    Ray GetRay(float s, float t);
};

Camera::Camera(glm::vec3 camera_pos, glm::vec3 target): camera_pos(camera_pos) {
    this->fov = CAMERA_FOV;
    this->aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
    this->focus = CAMERA_FOCUS;
    this->world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    camera_front = glm::normalize(camera_pos - target);
    camera_right = glm::normalize(glm::cross(camera_front, world_up));
    camera_up = glm::normalize(glm::cross(camera_right, camera_front));

    float half_height = focus * tan(glm::radians(fov * 0.5f));
    float half_width = aspect * half_height;

    lower_left_corner = camera_pos - half_width * camera_right - half_height * camera_up - focus * camera_front;
    horizontal = 2.0f * half_width * camera_right;
    vertical = 2.0f * half_height * camera_up;
}

Ray Camera::GetRay(float s, float t) {
    return { camera_pos, lower_left_corner + s * horizontal + t * vertical - camera_pos };
}
