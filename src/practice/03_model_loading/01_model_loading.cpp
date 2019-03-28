// Created by SelFree on 2019/03/26.
// Copyright © 2019年 SelFree. All rights reserved.

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/common/camera.h"
#include "src/util/util.h"
#include "src/practice/03_model_loading/01_model_loading.h"

namespace model_loading {

static glm::vec3 g_light_positions[] = {
    glm::vec3(1.0f, 2.0f, 0.5f),
};

ModelLoading::ModelLoading()
    : delegate_(this, &ModelLoading::ProcessInput) {
}

ModelLoading::~ModelLoading() {
}

void ModelLoading::Attached() {
    // 隐藏光标
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    framework::Instance()->AppendProcessInputDelegate(&delegate_);

    framework::Instance()->set_impl(this);
    camera::Instance()->Reset(glm::vec3(0.0f, 0.0f, 5.0f));

    framework::CameraMove::Reset();
}

void ModelLoading::Dettached() {
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    framework::Instance()->RemoveProcessInputDelegate(&delegate_);

    if (framework::Instance()->get_impl() == this) {
        framework::Instance()->set_impl(nullptr);
    }
}

void ModelLoading::Init() {
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

    model_.Load(util::GetResourceFilename("models/nanosuit/nanosuit.obj"));

    light_shader_.Compile(util::GetResourceFilename("shaders/03_model_loading/01_model_loading/light.vs")
                    , util::GetResourceFilename("shaders/03_model_loading/01_model_loading/light.fs"));
    object_shader_.Compile(util::GetResourceFilename("shaders/03_model_loading/01_model_loading/object.vs")
                    , util::GetResourceFilename("shaders/03_model_loading/01_model_loading/object.fs"));

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera::Instance()->get_zoom())
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);

    light_shader_.Use();
    light_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
    light_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    light_shader_.SetVec3("light_color", 1.0f, 1.0f, 1.0f);
    glm::mat4 light_model(1.0f);
    light_model = glm::translate(light_model, g_light_positions[0]);
    light_model = glm::scale(light_model, glm::vec3(0.2f));
    light_shader_.SetMatrix4fv("model", glm::value_ptr(light_model));

    object_shader_.Use();
    object_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));

    glm::mat4 model = glm::mat4(1.0f);
    // translate it down so it's at the center of the scene
    model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
    // it's a bit too big for our scene, so scale it down
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

    object_shader_.SetMatrix4fv("model", glm::value_ptr(model));

    object_shader_.SetFloat("shininess", 32.0f);

    // 定向光
    object_shader_.SetVec3("dir_light.direction", -0.2f, -1.0f, -0.3f);
    object_shader_.SetVec3("dir_light.ambient", 0.1f, 0.1f, 0.1f);
    object_shader_.SetVec3("dir_light.diffuse", 0.4f, 0.4f, 0.4f);
    object_shader_.SetVec3("dir_light.specular", 0.5f, 0.5f, 0.5f);

    // 聚光灯
    // 光照属性
    object_shader_.SetVec3("spot_light.ambient", 0.05f, 0.05f, 0.05f);
    object_shader_.SetVec3("spot_light.diffuse", 0.8f, 0.8f, 0.8f);
    object_shader_.SetVec3("spot_light.specular", 1.0f, 1.0f, 1.0f);
    // 衰减系数
    object_shader_.SetFloat("spot_light.constant", 1.0f);
    object_shader_.SetFloat("spot_light.linear", 0.09f);
    object_shader_.SetFloat("spot_light.quadratic", 0.032f);
    object_shader_.SetFloat("spot_light.cut_off"
        , glm::cos(glm::radians(12.5f)));
    object_shader_.SetFloat("spot_light.outer_cut_off"
        , glm::cos(glm::radians(15.0f)));

    // 点光
    object_shader_.SetVec3("point_light.position"
        , glm::value_ptr(g_light_positions[0]));
    object_shader_.SetVec3("spot_light.direction"
        , glm::value_ptr(camera::Instance()->get_front()));
    object_shader_.SetVec3("point_light.ambient", 0.1f, 0.1f, 0.1f);
    object_shader_.SetVec3("point_light.diffuse", 0.8f, 0.8f, 0.8f);
    object_shader_.SetVec3("point_light.specular", 1.0f, 1.0f, 1.0f);
    object_shader_.SetFloat("point_light.constant", 1.0f);
    object_shader_.SetFloat("point_light.linear", 0.09f);
    object_shader_.SetFloat("point_light.quadratic", 0.032f);
}

void ModelLoading::Draw() {
    // render the loaded model
    object_shader_.Use();
    model_.Draw(object_shader_);

    light_vao_.Bind();
    light_shader_.Use();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void ModelLoading::ProcessInput() {
    framework::CameraMove::ProcessInput();

    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    object_shader_.SetVec3("view_pos"
        , glm::value_ptr(camera::Instance()->get_position()));
    object_shader_.SetVec3("spot_light.position"
        , glm::value_ptr(camera::Instance()->get_position()));

    light_shader_.Use();
    light_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    light_shader_.SetVec3("view_pos"
        , glm::value_ptr(camera::Instance()->get_position()));
}


void ModelLoading::Position(double xpos, double ypos) {
    framework::CameraMove::Position(xpos, ypos);
    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    object_shader_.SetVec3("spot_light.direction"
        , glm::value_ptr(camera::Instance()->get_front()));

    light_shader_.Use();
    light_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
}

void ModelLoading::Scroll(double xoffset, double yoffset) {
    framework::CameraMove::Scroll(xoffset, yoffset);

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera::Instance()->get_zoom())
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);

    object_shader_.Use();
    object_shader_.SetMatrix4fv("projection"
        , glm::value_ptr(projection));

    light_shader_.Use();
    light_shader_.SetMatrix4fv("projection"
        , glm::value_ptr(projection));
}

}  // namespace model_loading
