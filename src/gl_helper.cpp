//  Created by SelFree on 2019/2/27.
//  Copyright © 2019年 SelFree. All rights reserved.

#include "src/gl_helper.h"

namespace gl_helper {

template <>
void VertexAttribPointer<float>(
    GLuint index            // 索引
    , GLint size            // 大小
    , GLsizei stride        // 步长
    , uint32_t offset       // 起始位置偏移
    , GLboolean normalized  // 是否标准化
    , float *unused) {
    ::glVertexAttribPointer(index, size
        , GL_FLOAT, normalized
        , stride * sizeof(float)
        , reinterpret_cast<float*>(0) + offset);
}

}  // namespace gl_helper
