// Created by SelFree on 2019/03/01.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_COMMON_CAMERA_H_
#define SRC_COMMON_CAMERA_H_

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include "src/util/singleton.h"

namespace camera {

class Camera;
Camera *Instance();

// Default camera values
const float kYaw         = -90.0f;
const float kPitch       = 0.0f;
const float kMoveSpeed   = 2.5f;
const float kSensitivity = 0.1f;
const float kZoom        = 45.0f;

// 移动方向定义
const uint32_t kMoveForward  = 0x01;
const uint32_t kMoveBackward = 0x02;
const uint32_t kMoveLeft     = 0x04;
const uint32_t kMoveRight    = 0x08;

class Camera : public Singleton<Camera> {
 public:
    // Constructor with vectors
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f)
        , glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)
        , float yaw = kYaw, float pitch = kPitch);
    virtual ~Camera();

    void Reset(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f)
        , glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)
        , float yaw = kYaw, float pitch = kPitch);

    // 计算观察矩阵
    glm::mat4 GetViewMatrix();

    // 处理移动事件
    void ProcessMovement(uint32_t directions, float delta);

    // 处理鼠标移动事件
    void ProcessMouseMovement(float xoffset, float yoffset
        , GLboolean constrain_pitch = true);

    // 处理鼠标滚轮事件
    void ProcessMouseScroll(float yoffset);

    glm::vec3 get_position() const;
    glm::vec3 get_front() const;
    glm::vec3 get_up() const;
    glm::vec3 get_right() const;
    float get_zoom() const;

 protected:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    virtual void updateCameraVectors();

 private:
    // Camera Attributes
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    // Euler Angles
    float yaw_;
    float pitch_;
    float zoom_;
    // Camera options
    float movement_speed_;
    float mouse_sensitivity_;
};

}  // namespace camera


#endif  // SRC_COMMON_CAMERA_H_
