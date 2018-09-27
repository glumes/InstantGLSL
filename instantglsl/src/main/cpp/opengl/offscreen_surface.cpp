//
// Created by glumes on 2018/9/3.
//

#include "offscreen_surface.h"

OffscreenSurface::OffscreenSurface(EGLCore *eglCore, int width, int height) : EglSurfaceBase(
        eglCore) {

    createOffscreenSurface(width, height);
}

void OffscreenSurface::release() {
    releaseEglSurface();
}
