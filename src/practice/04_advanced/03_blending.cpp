// Created by SelFree on 2019/03/29.
// Copyright © 2019年 SelFree. All rights reserved.

#include <glad/glad.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/common/camera.h"
#include "src/util/util.h"
#include "src/practice/04_advanced/03_blending.h"

namespace advanced {
Blending::Blending()
    : delegate_(this, &Blending::ProcessInput) {
}

Blending::~Blending() {
}

void Blending::Attached() {
    // 启用面剔除
    glEnable(GL_CULL_FACE);

    // 启用混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 隐藏光标
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    framework::Instance()->AppendProcessInputDelegate(&delegate_);

    framework::Instance()->set_impl(this);
    camera::Instance()->Reset(glm::vec3(0.0f, 0.0f, 5.0f));

    framework::CameraMove::Reset();
}

void Blending::Dettached() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    framework::Instance()->RemoveProcessInputDelegate(&delegate_);

    if (framework::Instance()->get_impl() == this) {
        framework::Instance()->set_impl(nullptr);
    }
}


void Blending::Init() {
    object_vao_.Bind();
    gl_helper::VertexBufferObject<float> vbo;
    vbo.Bind();

    float vertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left
    };
    vbo.SetData(sizeof(vertices)/sizeof(float), vertices);

    gl_helper::VertexAttribPointer<float>(0, 3, 5);
    gl_helper::VertexAttribPointer<float>(1, 2, 5, 3);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    object_vao_.Unbind();

    float floor_vertices[] = {
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,

        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };
    floor_vao_.Bind();
    gl_helper::VertexBufferObject<float> floor_vbo;
    floor_vbo.Bind();
    floor_vbo.SetData(sizeof(floor_vertices)/sizeof(float), floor_vertices);
    gl_helper::VertexAttribPointer<float>(0, 3, 5);
    gl_helper::VertexAttribPointer<float>(1, 2, 5, 3);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    floor_vao_.Unbind();

    float grass_vertices[] = {
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };
    grass_vao_.Bind();
    gl_helper::VertexBufferObject<float> grass_vbo;
    grass_vbo.Bind();
    grass_vbo.SetData(sizeof(grass_vertices)/sizeof(float), grass_vertices);
    gl_helper::VertexAttribPointer<float>(0, 3, 5);
    gl_helper::VertexAttribPointer<float>(1, 2, 5, 3);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    grass_vao_.Unbind();

    texture1_.Load(util::GetResourceFilename("texture/container.jpg"));
    texture2_.Load(util::GetResourceFilename("texture/metal.png"));
    texture3_.Load(util::GetResourceFilename("texture/grass.png"));

    object_shader_.Compile(util::GetResourceFilename("shaders/04_advanced/03_blending/object.vs")
                    , util::GetResourceFilename("shaders/04_advanced/03_blending/object.fs"));

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f)
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);
    object_shader_.Use();
    object_shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    object_shader_.SetInt("texture1", GL_TEXTURE0 - GL_TEXTURE0);
}

void Blending::Draw() {
    texture2_.Bind(GL_TEXTURE0);

    // 画地板
    floor_vao_.Bind();
    object_shader_.Use();
    object_shader_.SetMatrix4fv("model", glm::value_ptr(glm::mat4(1.0f)));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 画第一个箱子
    object_vao_.Bind();
    texture1_.Bind(GL_TEXTURE0);
    object_shader_.SetInt("texture1", GL_TEXTURE0 - GL_TEXTURE0);

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    object_shader_.SetMatrix4fv("model", glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 画第二个箱子
    glEnable(GL_DEPTH_TEST);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    object_shader_.SetMatrix4fv("model", glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    std::vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));
    vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));

    grass_vao_.Bind();
    texture3_.Bind(GL_TEXTURE0);
    for (size_t i = 0; i < vegetation.size(); i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, vegetation[i]);
        model = glm::rotate(model, 3.14159f, glm::vec3(0.0f, 0.0f, 1.0f));
        object_shader_.SetMatrix4fv("model", glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


void Blending::ProcessInput() {
    framework::CameraMove::ProcessInput();

    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    object_shader_.SetVec3("view_pos"
        , glm::value_ptr(camera::Instance()->get_position()));
    object_shader_.SetVec3("spot_light.position"
        , glm::value_ptr(camera::Instance()->get_position()));
}


void Blending::Position(double xpos, double ypos) {
    framework::CameraMove::Position(xpos, ypos);
    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    object_shader_.SetVec3("spot_light.direction"
        , glm::value_ptr(camera::Instance()->get_front()));
}

void Blending::Scroll(double xoffset, double yoffset) {
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
}  // namespace advanced
