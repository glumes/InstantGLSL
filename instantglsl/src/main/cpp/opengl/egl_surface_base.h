//
// Created by glumes on 2018/9/3.
//

#ifndef OPENGLNATIVE_EGL_SURFACE_BASE_H
#define OPENGLNATIVE_EGL_SURFACE_BASE_H

#include "egl_core.h"

class EglSurfaceBase {

public:
    EglSurfaceBase(EGLCore *eglCore);

    void createWindowSurface(ANativeWindow *nativeWindow);

    void createOffscreenSurface(int width, int height);

    int getWidth();

    int getHeight();

    void releaseEglSurface();

    void makeCurrent();

    void makeCurrentReadFrom(EglSurfaceBase readSurface);

    bool swapBuffers();

    void setPresentationTime(long nsecs);


protected:
    EGLCore *mEglCore;
    EGLSurface mEGLSurface = EGL_NO_SURFACE;
    int mWidth = -1;
    int mHeight = -1;

};


#endif //OPENGLNATIVE_EGL_SURFACE_BASE_H
