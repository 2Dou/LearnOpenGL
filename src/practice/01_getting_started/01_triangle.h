// Created by SelFree on 2019/2/26.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_PRACTICE_01_GETTING_STARTED_01_TRIANGLE_H_
#define SRC_PRACTICE_01_GETTING_STARTED_01_TRIANGLE_H_

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include "src/framework.h"
#include "src/common/shader.h"
#include "src/common/gl_helper.h"

namespace getting_started {

class Triangle : public framework::IPencil {
 public:
    Triangle();
    ~Triangle();
    void Init();
    void Draw();

 private:
    gl_helper::VertexArrayObject    vao_;
    Shader  shader_;
};

}  // namespace getting_started

#endif  // SRC_PRACTICE_01_GETTING_STARTED_01_TRIANGLE_H_
