//
// Created by glumes on 2018/9/18.
//

#ifndef INSTANTGLSL_OES_TEXTURE_DRAWER_H
#define INSTANTGLSL_OES_TEXTURE_DRAWER_H

#include <jni.h>
#include <string>
#include <iostream>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#include <sstream>
#include <opengl_util.h>

using namespace OpenGLUtil;

class OESTextureDrawer {

private:

    const GLfloat *mVertexCoordinates;
    const GLfloat *mTextureCoordinates;

    const char *mVertexShader;
    const char *mFragmentShader;

    GLuint mProgram = 0;

    GLint mPositionHandle;
    GLint mTextureCoordHandle;
    GLint mTextureHandle;
    GLint mSTMatrixHandle;

public:
    OESTextureDrawer();

    ~OESTextureDrawer();

    void onSurfaceCreated();

    void drawFrame(int texture, float *matrix);
};


#endif //INSTANTGLSL_OES_TEXTURE_DRAWER_H
