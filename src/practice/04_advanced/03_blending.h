// Created by SelFree on 2019/03/29.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_PRACTICE_04_ADVANCED_03_BLENDING_H_
#define SRC_PRACTICE_04_ADVANCED_03_BLENDING_H_

#include <glad/glad.h>

#include "src/framework.h"
#include "src/common/shader.h"
#include "src/common/gl_helper.h"
#include "src/common/camera_move.h"

namespace advanced {

class Blending : public framework::IPencil
            , public framework::CameraMove {
 public:
    Blending();
    ~Blending();

    void Init();
    void Draw();

    void Attached();
    void Dettached();

    void ProcessInput();

    // IMouseEvent
    void Position(double xpos, double ypos);
    void Scroll(double xoffset, double yoffset);

 private:
    gl_helper::VertexArrayObject    floor_vao_;
    gl_helper::VertexArrayObject    object_vao_;
    gl_helper::VertexArrayObject    grass_vao_;
    gl_helper::Texture2D texture1_;
    gl_helper::Texture2D texture2_;
    gl_helper::Texture2D texture3_;
    Shader  object_shader_;

    framework::ProcessInputDelegate<Blending> delegate_;
};

}  // namespace advanced


#endif  // SRC_PRACTICE_04_ADVANCED_03_BLENDING_H_
