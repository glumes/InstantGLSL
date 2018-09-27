//
// Created by glumes on 2018/9/3.
//

#include "egl_core.h"


EGLCore::EGLCore() {
    init(nullptr, 0);
}

EGLCore::EGLCore(EGLContext sharedContext, int flags) {
    init(sharedContext, flags);
}


EGLCore::~EGLCore() {
    release();
}

/**
 *
 * @param sharedContext
 * @param flags
 * @return
 */
bool EGLCore::init(EGLContext sharedContext, int flags) {
    if (mEGLDisplay != EGL_NO_DISPLAY) {
        LOGE("EGL already set up");
//        throw runtime_error("EGL already set up");
    }

    if (sharedContext == nullptr) {
        sharedContext = EGL_NO_CONTEXT;
    }

    mEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mEGLDisplay == EGL_NO_DISPLAY) {
        LOGE("EGL already set up");
//        throw runtime_error("unable to get EGL14 display");
    }

//    int *version[2];
    if (!eglInitialize(mEGLDisplay, 0, 0)) {
        mEGLDisplay = nullptr;
//        throw runtime_error("unable to initialize EGL");
        LOGE("EGL already set up");

    }

    if ((flags & FLAG_TRY_GLES3) != 0) {
        EGLConfig config = getConfig(flags, 3);
        if (config != nullptr) {
            int attrib3_list[] = {
                    EGL_CONTEXT_CLIENT_VERSION, 3,
                    EGL_NONE
            };
            EGLContext context = eglCreateContext(mEGLDisplay, config, sharedContext, attrib3_list);
            if (eglGetError() == EGL_SUCCESS) {
                mEGLConfig = config;
                mEGLContext = context;
                mGLVersion = 3;
            }
        }
    }
    if (mEGLContext == EGL_NO_CONTEXT) {
        EGLConfig config = getConfig(flags, 2);
        if (config == nullptr) {
//            throw runtime_error("Unable to find a suitable EGLConfig");
            LOGE("EGL already set up");

        }
        int attrib2_list[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };
        EGLContext context = eglCreateContext(mEGLDisplay, config, sharedContext, attrib2_list);
        checkEglError("eglCreateContext");
        mEGLConfig = config;
        mEGLContext = context;
        mGLVersion = 2;
    }

    int values;
    eglQueryContext(mEGLDisplay, mEGLContext, EGL_CONTEXT_CLIENT_VERSION, &values);
    LOGD("EGLContext created, client version %d", values);
    return true;
}


/**
 *
 * @param flags
 * @param version
 * @return
 */
EGLConfig EGLCore::getConfig(int flags, int version) {
    int renderableType = EGL_OPENGL_ES2_BIT;
    if (version >= 3) {
        renderableType |= EGL_OPENGL_ES3_BIT_KHR;
    }

    int attribList[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
//            EGL_DEPTH_SIZE, 16,
//            EGL_STENCIL_SIZE, 8,
            EGL_RENDERABLE_TYPE, renderableType,
            EGL_NONE, 0,
            EGL_NONE
    };
    int length = sizeof(attribList) / sizeof(attribList[0]);
    if ((flags & FLAG_RECORDABLE) != 0) {
        attribList[length - 3] = EGL_RECORDABLE_ANDROID;
        attribList[length - 2] = 1;
    }

    EGLConfig configs = nullptr;
    int numConfigs;
    if (!eglChooseConfig(mEGLDisplay, attribList, &configs, 1, &numConfigs)) {
        LOGE("unable to find RGB8888  %d  EGLConfig", version);
        return nullptr;
    }
    return configs;
}


int EGLCore::querySurface(EGLSurface eglSurface, int what) {
    int value;
    eglQuerySurface(mEGLDisplay, eglSurface, what, &value);
    return value;
}

const char *EGLCore::queryString(int what) {
    return eglQueryString(mEGLDisplay, what);
}

int EGLCore::getGLVersion() {
    return mGLVersion;
}

bool EGLCore::isCurrent(EGLSurface eglSurface) {
    return mEGLContext == eglGetCurrentContext() && eglSurface == eglGetCurrentSurface(EGL_DRAW);
}

void EGLCore::setPresentationTime(EGLSurface eglSurface, long nsecs) {
    eglPresentationTimeANDROID(mEGLDisplay, eglSurface, nsecs);
}

/**
 *
 */
void EGLCore::release() {
    if (mEGLDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(mEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(mEGLDisplay, mEGLContext);
        eglReleaseThread();
        eglTerminate(mEGLDisplay);
    }
    mEGLDisplay = EGL_NO_DISPLAY;
    mEGLContext = EGL_NO_CONTEXT;
    mEGLConfig = nullptr;
}

/**
 *
 * @param eglSurface
 * @return
 */
bool EGLCore::swapBuffers(EGLSurface eglSurface) {
    return (bool) eglSwapBuffers(mEGLDisplay, eglSurface);
}

void EGLCore::makeNothingCurrent() {
    if (!eglMakeCurrent(mEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
        throw runtime_error("eglMakeCurrent failed");
    }
}

/**
 *
 * @param drawSurface
 * @param readSurface
 */
void EGLCore::makeCurrent(EGLSurface drawSurface, EGLSurface readSurface) {
    if (mEGLDisplay == EGL_NO_DISPLAY) {
        LOGE("NOTE: makeCurrent w/o display");
    }
    if (!eglMakeCurrent(mEGLDisplay, drawSurface, readSurface, mEGLContext)) {
        throw runtime_error("eglMakeCurrent(draw,read) failed");
    }
}

/**
 *
 * @param eglSurface
 */
void EGLCore::makeCurrent(EGLSurface eglSurface) {
    if (mEGLDisplay == EGL_NO_DISPLAY) {
        LOGE("NOTE: makeCurrent w/o display");
    }
    if (!eglMakeCurrent(mEGLDisplay, eglSurface, eglSurface, mEGLContext)) {
        throw runtime_error("eglMakeCurrent failed");
    }
}

/**
 *
 * @param width
 * @param height
 * @return
 */
EGLSurface EGLCore::createOffscreenSurface(int width, int height) {
    int surfaceAttribs[] = {
            EGL_WIDTH, width,
            EGL_HEIGHT, height,
            EGL_NONE
    };
    EGLSurface eglSurface = eglCreatePbufferSurface(mEGLDisplay, mEGLConfig, surfaceAttribs);
    checkEglError("eglCreatePbufferSurface");
    if (eglSurface == nullptr) {
        throw runtime_error("surface was null");
    }
    return nullptr;
}

/**
 *
 * @param surface
 * @return
 */
EGLSurface EGLCore::createWindowSurface(ANativeWindow *surface) {
    if (surface == nullptr) {
        return nullptr;
    }
    int surfaceAttribs[] = {
            EGL_NONE
    };
    EGLSurface eglSurface = eglCreateWindowSurface(mEGLDisplay, mEGLConfig, surface,
                                                   surfaceAttribs);
    checkEglError("eglCreateWindowSurface");
    if (eglSurface == nullptr) {
        throw runtime_error("surface was null");
    }

    return eglSurface;
}

/**
 *
 * @param eglSurface
 */
void EGLCore::releaseSurface(EGLSurface eglSurface) {
    eglDestroySurface(mEGLDisplay, eglSurface);
}


void EGLCore::checkEglError(const char *msg) {
    int error;
    if ((error = eglGetError()) != EGL_SUCCESS) {
        LOGE("%s: EGL error: %x", msg, error);
        string str = string(msg) + ": EGL error :" + std::to_string(error);
//        throw runtime_error(str);
        LOGE("EGL already set up");

    }
}

