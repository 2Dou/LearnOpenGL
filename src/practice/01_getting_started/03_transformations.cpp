// Created by SelFree on 2019/03/01.
// Copyright © 2019年 SelFree. All rights reserved.

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/practice/01_getting_started/03_transformations.h"

namespace getting_started {

Transformation::Transformation() {
}

Transformation::~Transformation() {
}

void Transformation::Init() {
    vao_.Bind();

    gl_helper::VertexBufferObject<float> vbo;

    float data[] = {
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f
    };
    vbo.SetData(sizeof(data)/sizeof(float), data);

    gl_helper::VertexAttribPointer<float>(0, 3, 5);
    gl_helper::VertexAttribPointer<float>(1, 2, 5, 3);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    texture1_.Load(util::GetResourceFilename("texture/container.jpg")
        , GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE1);
    texture2_.Load(util::GetResourceFilename("texture/awesomeface.png")
        , GL_TEXTURE1);

    shader_.Compile(util::GetResourceFilename("shaders/01_getting_started/03_transformations/vertex.vs")
                    , util::GetResourceFilename("shaders/01_getting_started/03_transformations/fragment.fs"));
    shader_.Use();  // 设置之前必须启用
    shader_.SetInt("texture_id1", texture1_.get_texture_index() - GL_TEXTURE0);
    shader_.SetInt("texture_id2", texture2_.get_texture_index() - GL_TEXTURE0);

    vao_.Unbind();
}

void Transformation::Draw() {
    shader_.Use();
    vao_.Bind();

    texture1_.Bind(GL_TEXTURE0);
    texture2_.Bind(GL_TEXTURE1);

    glm::mat4 trans(1.0f);
    trans = glm::rotate(trans
        , static_cast<float>(glfwGetTime())  // 旋转角度
        , glm::vec3(0.0f, 0.0f, 1.0f));  // 对哪些坐标生效
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    shader_.SetMatrix4fv("transform", glm::value_ptr(trans));

    // 画第一个
    glDrawArrays(GL_TRIANGLES, 0, 6);

    trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
    float scale = abs(sin(static_cast<float>(glfwGetTime())));
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));
    shader_.SetMatrix4fv("transform", glm::value_ptr(trans));

    // 画第二个
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

}  // namespace getting_started
