//
// Created by glumes on 2018/9/6.
//

#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <android/native_window.h>

#include <opengl_util.h>
#include <window_surface.h>
#include <egl_core.h>
#include <texture_drawer.h>
#include <file_fast_map.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image_write.h>

using namespace OpenGLUtil;

EGLCore *eglCore = nullptr;
WindowSurface *windowSurface = nullptr;
ANativeWindow *nativeWindow = nullptr;
TextureDrawer *drawer = nullptr;

GLint mTextureId = 0;


#define OPENGL_TAG "OpenGLUtil"

#define TLOGD(...) __android_log_print(ANDROID_LOG_DEBUG,OPENGL_TAG,__VA_ARGS__)
#define TLOGE(...) __android_log_print(ANDROID_LOG_ERROR,OPENGL_TAG,__VA_ARGS__)


extern "C" {
JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_InstantRenderer_init(JNIEnv *env, jobject, jobject surface) {
    eglCore = new EGLCore(nullptr, 0);

    nativeWindow = ANativeWindow_fromSurface(env, surface);

    windowSurface = new WindowSurface(eglCore, nativeWindow, false);

    windowSurface->makeCurrent();

    drawer = new TextureDrawer();

}


JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_InstantRenderer_renderer(JNIEnv *, jobject) {

//    windowSurface->makeCurrent();

    drawer->renderer();

    windowSurface->swapBuffers();
}

JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_InstantRenderer_shaderChange(JNIEnv *env, jobject, jstring jpath) {
    const char *path = env->GetStringUTFChars(jpath, 0);

    TLOGE("shader change");

    const char *shader = FileFastMap::fastRead(path);

    if (shader != nullptr) {
        TLOGE("shader is not null");
        TLOGE("shader is %s", shader);
    } else {
        TLOGE("shader is empty");
    }

    drawer->onShaderChange(shader);

    env->ReleaseStringUTFChars(jpath, path);
}

JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_InstantRenderer_textureChange__I(JNIEnv *, jobject, jint texture) {
    mTextureId = texture;
    drawer->onTextureChange(mTextureId);

}

JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_InstantRenderer_textureChange__Ljava_lang_String_2(JNIEnv *env, jobject,
                                                                               jstring jpath) {
    const char *path = env->GetStringUTFChars(jpath, 0);

    int w, h, n;

    TLOGD("texture path is %s", path);

    unsigned char *data = stbi_load(path, &w, &h, &n, 0);

    TLOGD("w is %d,height is %d,n is %d", w, h, n);

    mTextureId = loadTexture(w, h, data);

    drawer->onTextureChange(mTextureId);

    env->ReleaseStringUTFChars(jpath, path);
}

JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_InstantRenderer_reportError(JNIEnv *, jobject) {

}

JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_InstantRenderer_destroy(JNIEnv *, jobject) {
    windowSurface->release();
    eglCore->release();
    delete windowSurface;
    delete eglCore;
    ANativeWindow_release(nativeWindow);
//    delete nativeWindow;
//    delete drawer;
}

}
