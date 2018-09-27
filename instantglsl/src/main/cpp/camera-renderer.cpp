//
// Created by glumes on 2018/9/18.
//
#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <android/native_window.h>

#include <opengl_util.h>
#include <window_surface.h>
#include <egl_core.h>
#include <oes_texture_drawer.h>
#include <texture_drawer.h>

using namespace OpenGLUtil;

EGLCore *eglCore = nullptr;
WindowSurface *windowSurface = nullptr;
ANativeWindow *nativeWindow = nullptr;
OESTextureDrawer *oesTextureDrawer = nullptr;
TextureDrawer *textureDrawer = nullptr;

extern "C" {
JNIEXPORT void JNICALL Java_com_glumes_instantglsl_CameraRenderer_onSurfaceCreated
        (JNIEnv *env, jobject obj, jobject surface) {

    eglCore = new EGLCore(nullptr, 0);

    nativeWindow = ANativeWindow_fromSurface(env, surface);

    windowSurface = new WindowSurface(eglCore, nativeWindow, false);

    windowSurface->makeCurrent();

    textureDrawer = new TextureDrawer();
    oesTextureDrawer = new OESTextureDrawer();

    oesTextureDrawer->onSurfaceCreated();
}

JNIEXPORT void JNICALL Java_com_glumes_instantglsl_CameraRenderer_onSurfaceChanged
        (JNIEnv *, jobject, jint, jint) {

}

JNIEXPORT void JNICALL Java_com_glumes_instantglsl_CameraRenderer_onSurfaceDestroyed
        (JNIEnv *, jobject) {
    windowSurface->release();
    eglCore->release();
    delete windowSurface;
    delete eglCore;
    ANativeWindow_release(nativeWindow);
}


JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_CameraRenderer_draw(JNIEnv *env, jobject instance, jint textureId,
                                                jfloatArray matrix_) {
    jfloat *matrix = env->GetFloatArrayElements(matrix_, NULL);


    oesTextureDrawer->drawFrame(textureId, matrix);

    env->ReleaseFloatArrayElements(matrix_, matrix, 0);
}

JNIEXPORT jint JNICALL Java_com_glumes_instantglsl_CameraRenderer_getTextureId
        (JNIEnv *, jobject) {
    return OpenGLUtil::createOESTexture();
}

JNIEXPORT void JNICALL Java_com_glumes_instantglsl_CameraRenderer_swapBuffers
        (JNIEnv *, jobject) {
    LOGD("swap buffers");
    windowSurface->swapBuffers();
}

JNIEXPORT void JNICALL Java_com_glumes_instantglsl_CameraRenderer_shaderChange
        (JNIEnv *, jobject, jstring) {
}

}