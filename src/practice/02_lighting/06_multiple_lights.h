// Created by SelFree on 2019/03/25.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_PRACTICE_02_LIGHTING_06_MULTIPLE_LIGHTS_H_
#define SRC_PRACTICE_02_LIGHTING_06_MULTIPLE_LIGHTS_H_

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include "src/framework.h"
#include "src/common/shader.h"
#include "src/common/gl_helper.h"
#include "src/common/camera_move.h"

namespace lighting {

class MultipleLights : public framework::IPencil
            , public framework::CameraMove {
 public:
    MultipleLights();
    ~MultipleLights();
    void Init();
    void Draw();

    void Attached();
    void Dettached();

    void ProcessInput();

    // IMouseEvent
    void Position(double xpos, double ypos);
    void Scroll(double xoffset, double yoffset);

 private:
    gl_helper::VertexArrayObject    light_vao_;
    gl_helper::VertexArrayObject    object_vao_;
    gl_helper::Texture2D texture1_;
    gl_helper::Texture2D texture2_;

    Shader  light_shader_;
    Shader  object_shader_;

    framework::ProcessInputDelegate<MultipleLights> delegate_;
};

}  // namespace lighting

#endif  // SRC_PRACTICE_02_LIGHTING_06_MULTIPLE_LIGHTS_H_
