// Created by SelFree on 2019/2/28.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_PRACTICE_01_GETTING_STARTED_02_TEXTURE_TEST_H_
#define SRC_PRACTICE_01_GETTING_STARTED_02_TEXTURE_TEST_H_

#include "src/util/util.h"
#include "src/gl_helper.h"
#include "src/framework.h"
#include "src/shader.h"

namespace getting_started {

class TextureTest : public framework::IPencil {
 public:
    TextureTest();
    ~TextureTest();

    void Init();
    void Draw();

 private:
    gl_helper::VertexArrayObject vao_;
    gl_helper::Texture2D texture1_;
    gl_helper::Texture2D texture2_;
    Shader  shader_;
};

}  // namespace getting_started

#endif  // SRC_PRACTICE_01_GETTING_STARTED_02_TEXTURE_TEST_H_
