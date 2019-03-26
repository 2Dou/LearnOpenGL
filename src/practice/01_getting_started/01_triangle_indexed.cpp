// Created by SelFree on 2019/2/26.
// Copyright © 2019年 SelFree. All rights reserved.

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "src/util/util.h"
#include "src/practice/01_getting_started/01_triangle_indexed.h"

namespace getting_started {

TriangleIndexed::TriangleIndexed()
: ebo_(GL_ELEMENT_ARRAY_BUFFER) {
}

TriangleIndexed::~TriangleIndexed() {
}

void TriangleIndexed::Init() {
    std::vector<float> points{
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f,  // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    std::vector<int> indexs{
        0, 1, 3,  // 第一个三角形
        1, 2, 3   // 第二个三角形
    };
    std::vector<float> colors{
        1.0f, 0.0f, 0.0f,  // 顶点颜色
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f
    };

    // 绑定顶点对象
    vao_.Bind();

    gl_helper::VertexBufferObject<float> vbo_points;
    // 向缓冲区发送顶点数据
    vbo_points.SetData(points.size(), points.data());
    // 指定顶点属性
    gl_helper::VertexAttribPointer<float>(0, 3, 3);
    glEnableVertexAttribArray(0);

    gl_helper::VertexBufferObject<float> vbo_colors;
    // 向缓冲区发送颜色数据
    vbo_colors.SetData(colors.size(), colors.data());
    // 指定顶点属性
    gl_helper::VertexAttribPointer<float>(1, 3, 3);
    glEnableVertexAttribArray(1);

    // 向缓冲区发送索引数据
    ebo_.SetData(indexs.size(), indexs.data());

    vao_.Unbind();

    glBindVertexArray(0);

    shader_.Compile(util::GetResourceFilename("shaders/01_getting_started/01_triangle/vertex.vs")
                    , util::GetResourceFilename("shaders/01_getting_started/01_triangle/fragment.fs"));
}

void TriangleIndexed::Draw() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    shader_.Use();
    vao_.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

}  // namespace getting_started
