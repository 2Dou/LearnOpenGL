// Created by SelFree on 2019/2/26.
// Copyright © 2019年 SelFree. All rights reserved.

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "src/util/util.h"
#include "src/practice/01_getting_started/01_triangle.h"

namespace getting_started {

Triangle::Triangle() : vao_() {
}

Triangle::~Triangle() {
}

void Triangle::Init() {
    // 绑定顶点对象
    vao_.Bind();

    float posints[] = {-0.5f, -0.5f, 0.0f,  // 顶点位置
                    0.5f, -0.5f, 0.0f,
                    0.0f,  0.5f, 0.0f,
                };
    float colors[] = {
                    1.0f, 0.0f, 0.0f,  // 顶点颜色
                    0.0f, 0.0f, 1.0f,
                    0.0f, 1.0f, 0.0f
                };

    // 创建顶点缓冲对象用于发送三角形顶点
    gl_helper::VertexBufferObject<float> vbo_points;
    // 绑定缓冲区，建立缓冲区与顶点对象联系
    vbo_points.Bind();
    // 向刚才绑定的缓冲区发送数据
    vbo_points.SetData(9, posints);

    // 指定顶点属性
    gl_helper::VertexAttribPointer<float>(0, 3, 3);
    // 激活顶点属性
    glEnableVertexAttribArray(0);

    // 创建顶点缓冲对象用于发送颜色
    gl_helper::VertexBufferObject<float> vbo_colors;
    // 向刚才缓冲区发送数据（SetData里面会调用Bind）
    vbo_colors.SetData(9, colors);

    // 指定顶点属性
    gl_helper::VertexAttribPointer<float>(1, 3, 3);
    // 激活顶点属性
    glEnableVertexAttribArray(1);

    vao_.Unbind();

    shader_.Compile(util::GetResourceFilename("shaders/01_getting_started/01_triangle/vertex.vs")
                    , util::GetResourceFilename("shaders/01_getting_started/01_triangle/fragment.fs"));

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // 线条模式
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // 填充模式，默认
}

void Triangle::Draw() {
    vao_.Bind();
    shader_.Use();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

}  // namespace getting_started
