//  Created by SelFree on 2019/2/21.
//  Copyright © 2018年 SelFree. All rights reserved.

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>

#include "src/util/util.h"

namespace util {

// 加载纹理
GLuint LoadTexture2D(const std::string &path) {
    // 垂直翻转
    stbi_set_flip_vertically_on_load(true);

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER
        , GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载并生成纹理
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str()
        , &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = 0;
        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format
            , width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);  // 生成多级渐远纹理
    } else {
        std::cout << "Failed to load texture" << std::endl;
        glDeleteTextures(1, &id);
        return -1;
    }

    stbi_image_free(data);

    return id;
}

}  // namespace util
