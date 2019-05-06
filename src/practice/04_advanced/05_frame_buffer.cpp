// Created by SelFree on 2019/04/16.
// Copyright © 2019年 SelFree. All rights reserved.

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/common/camera.h"
#include "src/util/util.h"
#include "src/practice/04_advanced/05_frame_buffer.h"

namespace advanced {
FrameBuffer::FrameBuffer()
    : delegate_(this, &FrameBuffer::ProcessInput) {
    glGenFramebuffers(1, &fbo_);
    glGenRenderbuffers(1, &rbo_);
}

FrameBuffer::~FrameBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &fbo_);
    glDeleteRenderbuffers(1, &rbo_);
}

void FrameBuffer::Attached() {
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

void FrameBuffer::Dettached() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    framework::Instance()->RemoveProcessInputDelegate(&delegate_);

    if (framework::Instance()->get_impl() == this) {
        framework::Instance()->set_impl(nullptr);
    }
}


void FrameBuffer::Init() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    // 创建一个纹理
    glGenTextures(1, &screen_texture_);
    glBindTexture(GL_TEXTURE_2D, screen_texture_);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB
        , framework::Instance()->get_width()
        , framework::Instance()->get_height()
        , 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 将纹理附加到帧缓冲上
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0
        , GL_TEXTURE_2D, screen_texture_, 0);

    // 绑定渲染缓冲
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    // 创建深度和模板渲染缓冲
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8
        , framework::Instance()->get_width()
        , framework::Instance()->get_height());

    // 将渲染缓冲附加到帧缓冲上
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT
        , GL_RENDERBUFFER, rbo_);

    float quad_vertices[] = {
        // vertex attributes for a quad that fills the entire screen
        // in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    screen_vao_.Bind();
    gl_helper::VertexBufferObject<float> screen_vbo;
    screen_vbo.Bind();
    screen_vbo.SetData(sizeof(quad_vertices)/sizeof(float), quad_vertices);
    gl_helper::VertexAttribPointer<float>(0, 2, 4);
    gl_helper::VertexAttribPointer<float>(1, 2, 4, 2);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    screen_vao_.Unbind();


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
            << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    screen_shader_.Compile(util::GetResourceFilename("shaders/04_advanced/05_framebuffer/screen.vs")
                    , util::GetResourceFilename("shaders/04_advanced/05_framebuffer/screen.fs"));
    screen_shader_.Use();
    screen_shader_.SetInt("screenTexture", 0);

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

void FrameBuffer::Draw() {
    texture2_.Bind(GL_TEXTURE0);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glEnable(GL_DEPTH_TEST);
    // 清除帧缓冲
    glClearColor(0.1f, 0.1f, 0.1f, 2.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    screen_shader_.Use();
    screen_vao_.Bind();
    // use the color attachment texture as the texture of the quad plane
    glBindTexture(GL_TEXTURE_2D, screen_texture_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void FrameBuffer::ProcessInput() {
    framework::CameraMove::ProcessInput();

    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    object_shader_.SetVec3("view_pos"
        , glm::value_ptr(camera::Instance()->get_position()));
    object_shader_.SetVec3("spot_light.position"
        , glm::value_ptr(camera::Instance()->get_position()));
}


void FrameBuffer::Position(double xpos, double ypos) {
    framework::CameraMove::Position(xpos, ypos);
    object_shader_.Use();
    object_shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
    object_shader_.SetVec3("spot_light.direction"
        , glm::value_ptr(camera::Instance()->get_front()));
}

void FrameBuffer::Scroll(double xoffset, double yoffset) {
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
