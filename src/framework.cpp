//  Created by SelFree on 2019/2/26.
//  Copyright © 2019年 SelFree. All rights reserved.

#include <glad/glad.h>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#include <windows.h>
#elif !defined(__unix)
#define __unix
#endif

#ifdef __unix
#include <unistd.h>
#endif

#include <iostream>

#include "src/framework.h"
#include "src/common/camera.h"

namespace framework {

/* sleep in millisecond */
static inline void isleep(uint32_t millisecond);

void framebufferSizeCallback_(GLFWwindow* window, int width, int height);
void mouseCallback_(GLFWwindow* window, double xpos, double ypos);
void scrollCallback_(GLFWwindow* window, double xoffset, double yoffset);

Framework* Instance() {
    return Framework::Instance();
}

Framework::Framework(int frames_per_second)
    : window_(nullptr) {
    SetFramesPerSecond(frames_per_second);
}

Framework::~Framework() {
}

GLFWwindow *Framework::Init(const std::string name
                            , int width /* = DEFAULT_SCREEN_WIDTH */
                            , int height /* = DEFAULT_SCREEN_HEIGHT */) {
    this->width_ = width;
    this->height_ = height;

    if (!this->init(name)) {
        return nullptr;
    }
    return this->window_;
}

void Framework::Start() {
    this->startLoop();
}

void Framework::Stop() {
    this->stopLoop();
}

void Framework::SetFramesPerSecond(int count) {
    this->frame_interval_
        = static_cast<uint32_t>(1000.0f / static_cast<float>(count));
}

GLFWwindow* Framework::get_window() const {
    return this->window_;
}

int Framework::get_width() const {
    int width;
    glfwGetWindowSize(this->window_, &width, NULL);
    return width;
}

int Framework::get_height() const {
    int height;
    glfwGetWindowSize(this->window_, NULL, &height);
    return height;
}

float Framework::get_frame_delta() const {
    return frame_delta_;
}

// 添加画笔
void Framework::set_pencil(IPencil *pencil) {
    if (pencil_ == pencil) {
        return;
    }

    if (pencil_) {
        pencil_->Dettached();
    }
    pencil_ = pencil;
    pencil_->Attached();
}

void Framework::AppendProcessInputDelegate(IProcessInputDelegate *delegate) {
    process_input_delegate_list_.push_back(delegate);
}

void Framework::RemoveProcessInputDelegate(IProcessInputDelegate *delegate) {
    for (auto it = process_input_delegate_list_.begin();
        it != process_input_delegate_list_.end();
        it++) {
        if (*it == delegate) {
            process_input_delegate_list_.erase(it);
            break;
        }
    }
}

bool Framework::init(const std::string &name) {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    // uncomment this statement to fix compilation on OS X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(this->width_, this->height_
        , name.c_str(), NULL, NULL);
    if (window == nullptr) {
        std::cout << "failed to create glfw window." << std::endl;
        glfwTerminate();
        return false;
    }

    // do this before loading glad
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback_);

    glfwSetCursorPosCallback(window, mouseCallback_);
    glfwSetScrollCallback(window, scrollCallback_);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "failed to load glad." << std::endl;
        glfwTerminate();
        return false;
    }

    this->window_ = window;

    return true;
}


void Framework::startLoop() {
    this->time_ = glfwGetTime();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(this->window_)) {
        double now = glfwGetTime();
        frame_delta_ = static_cast<float>(now - this->time_);
        time_ = now;

        // input
        // -----
        this->processInput();

        // render
        // ------
        glClearColor(BACKGROUND_COLOR);
        glClear(GL_COLOR_BUFFER_BIT
            | GL_DEPTH_BUFFER_BIT
            | GL_STENCIL_BUFFER_BIT);

        this->drawFrame();

        // glfw: swap buffers and poll IO events
        // (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(this->window_);
        glfwPollEvents();

        isleep(this->frame_interval_);
    }

    glfwTerminate();
    this->window_ = nullptr;
}

void Framework::drawFrame() {
    if (pencil_) {
        pencil_->Draw();
    }
}

void Framework::stopLoop() {
    if (this->window_ != nullptr) {
        glfwSetWindowShouldClose(this->window_, 1);
    }
}

void Framework::processInput() {
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }

    for (auto it : process_input_delegate_list_) {
        it->ProcessInput();
    }
}

// glfw: whenever the window size changed (by OS or user resize)
// this callback function executes
void framebufferSizeCallback_(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions;
    // note that width and height will be significantly larger than
    // specified on retina displays.
    Framework::Instance()->framebufferSizeCallback(window, width, height);
}

void Framework::framebufferSizeCallback(
    GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Framework::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    this->Position(xpos, ypos);
    // camera::Instance()->ProcessMouseMovement(xoffset, yoffset);
}

void mouseCallback_(GLFWwindow* window, double xpos, double ypos) {
    Framework::Instance()->mouseCallback(window, xpos, ypos);
}

void Framework::scrollCallback(
    GLFWwindow* window, double xoffset, double yoffset) {
    // camera::Instance()->ProcessMouseScroll(yoffset);
    this->Scroll(xoffset, yoffset);
}

void scrollCallback_(GLFWwindow* window, double xoffset, double yoffset) {
    Framework::Instance()->scrollCallback(window, xoffset, yoffset);
}

static inline void isleep(uint32_t millisecond) {
    usleep((millisecond << 10) - (millisecond << 4) - (millisecond << 3));
}

}  // namespace framework
