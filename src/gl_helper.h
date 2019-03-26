//  Created by SelFree on 2019/2/27.
//  Copyright © 2019年 SelFree. All rights reserved.

#pragma once

#ifndef SRC_GL_HELPER_H_
#define SRC_GL_HELPER_H_

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <string>

#include "src/util/util.h"

namespace gl_helper {

// 顶点数组对象
class VertexArrayObject {
 public:
    VertexArrayObject() {
        glGenVertexArrays(1, &id_);
    }
    ~VertexArrayObject() {
        Unbind();
        glDeleteVertexArrays(1, &id_);
    }

    GLuint get_id() {
        return id_;
    }

    void Bind() {
        glBindVertexArray(id_);
    }
    void Unbind() {
        glBindVertexArray(0);
    }

 private:
    GLuint id_;
};

// 顶点缓存对象
template <typename T>
class VertexBufferObject {
 public:
    explicit VertexBufferObject(GLenum target = GL_ARRAY_BUFFER)
            : target_(target) {
                glGenBuffers(1, &id_);
            }
    ~VertexBufferObject() {
        this->Unbind();
        glDeleteBuffers(1, &id_);
    }

    GLuint get_id() {
        return id_;
    }

    // GL_STATIC_DRAW ：数据不会或几乎不会改变。
    // GL_DYNAMIC_DRAW：数据会被改变很多。
    // GL_STREAM_DRAW ：数据每次绘制时都会改变。
    void SetData(uint32_t length, T* data, GLenum usage = GL_STATIC_DRAW) {
        this->Bind();
        glBufferData(target_, sizeof(T) * length, data, usage);
    }

    void Bind() {
        glBindBuffer(target_, id_);
    }
    void Unbind() {
        glBindBuffer(target_, 0);
    }

 private:
    GLenum  target_;
    GLuint  id_;
};


class Texture2D {
 public:
    Texture2D() : id_(0), texture_index_(GL_TEXTURE0) {}

    ~Texture2D() {
        glDeleteTextures(1, &id_);
    }

    GLuint get_id() {
        return id_;
    }

    GLenum get_texture_index() {
        return texture_index_;
    }

    GLuint Load(const std::string &path
        , GLuint mode
        , GLenum texture_index = GL_TEXTURE0) {
        texture_index_ = texture_index;
        this->Active();
        id_ = util::LoadTexture2D(path, mode);
        return id_;
    }

    void Bind(GLenum texture = GL_TEXTURE0) {
        glActiveTexture(texture);
        glBindTexture(GL_TEXTURE_2D, id_);
    }

    void Active() {
        glActiveTexture(texture_index_);
    }

 private:
    GLuint  id_;
    GLenum  texture_index_;
};


template <typename T>
void VertexAttribPointer(
    GLuint index            // 索引
    , GLint size            // 大小
    , GLsizei stride        // 步长
    , uint32_t offset = 0   // 起始位置偏移
    , GLboolean normalized = GL_FALSE  // 是否标准化
    , T *unused = nullptr);

}  // namespace gl_helper

#endif  // SRC_GL_HELPER_H_
