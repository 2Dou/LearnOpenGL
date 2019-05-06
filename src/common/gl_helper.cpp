//  Created by SelFree on 2019/2/27.
//  Copyright © 2019年 SelFree. All rights reserved.

#include "src/common/gl_helper.h"

namespace gl_helper {

VertexArrayObject::VertexArrayObject() {
    glGenVertexArrays(1, &id_);
}

VertexArrayObject::~VertexArrayObject() {
    Unbind();
    glDeleteVertexArrays(1, &id_);
}

GLuint VertexArrayObject::get_id() {
    return id_;
}

void VertexArrayObject::Bind() {
    glBindVertexArray(id_);
}

void VertexArrayObject::Unbind() {
    glBindVertexArray(0);
}

Texture2D::Texture2D() : id_(0) {}

Texture2D::~Texture2D() {
    Delete();
}

GLuint Texture2D::get_id() {
    return id_;
}

GLuint Texture2D::Load(const std::string &path) {
    id_ = util::LoadTexture2D(path);
    return id_;
}

void Texture2D::Bind(GLenum texture_index) {
    glActiveTexture(texture_index);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture2D::Delete() {
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
    }
}

CubeTexture::CubeTexture() : id_(0) {}

CubeTexture::~CubeTexture() {
    Delete();
}

GLuint CubeTexture::get_id() {
    return id_;
}

GLuint CubeTexture::Load(const std::vector<std::string> &files) {
    id_ = util::LoadCubeTexture(files);
    return id_;
}

void CubeTexture::Bind(GLenum texture_index) {
    glActiveTexture(texture_index);
    glBindTexture(GL_TEXTURE_2D, id_);
}

void CubeTexture::Delete() {
    if (id_ != 0) {
        glDeleteTextures(1, &id_);
    }
}

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
