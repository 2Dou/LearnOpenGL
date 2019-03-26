// Created by SelFree on 2019/03/01.
// Copyright © 2019年 SelFree. All rights reserved.

#include <glm/gtc/matrix_transform.hpp>
#include "src/common/camera.h"

namespace camera {

Camera *Instance() {
    return Camera::Instance();
}

Camera::Camera(glm::vec3 position
                    , glm::vec3 up
                    , float yaw
                    , float pitch) {
    this->Reset(position, up, yaw, pitch);
}

Camera::~Camera() {
}

void Camera::Reset(glm::vec3 position
                    , glm::vec3 up
                    , float yaw
                    , float pitch) {
    front_    = glm::vec3(0.0f, 0.0f, -1.0f);
    zoom_     = kZoom;
    yaw_      = yaw;
    pitch_    = pitch;
    position_ = position;
    world_up_ = up;

    movement_speed_    = kMoveSpeed;
    mouse_sensitivity_ = kSensitivity;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::ProcessMovement(uint32_t directions, float delta) {
    float velocity = movement_speed_ * delta;

    if (directions & kMoveForward) {
        position_ += front_ * velocity;
    }
    if (directions & kMoveBackward) {
        position_ -= front_ * velocity;
    }
    if (directions & kMoveLeft) {
        position_ -= right_ * velocity;
    }
    if (directions & kMoveRight) {
        position_ += right_ * velocity;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset
        , GLboolean constrain_pitch) {
    xoffset *= mouse_sensitivity_;
    yoffset *= mouse_sensitivity_;

    yaw_   += xoffset;
    pitch_ += yoffset;

    // Make sure that when pitch is out of bounds,
    // screen doesn't get flipped
    if (constrain_pitch) {
        if (pitch_ > 89.0f) {
            pitch_ = 89.0f;
        } else if (pitch_ < -89.0f) {
            pitch_ = -89.0f;
        }
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    zoom_ -= yoffset;
    if (zoom_ <= 1.0f) {
        zoom_ = 1.0f;
    } else if (zoom_ >= 45.0f) {
        zoom_ = 45.0f;
    }
}

glm::vec3 Camera::get_position() const {
    return position_;
}
glm::vec3 Camera::get_front() const {
    return front_;
}
glm::vec3 Camera::get_up() const {
    return up_;
}
glm::vec3 Camera::get_right() const {
    return right_;
}
float Camera::get_zoom() const {
    return zoom_;
}

void Camera::updateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    this->front_ = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    right_ = glm::normalize(glm::cross(front_, world_up_));
    up_    = glm::normalize(glm::cross(right_, front_));
}

}  // namespace camera
