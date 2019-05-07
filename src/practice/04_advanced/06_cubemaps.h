// Created by SelFree on 2019/05/05.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_PRACTICE_04_ADVANCED_06_CUBEMAPS_H_
#define SRC_PRACTICE_04_ADVANCED_06_CUBEMAPS_H_

#include <glad/glad.h>

#include "src/framework.h"
#include "src/common/shader.h"
#include "src/common/gl_helper.h"
#include "src/common/camera_move.h"
#include "src/common/model.h"

namespace advanced {

class Cubemaps : public framework::IPencil
            , public framework::CameraMove {
 public:
    Cubemaps();
    ~Cubemaps();

    void Init();
    void Draw();

    void Attached();
    void Dettached();

    void ProcessInput();

    // IMouseEvent
    void Position(double xpos, double ypos);
    void Scroll(double xoffset, double yoffset);

 private:
    gl_helper::VertexArrayObject    sky_vao_;
    gl_helper::CubeTexture  sky_texture_;
    Shader  sky_shader_;

    gl_helper::VertexArrayObject    box_vao_;
    gl_helper::Texture2D    box_texture_;
    Shader  box_shader_;

    Shader  model_shader_;
    Model   model_;

    framework::ProcessInputDelegate<Cubemaps> delegate_;
};

}  // namespace advanced


#endif  // SRC_PRACTICE_04_ADVANCED_06_CUBEMAPS_H_
