// Created by SelFree on 2019/05/05.
// Copyright © 2019年 SelFree. All rights reserved.

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/common/camera.h"
#include "src/util/util.h"
#include "src/practice/04_advanced/06_cubemaps.h"

namespace advanced {
Cubemaps::Cubemaps()
    : delegate_(this, &Cubemaps::ProcessInput) {
}

Cubemaps::~Cubemaps() {
}

void Cubemaps::Attached() {
    // 启用混合
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 隐藏光标
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    framework::Instance()->AppendProcessInputDelegate(&delegate_);

    framework::Instance()->set_impl(this);
    camera::Instance()->Reset(glm::vec3(0.0f, 0.0f, 5.0f));

    framework::CameraMove::Reset();
}

void Cubemaps::Dettached() {
    glDisable(GL_CULL_FACE);
    // glDisable(GL_BLEND);

    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    framework::Instance()->RemoveProcessInputDelegate(&delegate_);

    if (framework::Instance()->get_impl() == this) {
        framework::Instance()->set_impl(nullptr);
    }
}


void Cubemaps::Init() {
    float sky_vertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, 1.0f,
         1.0f,  1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, 1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // 发送顶点数据
    sky_vao_.Bind();
    gl_helper::VertexBufferObject<float> vbo;
    vbo.Bind();
    vbo.SetData(sizeof(sky_vertices)/sizeof(float), sky_vertices);
    gl_helper::VertexAttribPointer<float>(0, 3, 3);
    glEnableVertexAttribArray(0);
    sky_vao_.Unbind();

    sky_shader_.Compile(util::GetResourceFilename("shaders/04_advanced/06_cubemaps/sky.vs")
                    , util::GetResourceFilename("shaders/04_advanced/06_cubemaps/sky.fs"));
    sky_shader_.Use();
    sky_shader_.SetInt("skybox", GL_TEXTURE0 - GL_TEXTURE0);
    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f)
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);
    sky_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));

    auto view = glm::mat4(glm::mat3(camera::Instance()->GetViewMatrix()));
    sky_shader_.SetMatrix4fv("view", glm::value_ptr(view));

    // 纹理
    std::vector<std::string> sky_files{
        util::GetResourceFilename("texture/skybox/right.tga"),
        util::GetResourceFilename("texture/skybox/left.tga"),
        util::GetResourceFilename("texture/skybox/top.tga"),
        util::GetResourceFilename("texture/skybox/bottom.tga"),
        util::GetResourceFilename("texture/skybox/front.tga"),
        util::GetResourceFilename("texture/skybox/back.tga")
    };
    sky_texture_.Load(sky_files);

    //////
    float box_vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // Bottom-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top-right
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // top-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top-left
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f // bottom-left
    };

    // 数据
    box_vao_.Bind();
    gl_helper::VertexBufferObject<float> box_vbo;
    box_vbo.Bind();
    box_vbo.SetData(sizeof(box_vertices)/sizeof(float), box_vertices);
    gl_helper::VertexAttribPointer<float>(0, 3, 8);
    gl_helper::VertexAttribPointer<float>(1, 2, 8, 3);
    gl_helper::VertexAttribPointer<float>(2, 3, 8, 5);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    box_vao_.Unbind();

    // shader
    box_shader_.Compile(util::GetResourceFilename("shaders/04_advanced/06_cubemaps/box.vs")
                    , util::GetResourceFilename("shaders/04_advanced/06_cubemaps/box.fs"));
    box_shader_.Use();
    box_shader_.SetInt("texture1", GL_TEXTURE0 - GL_TEXTURE0);
    box_shader_.SetInt("skybox", GL_TEXTURE1 - GL_TEXTURE0);
    box_shader_.SetVec3("camera_position"
        , glm::value_ptr(camera::Instance()->get_position()));
    box_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
    box_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    box_shader_.SetMatrix4fv("model", glm::value_ptr(glm::mat4(1.0f)));

    // 纹理
    box_texture_.Load(util::GetResourceFilename("texture/container.jpg"));

    // 忍者
    model_.Load(util::GetResourceFilename("models/nanosuit/nanosuit.obj"));

    model_shader_.Compile(util::GetResourceFilename("shaders/04_advanced/06_cubemaps/model.vs")
                    , util::GetResourceFilename("shaders/04_advanced/06_cubemaps/model.fs"));
    model_shader_.Use();
    model_shader_.SetVec3("camera_position"
        , glm::value_ptr(camera::Instance()->get_position()));
    model_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
    model_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, -0.5f, 0.5f));
    model = glm::scale(model, glm::vec3(0.1, 0.1, 0.1));
    model_shader_.SetMatrix4fv("model", glm::value_ptr(model));
    model_shader_.SetInt("skybox", GL_TEXTURE4 - GL_TEXTURE0);
}

void Cubemaps::Draw() {
    box_vao_.Bind();
    box_shader_.Use();
    box_texture_.Bind(GL_TEXTURE0);
    sky_texture_.Bind(GL_TEXTURE1);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 启用面剔除
    glEnable(GL_CULL_FACE);
    model_shader_.Use();
    sky_texture_.Bind(GL_TEXTURE4);
    model_.Draw(model_shader_);
    glDisable(GL_CULL_FACE);

    glDepthFunc(GL_LEQUAL);
    sky_vao_.Bind();
    sky_shader_.Use();
    sky_texture_.Bind(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);  // set depth function back to default

    sky_vao_.Unbind();
}


void Cubemaps::ProcessInput() {
    framework::CameraMove::ProcessInput();

    sky_shader_.Use();
    auto view = glm::mat4(glm::mat3(camera::Instance()->GetViewMatrix()));
    sky_shader_.SetMatrix4fv("view", glm::value_ptr(view));

    box_shader_.Use();
    box_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    box_shader_.SetVec3("camera_position"
        , glm::value_ptr(camera::Instance()->get_position()));

    model_shader_.Use();
    model_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    model_shader_.SetVec3("camera_position"
        , glm::value_ptr(camera::Instance()->get_position()));
}


void Cubemaps::Position(double xpos, double ypos) {
    framework::CameraMove::Position(xpos, ypos);
    sky_shader_.Use();
    auto view = glm::mat4(glm::mat3(camera::Instance()->GetViewMatrix()));
    sky_shader_.SetMatrix4fv("view", glm::value_ptr(view));

    box_shader_.Use();
    box_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));

    model_shader_.Use();
    model_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
}

void Cubemaps::Scroll(double xoffset, double yoffset) {
    framework::CameraMove::Scroll(xoffset, yoffset);

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera::Instance()->get_zoom())
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);

    sky_shader_.Use();
    sky_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));

    box_shader_.Use();
    box_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));

    model_shader_.Use();
    model_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
}
}  // namespace advanced
