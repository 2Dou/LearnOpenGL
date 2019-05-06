// Created by SelFree on 2019/04/16.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_PRACTICE_04_ADVANCED_05_FRAME_BUFFER_H_
#define SRC_PRACTICE_04_ADVANCED_05_FRAME_BUFFER_H_

#include <glad/glad.h>

#include "src/framework.h"
#include "src/common/shader.h"
#include "src/common/gl_helper.h"
#include "src/common/camera_move.h"
#include "src/common/model.h"

namespace advanced {

class FrameBuffer : public framework::IPencil
            , public framework::CameraMove {
 public:
    FrameBuffer();
    ~FrameBuffer();

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
    gl_helper::Texture2D    texture1_;
    gl_helper::Texture2D    texture2_;
    gl_helper::Texture2D    texture3_;
    Shader  object_shader_;

    GLuint  fbo_;
    GLuint  rbo_;
    GLuint  screen_texture_;
    gl_helper::VertexArrayObject    screen_vao_;
    Shader  screen_shader_;

    framework::ProcessInputDelegate<FrameBuffer> delegate_;
};

}  // namespace advanced


#endif  // SRC_PRACTICE_04_ADVANCED_05_FRAME_BUFFER_H_
