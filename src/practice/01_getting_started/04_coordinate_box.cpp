// Created by SelFree on 2019/03/01.
// Copyright © 2019年 SelFree. All rights reserved.

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/practice/01_getting_started/04_coordinate_box.h"

namespace getting_started {

CoordinateBox::CoordinateBox() {
}

CoordinateBox::~CoordinateBox() {
}

void CoordinateBox::Init() {
    vao_.Bind();

    gl_helper::VertexBufferObject<float> vbo;

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    vbo.SetData(sizeof(vertices)/sizeof(float), vertices);

    gl_helper::VertexAttribPointer<float>(0, 3, 5);
    gl_helper::VertexAttribPointer<float>(1, 2, 5, 3);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    texture1_.Load(util::GetResourceFilename("texture/container.jpg"));
    texture2_.Load(util::GetResourceFilename("texture/awesomeface.png"));

    vao_.Unbind();

    shader_.Compile(util::GetResourceFilename("shaders/01_getting_started/04_coordinate/vertex.vs")
                    , util::GetResourceFilename("shaders/01_getting_started/04_coordinate/fragment.fs"));
    shader_.Use();  // 设置之前必须启用
    shader_.SetInt("texture_id1", GL_TEXTURE0 - GL_TEXTURE0);
    shader_.SetInt("texture_id2", GL_TEXTURE1 - GL_TEXTURE0);

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

    glEnable(GL_DEPTH_TEST);  // 开启景深
}

void CoordinateBox::Draw() {
    shader_.Use();
    vao_.Bind();

    texture1_.Bind(GL_TEXTURE0);
    texture2_.Bind(GL_TEXTURE1);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    for (int i = 0; i < sizeof(cubePositions)/sizeof(*cubePositions); i++) {
        glm::mat4 model(1.0f);
        model = glm::translate(model, cubePositions[i]);
        model = glm::rotate(model, glm::radians(i*20.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        if (i % 3 == 0) {
            model = glm::rotate(model, static_cast<float>(glfwGetTime())
                , glm::vec3(1.0f, 1.0f, 1.0f));
        }
        shader_.SetMatrix4fv("model", glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

}  // namespace getting_started
