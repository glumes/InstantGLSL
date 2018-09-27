//
// Created by glumes on 2018/9/18.
//

#include "oes_texture_drawer.h"


OESTextureDrawer::OESTextureDrawer() {

    mVertexCoordinates = OpenGLUtil::DEFAULT_VERTEX_COORDIANTE;

    mTextureCoordinates = OpenGLUtil::DEFAULT_TEXTURE_COORDIANTE;

    mVertexShader = OpenGLUtil::DEFAULT_VERTEX_SHAER;

    mFragmentShader = OpenGLUtil::DEFAULT_OES_FRAGMENT_SHADER;

}

void OESTextureDrawer::onSurfaceCreated() {

    mProgram = OpenGLUtil::createProgram(mVertexShader, mFragmentShader);

    glUseProgram(mProgram);

    mPositionHandle = glGetAttribLocation(mProgram, "aPosition");

    mTextureCoordHandle = glGetAttribLocation(mProgram, "inputTextureCoordinate");

    mTextureHandle = glGetUniformLocation(mProgram, "inputImageTexture");

    mSTMatrixHandle = glGetUniformLocation(mProgram, "uSTMatrix");
}

void OESTextureDrawer::drawFrame(int texture, float *matrix) {


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture);
    glUniform1i(mTextureHandle, 0);

    glVertexAttribPointer(mPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, mVertexCoordinates);
    glEnableVertexAttribArray(mPositionHandle);

    glVertexAttribPointer(mTextureCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, mTextureCoordinates);
    glEnableVertexAttribArray(mTextureCoordHandle);

    glUniformMatrix4fv(mSTMatrixHandle, 1, GL_FALSE, matrix);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    OpenGLUtil::checkGLError("draw Frame");

    glDisableVertexAttribArray(mPositionHandle);

    glDisableVertexAttribArray(mTextureHandle);

    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);

}

OESTextureDrawer::~OESTextureDrawer() {

}


