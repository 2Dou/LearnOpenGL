//  Created by SelFree on 2019/03/02.
//  Copyright © 2019年 SelFree. All rights reserved.

#ifndef SRC_COMMON_EVENT_H_
#define SRC_COMMON_EVENT_H_

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace framework {

class IMouseEvent {
 public:
    virtual void Position(double xpos, double ypos) = 0;
    virtual void Scroll(double xoffset, double yoffset) = 0;
};

class MouseEventProxy : public IMouseEvent {
 public:
    MouseEventProxy();
    virtual ~MouseEventProxy();

    void set_impl(IMouseEvent* impl);
    IMouseEvent* get_impl() const;

    virtual void Position(double xpos, double ypos);
    virtual void Scroll(double xoffset, double yoffset);

 private:
    IMouseEvent   *impl_;
};

class IProcessInputDelegate {
 public:
    virtual ~IProcessInputDelegate() {}
    virtual void ProcessInput(void) = 0;
};

template<typename T>
class ProcessInputDelegate : public IProcessInputDelegate {
 public:
    ProcessInputDelegate(T* p, void (T::*func)(void))
        : p_(p), func_(func) {}

    void ProcessInput(void) {
        (p_->*func_)();
    }

 private:
    T* p_;
    void (T::*func_)(void);
};

template<>
class ProcessInputDelegate<void> : public IProcessInputDelegate {
 public:
    explicit ProcessInputDelegate(void (*func)(void))
        : func_(func) {}

    void ProcessInput(void) {
        (*func_)();
    }

 private:
    void (*func_)(void);
};

}  // namespace framework

#endif  // SRC_COMMON_EVENT_H_
