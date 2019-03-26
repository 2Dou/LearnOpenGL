// Created by SelFree on 2019/2/28.
// Copyright © 2019年 SelFree. All rights reserved.

#include "src/practice/01_getting_started/02_texture_test.h"


namespace getting_started {

TextureTest::TextureTest() {
}

TextureTest::~TextureTest() {
}

void TextureTest::Init() {
    vao_.Bind();

    gl_helper::VertexBufferObject<float> vbo;

    float data[] = {
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f
    };
    vbo.SetData(sizeof(data)/sizeof(float), data);

    gl_helper::VertexAttribPointer<float>(0, 3, 5);
    gl_helper::VertexAttribPointer<float>(1, 2, 5, 3);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    texture1_.Load(util::GetResourceFilename("texture/container.jpg")
        , GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE1);
    texture2_.Load(util::GetResourceFilename("texture/awesomeface.png")
        , GL_TEXTURE1);

    shader_.Compile(util::GetResourceFilename("shaders/01_getting_started/02_texture/vertex.vs")
                    , util::GetResourceFilename("shaders/01_getting_started/02_texture/fragment.fs"));
    shader_.Use();  // 设置之前必须启用
    shader_.SetInt("texture_id1", texture1_.get_texture_index() - GL_TEXTURE0);
    shader_.SetInt("texture_id2", texture2_.get_texture_index() - GL_TEXTURE0);

    vao_.Unbind();
}

void TextureTest::Draw() {
    shader_.Use();
    vao_.Bind();

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

}  // namespace getting_started
