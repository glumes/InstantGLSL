//
// Created by glumes on 2018/9/3.
//

#ifndef OPENGLNATIVE_WINDOW_SURFACE_H
#define OPENGLNATIVE_WINDOW_SURFACE_H

#include <android/native_window.h>
#include "egl_surface_base.h"
class WindowSurface : public EglSurfaceBase {

public:
    WindowSurface(EGLCore *eglCore, ANativeWindow *window);

    WindowSurface(EGLCore *eglCore, ANativeWindow *window, bool releaseSurface);

    void release();

    void recreate(EGLCore *newEglCore);

private:
    ANativeWindow *mSurface = nullptr;
    bool mReleaseSurface;

};


#endif //OPENGLNATIVE_WINDOW_SURFACE_H
