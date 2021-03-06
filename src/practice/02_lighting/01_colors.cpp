// Created by SelFree on 2019/3/4.
// Copyright © 2019年 SelFree. All rights reserved.

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "src/util/util.h"
#include "src/common/camera.h"
#include "src/practice/02_lighting/01_colors.h"

namespace lighting {

Colors::Colors() : delegate_(this, &Colors::ProcessInput) {
}

Colors::~Colors() {
}

void Colors::Attached() {
    // 隐藏光标
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    framework::Instance()->AppendProcessInputDelegate(&delegate_);

    framework::Instance()->set_impl(this);
    camera::Instance()->Reset(glm::vec3(0.0f, 0.0f, 5.0f));

    framework::CameraMove::Reset();
}

void Colors::Dettached() {
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    framework::Instance()->RemoveProcessInputDelegate(&delegate_);

    if (framework::Instance()->get_impl() == this) {
        framework::Instance()->set_impl(nullptr);
    }
}

void Colors::Init() {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    // 绑定顶点对象
    light_vao_.Bind();
    // 创建顶点缓冲对象用于发送三角形顶点
    gl_helper::VertexBufferObject<float> vbo_points;
    // 绑定缓冲区，建立缓冲区与顶点对象联系
    vbo_points.Bind();
    // 向刚才绑定的缓冲区发送数据
    vbo_points.SetData(sizeof(vertices)/sizeof(*vertices), vertices);
    // 指定顶点属性
    gl_helper::VertexAttribPointer<float>(0, 3, 3);
    // 激活顶点属性
    glEnableVertexAttribArray(0);
    light_vao_.Unbind();

    // Object
    object_vao_.Bind();
    // 绑定缓冲区，建立缓冲区与顶点对象联系
    vbo_points.Bind();
    // 向刚才绑定的缓冲区发送数据
    vbo_points.SetData(sizeof(vertices)/sizeof(*vertices), vertices);
    // 指定顶点属性
    gl_helper::VertexAttribPointer<float>(0, 3, 3);
    // 激活顶点属性
    glEnableVertexAttribArray(0);
    object_vao_.Unbind();

    light_shader_.Compile(util::GetResourceFilename("shaders/02_lighting/01_colors/light.vs")
                    , util::GetResourceFilename("shaders/02_lighting/01_colors/light.fs"));

    object_shader_.Compile(util::GetResourceFilename("shaders/02_lighting/01_colors/object.vs")
                    , util::GetResourceFilename("shaders/02_lighting/01_colors/object.fs"));

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera::Instance()->get_zoom())
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);

    light_shader_.Use();
    light_shader_.SetVec3("light_color", 1.0f, 1.0f, 1.0f);
    light_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
    light_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));

    glm::mat4 light_model(1.0f);
    glm::vec3 light_pos(1.2f, 1.0f, 2.0f);
    light_model = glm::translate(light_model, light_pos);
    light_model = glm::scale(light_model, glm::vec3(0.2f));
    light_shader_.SetMatrix4fv("model", glm::value_ptr(light_model));

    object_shader_.Use();
    object_shader_.SetVec3("light_color", 1.0f, 1.0f, 1.0f);
    object_shader_.SetVec3("object_color", 1.0f, 0.5f, 0.3f);
    object_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
}

void Colors::Draw() {
    light_shader_.Use();
    light_vao_.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);


    object_shader_.Use();
    object_vao_.Bind();
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
        object_shader_.SetMatrix4fv("model", glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


void Colors::ProcessInput() {
    framework::CameraMove::ProcessInput();

    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    light_shader_.Use();
    light_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
}


void Colors::Position(double xpos, double ypos) {
    framework::CameraMove::Position(xpos, ypos);

    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    light_shader_.Use();
    light_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
}

void Colors::Scroll(double xoffset, double yoffset) {
    framework::CameraMove::Scroll(xoffset, yoffset);

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera::Instance()->get_zoom())
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);
    object_shader_.Use();
    object_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
    light_shader_.Use();
    light_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
}

}  // namespace lighting
