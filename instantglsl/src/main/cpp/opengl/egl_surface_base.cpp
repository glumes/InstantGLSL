//
// Created by glumes on 2018/9/3.
//

#include "egl_surface_base.h"

EglSurfaceBase::EglSurfaceBase(EGLCore *eglCore) : mEglCore(eglCore) {

}

void EglSurfaceBase::createWindowSurface(ANativeWindow *nativeWindow) {
    if (mEGLSurface != EGL_NO_SURFACE) {
        throw runtime_error("surface already created");
    }
    mEGLSurface = mEglCore->createWindowSurface(nativeWindow);
}

void EglSurfaceBase::createOffscreenSurface(int width, int height) {
    if (mEGLSurface != EGL_NO_SURFACE) {
        throw runtime_error("surface already created");
    }
    mEGLSurface = mEglCore->createOffscreenSurface(width, height);
    mWidth = width;
    mHeight = height;
}

int EglSurfaceBase::getWidth() {
    if (mWidth < 0) {
        return mEglCore->querySurface(mEGLSurface, EGL_WIDTH);
    } else {
        return mWidth;
    }
}


int EglSurfaceBase::getHeight() {
    if (mHeight < 0) {
        return mEglCore->querySurface(mEGLSurface, EGL_HEIGHT);
    } else {
        return mHeight;
    }
}

void EglSurfaceBase::releaseEglSurface() {
    mEglCore->releaseSurface(mEGLSurface);
    mEGLSurface = EGL_NO_SURFACE;
    mWidth = mHeight = -1;
}

void EglSurfaceBase::makeCurrent() {
    mEglCore->makeCurrent(mEGLSurface);
}

void EglSurfaceBase::makeCurrentReadFrom(EglSurfaceBase readSurface) {
    mEglCore->makeCurrent(mEGLSurface, readSurface.mEGLSurface);
}

bool EglSurfaceBase::swapBuffers() {
    bool result = mEglCore->swapBuffers(mEGLSurface);
    if (!result) {
        LOGE("WARNING: swapBuffers() failed");
    }
    return result;
}

void EglSurfaceBase::setPresentationTime(long nsecs) {
    mEglCore->setPresentationTime(mEGLSurface, nsecs);
}





