// Created by SelFree on 2019/03/01.
// Copyright © 2019年 SelFree. All rights reserved.

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/common/camera.h"
#include "src/practice/01_getting_started/05_camera.h"

namespace getting_started {

Camera::Camera() : delegate_(this, &Camera::ProcessInput) {
}

Camera::~Camera() {
}

void Camera::Attached() {
    first_mouse_move_ = true;

    // 隐藏光标
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    framework::Instance()->AppendProcessInputDelegate(&delegate_);

    framework::Instance()->set_impl(this);
    camera::Instance()->Reset();
}

void Camera::Dettached() {
    glfwSetInputMode(framework::Instance()->get_window()
        , GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    framework::Instance()->RemoveProcessInputDelegate(&delegate_);

    if (framework::Instance()->get_impl() == this) {
        framework::Instance()->set_impl(nullptr);
    }
}

void Camera::Init() {
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

    shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));

    glEnable(GL_DEPTH_TEST);  // 开启景深
}

void Camera::Draw() {
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

void Camera::ProcessInput() {
    auto *window = framework::Instance()->get_window();
    uint32_t flag = 0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        flag |= camera::kMoveForward;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        flag |= camera::kMoveBackward;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        flag |= camera::kMoveLeft;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        flag |= camera::kMoveRight;
    }

    camera::Instance()->ProcessMovement(flag
        , framework::Instance()->get_frame_delta());

    shader_.Use();
    shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
}


void Camera::Position(double xpos, double ypos) {
    static double last_x, last_y, last_time;
    if (first_mouse_move_) {
        last_x = xpos, last_y = ypos;
        last_time = glfwGetTime();
        first_mouse_move_ = false;
        return;
    }

    float xoffset = static_cast<double>(xpos - last_x);
    float yoffset = static_cast<double>(last_y - ypos);
    last_x = xpos, last_y = ypos;

    camera::Instance()->ProcessMouseMovement(xoffset, yoffset);
    shader_.Use();
    shader_.SetMatrix4fv("view"
        , glm::value_ptr(camera::Instance()->GetViewMatrix()));
}

void Camera::Scroll(double xoffset, double yoffset) {
    camera::Instance()->ProcessMouseScroll(yoffset);

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(camera::Instance()->get_zoom())
        , static_cast<float>(framework::Instance()->get_width())
            / static_cast<float>(framework::Instance()->get_height())
        , 0.1f, 100.0f);
    shader_.Use();
    shader_.SetMatrix4fv("projection", glm::value_ptr(projection));
}

}  // namespace getting_started
