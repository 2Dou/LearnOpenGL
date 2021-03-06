// Created by SelFree on 2019/03/01.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_PRACTICE_01_GETTING_STARTED_04_COORDINATE_BOX_H_
#define SRC_PRACTICE_01_GETTING_STARTED_04_COORDINATE_BOX_H_

#include "src/util/util.h"
#include "src/common/gl_helper.h"
#include "src/framework.h"
#include "src/common/shader.h"

namespace getting_started {

class CoordinateBox : public framework::IPencil {
 public:
    CoordinateBox();
    ~CoordinateBox();

    void Init();
    void Draw();

 private:
    gl_helper::VertexArrayObject vao_;
    gl_helper::Texture2D texture1_;
    gl_helper::Texture2D texture2_;
    Shader  shader_;
};

}  // namespace getting_started

#endif  // SRC_PRACTICE_01_GETTING_STARTED_04_COORDINATE_BOX_H_
