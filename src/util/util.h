//  Created by SelFree on 2019/2/21.
//  Copyright © 2018年 SelFree. All rights reserved.

#ifndef SRC_UTIL_UTIL_H_
#define SRC_UTIL_UTIL_H_

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>

namespace util {

// 查找资源路径
std::string GetResourceFilename(const std::string &name);

// 加载纹理
GLuint LoadTexture2D(const std::string &path, GLuint mode);

}  // namespace util


#endif  // SRC_UTIL_UTIL_H_
