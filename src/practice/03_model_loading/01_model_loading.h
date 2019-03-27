// Created by SelFree on 2019/03/26.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_PRACTICE_03_MODEL_LOADING_01_MODEL_LOADING_H_
#define SRC_PRACTICE_03_MODEL_LOADING_01_MODEL_LOADING_H_

#include <glad/glad.h>

#include "src/framework.h"
#include "src/common/shader.h"
#include "src/common/gl_helper.h"
#include "src/common/camera_move.h"
#include "src/common/model.h"

namespace model_loading {

class ModelLoading : public framework::IPencil
            , public framework::CameraMove {
 public:
    ModelLoading();
    ~ModelLoading();

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

    Shader  light_shader_;
    Shader  object_shader_;
    Model   model_;

    framework::ProcessInputDelegate<ModelLoading> delegate_;
};

}  // namespace model_loading


#endif  // SRC_PRACTICE_03_MODEL_LOADING_01_MODEL_LOADING_H_
