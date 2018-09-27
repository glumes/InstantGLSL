//
// Created by glumes on 2018/9/6.
//

#include "texture_drawer.h"

TextureDrawer::TextureDrawer() {

    mVertexCoordinates = OpenGLUtil::DEFAULT_VERTEX_COORDIANTE;

    mTextureCoordinates = OpenGLUtil::DEFAULT_TEXTURE_COORDIANTE;


    mVertexShader = OpenGLUtil::DEFAULT_TEXTURE_VERTEX_SHADER;

    mFragmentShader = OpenGLUtil::DEFAULT_TEXTURE_FRAGMENT_SHADER;

}

void TextureDrawer::createProgram() {
    if (mProgram != 0) {
        glDeleteProgram(mProgram);
    }

    mProgram = OpenGLUtil::createProgram(mVertexShader, mFragmentShader);
}


void TextureDrawer::getHandleInShader() {
    mPositionHandle = glGetAttribLocation(mProgram, "aPosition");

//    mMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");

    mTextureCoordHandle = glGetAttribLocation(mProgram, "inputTextureCoordinate");

    mTextureHandle = glGetUniformLocation(mProgram, "inputImageTexture");

}

void TextureDrawer::setHandleInShader() {

}

void TextureDrawer::enableAttribArray() {
    glVertexAttribPointer(mPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, mVertexCoordinates);
    glEnableVertexAttribArray(mPositionHandle);

    glVertexAttribPointer(mTextureCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, mTextureCoordinates);
    glEnableVertexAttribArray(mTextureCoordHandle);
}

void TextureDrawer::bindTexture() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    glUniform1i(mTextureHandle, 0);
}

void TextureDrawer::renderer() {

    createProgram();

    glUseProgram(mProgram);

    getHandleInShader();

    setHandleInShader();

    bindTexture();

    enableAttribArray();

    draw();

    disableAttribArray();

    unBindTexture();
}

void TextureDrawer::draw() {
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


void TextureDrawer::disableAttribArray() {

    glDisableVertexAttribArray(mPositionHandle);

    glDisableVertexAttribArray(mTextureHandle);
}


void TextureDrawer::unBindTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureDrawer::onSizeChange(int width, int height) {
    mWidth = width;
    mHeight = height;
}

void TextureDrawer::onShaderChange(const char *shader) {
    mFragmentShader = shader;
}

void TextureDrawer::onTextureChange(GLuint textureId) {
    mTextureId = textureId;
}

