// Created by SelFree on 2019/3/4.
// Copyright © 2019年 SelFree. All rights reserved.

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "src/util/util.h"
#include "src/common/camera.h"
#include "src/practice/02_lighting/05_spotlight.h"

namespace lighting {

Spotlight::Spotlight()
    : delegate_(this, &Spotlight::ProcessInput) {
}

Spotlight::~Spotlight() {
}

void Spotlight::Attached() {
    // 隐藏光标
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    framework::Instance()->AppendProcessInputDelegate(&delegate_);

    framework::Instance()->set_impl(this);
    camera::Instance()->Reset(glm::vec3(0.0f, 0.0f, 5.0f));

    framework::CameraMove::Reset();
}

void Spotlight::Dettached() {
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    framework::Instance()->RemoveProcessInputDelegate(&delegate_);

    if (framework::Instance()->get_impl() == this) {
        framework::Instance()->set_impl(nullptr);
    }
}

void Spotlight::Init() {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
    gl_helper::VertexAttribPointer<float>(0, 3, 8);
    // 激活顶点属性
    glEnableVertexAttribArray(0);
    light_vao_.Unbind();

    // Object
    object_vao_.Bind();
    // 绑定缓冲区，建立缓冲区与顶点对象联系
    vbo_points.Bind();
    // 向刚才绑定的缓冲区发送数据
    vbo_points.SetData(sizeof(vertices)/sizeof(*vertices), vertices);
    gl_helper::VertexAttribPointer<float>(0, 3, 8);
    glEnableVertexAttribArray(0);
    gl_helper::VertexAttribPointer<float>(1, 3, 8, 3);
    glEnableVertexAttribArray(1);
    gl_helper::VertexAttribPointer<float>(2, 2, 8, 6);
    glEnableVertexAttribArray(2);

    // 纹理
    texture1_.Load(util::GetResourceFilename("texture/container2.png")
        , GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE1);
    texture2_.Load(util::GetResourceFilename("texture/container2_specular.png")
        , GL_TEXTURE1);

    object_vao_.Unbind();

    object_shader_.Compile(util::GetResourceFilename("shaders/02_lighting/05_spotlight/object.vs")
                    , util::GetResourceFilename("shaders/02_lighting/05_spotlight/object.fs"));

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera::Instance()->get_zoom())
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);

    object_shader_.Use();
    object_shader_.SetVec3("view_pos"
        , glm::value_ptr(camera::Instance()->get_position()));
    object_shader_.SetVec3("object_color", 1.0f, 0.5f, 0.3f);
    object_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));

    // 光照属性
    glm::vec3 light_color(1.0f);
    object_shader_.SetVec3("light.ambient", glm::value_ptr(light_color * 0.2f));
    object_shader_.SetVec3("light.diffuse", glm::value_ptr(light_color));
    object_shader_.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

    object_shader_.SetFloat("light.constant", 1.0f);
    object_shader_.SetFloat("light.linear", 0.09f);
    object_shader_.SetFloat("light.quadratic", 0.032f);
    object_shader_.SetFloat("light.cut_off"
        , glm::cos(glm::radians(12.5f)));
    object_shader_.SetFloat("light.outer_cut_off"
        , glm::cos(glm::radians(17.5f)));

    object_shader_.SetInt("material.diffuse"
        , texture1_.get_texture_index() - GL_TEXTURE0);
    object_shader_.SetInt("material.specular"
        , texture2_.get_texture_index() - GL_TEXTURE0);
    object_shader_.SetFloat("material.shininess", 32.0f);
}

void Spotlight::Draw() {
    texture1_.Bind(GL_TEXTURE0);
    texture2_.Bind(GL_TEXTURE1);

    object_shader_.Use();
    object_shader_.SetVec3("view_pos"
        , glm::value_ptr(camera::Instance()->get_position()));

    object_shader_.SetVec3("light.position", glm::value_ptr(camera::Instance()->get_position()));
    object_shader_.SetVec3("light.direction", glm::value_ptr(camera::Instance()->get_front()));

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


void Spotlight::ProcessInput() {
    framework::CameraMove::ProcessInput();

    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    object_shader_.SetVec3("view_pos"
        , glm::value_ptr(camera::Instance()->get_position()));
}


void Spotlight::Position(double xpos, double ypos) {
    framework::CameraMove::Position(xpos, ypos);
    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
}

void Spotlight::Scroll(double xoffset, double yoffset) {
    framework::CameraMove::Scroll(xoffset, yoffset);

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera::Instance()->get_zoom())
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);

    object_shader_.Use();
    object_shader_.SetMatrix4fv("projection"
        , glm::value_ptr(projection));
}

}  // namespace lighting
