// Created by SelFree on 2019/03/04.
// Copyright © 2019年 SelFree. All rights reserved.

#include "src/camera_move.h"
#include "src/camera.h"

namespace framework {

CameraMove::CameraMove(): first_mouse_move_(true) {
}

CameraMove::~CameraMove() {
}

void CameraMove::Reset() {
    first_mouse_move_ = true;
}

void CameraMove::ProcessInput() {
    auto *window = framework::Instance()->get_window();
    uint32_t flag = 0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        flag |= camera::kMoveForward;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        flag |= camera::kMoveBackward;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        flag |= camera::kMoveLeft;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        flag |= camera::kMoveRight;
    }

    camera::Instance()->ProcessMovement(flag
        , framework::Instance()->get_frame_delta());
}


void CameraMove::Position(double xpos, double ypos) {
    static double last_x, last_y, last_time;
    if (first_mouse_move_) {
        last_x = xpos, last_y = ypos;
        last_time = glfwGetTime();
        first_mouse_move_ = false;
        return;
    }

    float xoffset = static_cast<double>(xpos - last_x);
    float yoffset = static_cast<double>(last_y - ypos);
    last_x = xpos, last_y = ypos;

    camera::Instance()->ProcessMouseMovement(xoffset, yoffset);
}

void CameraMove::Scroll(double xoffset, double yoffset) {
    camera::Instance()->ProcessMouseScroll(yoffset);
}


}  // namespace framework
