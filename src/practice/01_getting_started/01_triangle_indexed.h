// Created by SelFree on 2019/2/26.
// Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_PRACTICE_01_GETTING_STARTED_01_TRIANGLE_INDEXED_H_
#define SRC_PRACTICE_01_GETTING_STARTED_01_TRIANGLE_INDEXED_H_

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <vector>
#include "src/framework.h"
#include "src/shader.h"
#include "src/gl_helper.h"

namespace getting_started {

class TriangleIndexed : public framework::IPencil {
 public:
    TriangleIndexed();
    ~TriangleIndexed();

    void Init();
    void Draw();

 private:
    gl_helper::VertexArrayObject vao_;
    gl_helper::VertexBufferObject<int>  ebo_;

    Shader  shader_;
};

}  // namespace getting_started

#endif  // SRC_PRACTICE_01_GETTING_STARTED_01_TRIANGLE_INDEXED_H_
