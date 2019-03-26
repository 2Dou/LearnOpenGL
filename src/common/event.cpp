//  Created by SelFree on 2019/03/02.
//  Copyright © 2019年 SelFree. All rights reserved.

#include "src/common/event.h"

namespace framework {

MouseEventProxy::MouseEventProxy()
    : impl_(nullptr) {
}

MouseEventProxy::~MouseEventProxy() {
}

void MouseEventProxy::set_impl(IMouseEvent *impl) {
    impl_ = impl;
}

IMouseEvent * MouseEventProxy::get_impl() const {
    return impl_;
}

void MouseEventProxy::Position(double xpos, double ypos) {
    if (!impl_) {
        return;
    }
    impl_->Position(xpos, ypos);
}

void MouseEventProxy::Scroll(double xoffset, double yoffset) {
    if (!impl_) {
        return;
    }
    impl_->Scroll(xoffset, yoffset);
}

}  // namespace framework
