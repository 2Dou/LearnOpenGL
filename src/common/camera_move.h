// Created by SelFree on 2019/03/04.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_COMMON_CAMERA_MOVE_H_
#define SRC_COMMON_CAMERA_MOVE_H_

#include "src/common/event.h"
#include "src/framework.h"

namespace framework {

class CameraMove : public IMouseEvent {
 public:
    CameraMove();
    virtual ~CameraMove();

    virtual void ProcessInput();

    // IMouseEvent
    virtual void Position(double xpos, double ypos);
    virtual void Scroll(double xoffset, double yoffset);

    void Reset();

 private:
    bool first_mouse_move_;
};

}  // namespace framework


#endif  // SRC_COMMON_CAMERA_MOVE_H_
