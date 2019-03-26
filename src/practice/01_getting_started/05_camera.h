// Created by SelFree on 2019/03/02.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_PRACTICE_01_GETTING_STARTED_05_CAMERA_H_
#define SRC_PRACTICE_01_GETTING_STARTED_05_CAMERA_H_

#include "src/util/util.h"
#include "src/common/gl_helper.h"
#include "src/framework.h"
#include "src/common/shader.h"
#include "src/common/event.h"

namespace getting_started {

class Camera : public framework::IPencil
            , public framework::IMouseEvent {
 public:
    Camera();
    ~Camera();

    // IPencil
    void Attached();
    void Dettached();
    void Init();
    void Draw();
    void ProcessInput();

    // IMouseEvent
    void Position(double xpos, double ypos);
    void Scroll(double xoffset, double yoffset);

 private:
    bool first_mouse_move_ = true;

    gl_helper::VertexArrayObject vao_;
    gl_helper::Texture2D texture1_;
    gl_helper::Texture2D texture2_;
    Shader  shader_;

    framework::ProcessInputDelegate<Camera> delegate_;
};

}  // namespace getting_started

#endif  // SRC_PRACTICE_01_GETTING_STARTED_05_CAMERA_H_
