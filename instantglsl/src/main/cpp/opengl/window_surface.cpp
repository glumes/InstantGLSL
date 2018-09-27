//
// Created by glumes on 2018/9/3.
//

#include "window_surface.h"
#include <android/native_window.h>

WindowSurface::WindowSurface(EGLCore *eglCore, ANativeWindow *window) : EglSurfaceBase(eglCore) {

    createWindowSurface(window);
    mSurface = window;
}

WindowSurface::WindowSurface(EGLCore *eglCore, ANativeWindow *window, bool releaseSurface)
        : EglSurfaceBase(eglCore) {
    createWindowSurface(window);
    mSurface = window;
    mReleaseSurface = releaseSurface;
}

void WindowSurface::release() {
    releaseEglSurface();
    if (mSurface != nullptr) {
        if (mReleaseSurface) {
            ANativeWindow_release(mSurface);
            mSurface = nullptr;
        }
    }
}

void WindowSurface::recreate(EGLCore *newEglCore) {
    if (mSurface == nullptr) {
        throw runtime_error("not yet implemented for surface");
    }
    mEglCore = newEglCore;
    createWindowSurface(mSurface);
}


