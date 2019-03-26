// Created by SelFree on 2019/03/01.
// Copyright © 2019年 SelFree. All rights reserved.

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/practice/01_getting_started/04_coordinate.h"

namespace getting_started {

Coordinate::Coordinate() {
}

Coordinate::~Coordinate() {
}

void Coordinate::Init() {
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
        , GL_RGB, GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE1);
    texture2_.Load(util::GetResourceFilename("texture/awesomeface.png")
        , GL_RGBA, GL_TEXTURE1);

    vao_.Unbind();

    shader_.Compile(util::GetResourceFilename("shaders/01_getting_started/04_coordinate/vertex.vs")
                    , util::GetResourceFilename("shaders/01_getting_started/04_coordinate/fragment.fs"));
    shader_.Use();  // 设置之前必须启用
    shader_.SetInt("texture_id1", texture1_.get_texture_index() - GL_TEXTURE0);
    shader_.SetInt("texture_id2", texture2_.get_texture_index() - GL_TEXTURE0);

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f)
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);
    shader_.SetMatrix4fv("projection", glm::value_ptr(projection));

    glm::mat4 view(1.0f);
    // 注意，我们将矩阵向我们要进行移动场景的反方向移动。
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    shader_.SetMatrix4fv("view", glm::value_ptr(view));
}

void Coordinate::Draw() {
    shader_.Use();
    vao_.Bind();

    glm::mat4 model(1.0f);
    // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, static_cast<float>(glfwGetTime())
        , glm::vec3(1.0f, 1.0f, 1.0f));
    shader_.SetMatrix4fv("model", glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

}  // namespace getting_started
