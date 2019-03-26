//  Created by SelFree on 2019/2/26.
//  Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_FRAMEWORK_H_
#define SRC_FRAMEWORK_H_

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <list>
#include <mutex>
#include <string>

#include "src/common/event.h"
#include "src/util/singleton.h"
#include "src/config.h"

namespace framework {

class IPencil {
 public:
    virtual void Init() = 0;
    virtual void Draw() = 0;
    virtual void Attached() {}
    virtual void Dettached() {}
};

class Framework : public Singleton<Framework>, public MouseEventProxy {
 public:
    explicit Framework(int frames_per_second = 60);
    virtual ~Framework();

    virtual GLFWwindow *Init(const std::string name
                    , int width = DEFAULT_SCREEN_WIDTH
                    , int height = DEFAULT_SCREEN_HEIGHT);

    // 开始
    // 只能在主线程调用
    void Start();

    // 停止
    void Stop();

    // 设置每秒帧数
    void SetFramesPerSecond(int count);

    // 获取GLFWwindow
    GLFWwindow* get_window() const;

    // 获取屏幕宽度
    int get_width() const;

    // 获取屏幕高度
    int get_height() const;

    // 获取当前帧与上一帧间隔
    float get_frame_delta() const;

    // 设置画笔
    void set_pencil(IPencil *pencil);

    void AppendProcessInputDelegate(IProcessInputDelegate *delegate);

    void RemoveProcessInputDelegate(IProcessInputDelegate *delegate);

 protected:
    virtual void drawFrame();

    virtual void processInput();

    virtual void framebufferSizeCallback(GLFWwindow* window
                                        , int width, int height);
    friend void framebufferSizeCallback_(GLFWwindow*, int, int);

    void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    friend void mouseCallback_(GLFWwindow*, double, double);

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    friend void scrollCallback_(GLFWwindow*, double, double);

    bool init(const std::string &name);

    // 循环
    void startLoop();

    // 退出循环
    void stopLoop();

 private:
    GLFWwindow  *window_;
    int width_;
    int height_;

    double time_;

    float   frame_delta_;
    uint32_t frame_interval_;

    IPencil* pencil_;

    std::list<IProcessInputDelegate*> process_input_delegate_list_;
};

Framework* Instance();

}  // namespace framework


#endif  // SRC_FRAMEWORK_H_
