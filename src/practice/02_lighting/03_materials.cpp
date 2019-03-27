// Created by SelFree on 2019/3/4.
// Copyright © 2019年 SelFree. All rights reserved.

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "src/util/util.h"
#include "src/common/camera.h"
#include "src/practice/02_lighting/03_materials.h"

namespace lighting {

Materials::Materials() : delegate_(this, &Materials::ProcessInput) {
}

Materials::~Materials() {
}

void Materials::Attached() {
    // 隐藏光标
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    framework::Instance()->AppendProcessInputDelegate(&delegate_);

    framework::Instance()->set_impl(this);
    camera::Instance()->Reset(glm::vec3(0.0f, 0.0f, 5.0f));

    framework::CameraMove::Reset();
}

void Materials::Dettached() {
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    framework::Instance()->RemoveProcessInputDelegate(&delegate_);

    if (framework::Instance()->get_impl() == this) {
        framework::Instance()->set_impl(nullptr);
    }
}

void Materials::Init() {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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
    gl_helper::VertexAttribPointer<float>(0, 3, 6);
    // 激活顶点属性
    glEnableVertexAttribArray(0);
    light_vao_.Unbind();

    // Object
    object_vao_.Bind();
    // 绑定缓冲区，建立缓冲区与顶点对象联系
    vbo_points.Bind();
    // 向刚才绑定的缓冲区发送数据
    vbo_points.SetData(sizeof(vertices)/sizeof(*vertices), vertices);
    gl_helper::VertexAttribPointer<float>(0, 3, 6);
    glEnableVertexAttribArray(0);
    gl_helper::VertexAttribPointer<float>(1, 3, 6, 3);
    glEnableVertexAttribArray(1);
    object_vao_.Unbind();

    light_shader_.Compile(util::GetResourceFilename("shaders/02_lighting/03_materials/light.vs")
                    , util::GetResourceFilename("shaders/02_lighting/03_materials/light.fs"));

    object_shader_.Compile(util::GetResourceFilename("shaders/02_lighting/03_materials/object.vs")
                    , util::GetResourceFilename("shaders/02_lighting/03_materials/object.fs"));

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera::Instance()->get_zoom())
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);

    light_shader_.Use();
    light_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
    light_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));

    glm::mat4 light_model(1.0f);
    glm::vec3 light_pos(1.2f, 1.0f, 2.0f);
    light_model = glm::translate(light_model, light_pos);
    light_model = glm::scale(light_model, glm::vec3(0.2f));
    light_shader_.SetMatrix4fv("model", glm::value_ptr(light_model));

    object_shader_.Use();
    object_shader_.SetVec3("light_pos", glm::value_ptr(light_pos));
    object_shader_.SetVec3("view_pos"
        , glm::value_ptr(camera::Instance()->get_position()));
    object_shader_.SetVec3("object_color", 1.0f, 0.5f, 0.3f);
    object_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
}

void Materials::Draw() {
    light_shader_.Use();

    glm::vec3 light_color(1.0f);
    light_color.x = sin(glfwGetTime() * 2.0f);
    light_color.y = sin(glfwGetTime() * 0.7f);
    light_color.z = sin(glfwGetTime() * 1.3f);
    light_shader_.SetVec3("light_color", glm::value_ptr(light_color));
    light_vao_.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    object_shader_.Use();
    // 光照属性
    glm::vec3 diffuse = light_color * glm::vec3(0.5f);
    glm::vec3 ambient = diffuse * glm::vec3(0.2f);
    object_shader_.SetVec3("light.ambient", glm::value_ptr(ambient));
    object_shader_.SetVec3("light.diffuse", glm::value_ptr(diffuse));
    object_shader_.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // 材质
    object_shader_.SetVec3("material.ambient",  1.0f, 0.5f, 0.31f);
    object_shader_.SetVec3("material.diffuse",  1.0f, 0.5f, 0.31f);
    object_shader_.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
    object_shader_.SetFloat("material.shininess", 32.0f);

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


void Materials::ProcessInput() {
    framework::CameraMove::ProcessInput();

    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    object_shader_.SetVec3("view_pos"
        , glm::value_ptr(camera::Instance()->get_position()));

    light_shader_.Use();
    light_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
}


void Materials::Position(double xpos, double ypos) {
    framework::CameraMove::Position(xpos, ypos);

    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    light_shader_.Use();
    light_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
}

void Materials::Scroll(double xoffset, double yoffset) {
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
