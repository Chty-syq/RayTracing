//
// Created by chty on 23-7-4.
//
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <common/defs.hpp>
#include "framework/ray_tracing/ray.hpp"

class Camera {
private:
    glm::vec3 lower_left_corner{};    //视锥左下角
    glm::vec3 horizontal{};   //视锥平面水平方向跨度
    glm::vec3 vertical{};     //视锥平面垂直方向跨度
    void UpdateState();

public:
    glm::vec3 camera_pos;
    glm::vec3 camera_front{};
    glm::vec3 camera_up{};
    glm::vec3 camera_right{};
    glm::vec3 world_up{};
    float pitch;
    float yaw;
    float fov;      //视锥大小
    float aspect;   //屏幕宽高比
    float focus;    //焦距
    explicit Camera(glm::vec3 camera_pos);
    Camera(glm::vec3 camera_pos, glm::vec3 target);
    glm::mat4 GetViewMat() const;
    glm::mat4 GetPerspectiveMat() const;
    void KeyboardInput(Direction direction, float duration);
    void MouseMove(float offset_x, float offset_y);
    void MouseScroll(float offset);
    Ray GetRay(float s, float t);
};

Camera::Camera(glm::vec3 camera_pos): camera_pos(camera_pos) {
    this->fov = CAMERA_FOV;
    this->aspect = (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT;
    this->yaw = CAMERA_YAW;
    this->pitch = CAMERA_PITCH;
    this->focus = CAMERA_FOCUS;
    this->world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->UpdateState();
}

Camera::Camera(glm::vec3 camera_pos, glm::vec3 target): camera_pos(camera_pos) {
    this->fov = CAMERA_FOV;
    this->aspect = (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT;
    this->focus = CAMERA_FOCUS;
    this->world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    camera_front = glm::normalize(camera_pos - target);
    camera_right = glm::normalize(glm::cross(camera_front, world_up));
    camera_up = glm::normalize(glm::cross(camera_right, camera_front));

    float half_height = tan(glm::radians(fov * 0.5f));
    float half_width = aspect * half_height;

    lower_left_corner = camera_pos - half_width * camera_right - half_height * camera_up - camera_front;
    horizontal = 2.0f * half_width * camera_right;
    vertical = 2.0f * half_height * camera_up;
}

void Camera::UpdateState() {
    camera_front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    camera_front.y = sin(glm::radians(pitch));
    camera_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    camera_front = glm::normalize(camera_front);
    camera_right = glm::normalize(glm::cross(camera_front, world_up));
    camera_up = glm::normalize(glm::cross(camera_right, camera_front));

    float half_height = tan(glm::radians(fov * 0.5f));
    float half_width = aspect * half_height;

    lower_left_corner = camera_pos - half_width * camera_right - half_height * camera_up - camera_front;
    horizontal = 2.0f * half_width * camera_right;
    vertical = 2.0f * half_height * camera_up;
}

void Camera::KeyboardInput(Direction direction, float duration) {
    float distance = CAMERA_SPEED * duration;
    switch (direction) {
        case forward:
            camera_pos += camera_front * distance; break;
        case backward:
            camera_pos -= camera_front * distance; break;
        case left:
            camera_pos -= camera_right * distance; break;
        case right:
            camera_pos += camera_right * distance; break;
        case up:
            camera_pos.y += distance; break;
        case down:
            camera_pos.y -= distance; break;
    }
}

void Camera::MouseMove(float offset_x, float offset_y) {
    yaw += offset_x * CAMERA_SENSITIVITY;
    pitch += offset_y * CAMERA_SENSITIVITY;
    pitch = std::min(pitch, 89.0f);
    pitch = std::max(pitch, -89.0f);
    this->UpdateState();
}

void Camera::MouseScroll(float offset) {
    fov -= offset;
    fov = std::max(fov, 1.0f);
    fov = std::min(fov, 45.0f);
}

glm::mat4 Camera::GetViewMat() const {
    return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
}

glm::mat4 Camera::GetPerspectiveMat() const {
    return glm::perspective(glm::radians(fov), aspect, 0.1f, 100.0f);;
}

Ray Camera::GetRay(float s, float t) {
    return { camera_pos, lower_left_corner + s * horizontal + t * vertical - camera_pos };
}
