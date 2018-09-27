//
// Created by glumes on 2018/9/3.
//

#ifndef OPENGLNATIVE_OFFSRCEEN_SURFACE_H
#define OPENGLNATIVE_OFFSRCEEN_SURFACE_H


#include "egl_surface_base.h"

class OffscreenSurface : public EglSurfaceBase {

public:
    OffscreenSurface(EGLCore *eglCore, int width, int height);

    void release();
};


#endif //OPENGLNATIVE_OFFSRCEEN_SURFACE_H
