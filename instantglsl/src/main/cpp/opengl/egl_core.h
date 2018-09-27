//
// Created by glumes on 2018/9/3.
//

#ifndef OPENGLNATIVE_EGL_CORE_H
#define OPENGLNATIVE_EGL_CORE_H

#include <android/native_window.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <android/log.h>
#include <EGL/eglplatform.h>
#include <exception>
#include <iostream>
#define  OPENGL_LOG_TAG    "OpenGLUtil"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_INFO,OPENGL_LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,OPENGL_LOG_TAG,__VA_ARGS__)

#define FLAG_TRY_GLES3 0x02
#define FLAG_RECORDABLE 0x01

using namespace std;

typedef EGLBoolean (EGLAPIENTRYP EGL_PRESENTATION_TIME_ANDROIDPROC)(EGLDisplay display,
                                                                    EGLSurface surface,
                                                                    khronos_stime_nanoseconds_t time);

class EGLCore {
public:
    EGLCore();

    EGLCore(EGLContext sharedContext, int flags);

    ~EGLCore();

    void releaseSurface(EGLSurface eglSurface);

    EGLSurface createWindowSurface(ANativeWindow *surface);

    EGLSurface createOffscreenSurface(int width, int height);

    void makeCurrent(EGLSurface eglSurface);

    void makeCurrent(EGLSurface drawSurface, EGLSurface readSurface);

    void makeNothingCurrent();

    bool swapBuffers(EGLSurface eglSurface);

    void setPresentationTime(EGLSurface eglSurface, long nsecs);

    bool isCurrent(EGLSurface eglSurface);

    int querySurface(EGLSurface eglSurface, int what);

    const char *queryString(int what);

    int getGLVersion();

    void release();

private:

    EGLDisplay mEGLDisplay = EGL_NO_DISPLAY;
    EGLContext mEGLContext = EGL_NO_CONTEXT;
    EGLConfig mEGLConfig = nullptr;
    int mGLVersion = -1;

    bool init(EGLContext sharedContext, int flags);

    EGLConfig getConfig(int flags, int i);

    void checkEglError(const char *string);

    EGL_PRESENTATION_TIME_ANDROIDPROC eglPresentationTimeANDROID = NULL;

};


#endif //OPENGLNATIVE_EGL_CORE_H
