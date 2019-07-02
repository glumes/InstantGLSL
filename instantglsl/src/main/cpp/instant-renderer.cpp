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
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <stdio.h>
#include <unistd.h>
#include <PngHelper.h>
#include <Log.h>

using namespace std;
using namespace OpenGLUtil;

EGLCore *eglCore = nullptr;
WindowSurface *windowSurface = nullptr;
ANativeWindow *nativeWindow = nullptr;
TextureDrawer *drawer = nullptr;

GLint mTextureId = 0;

extern "C" {
JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_InstantRenderer_init(JNIEnv *env, jobject, jobject surface) {
    eglCore = new EGLCore(nullptr, 0);

    nativeWindow = ANativeWindow_fromSurface(env, surface);

    windowSurface = new WindowSurface(eglCore, nativeWindow, false);

    windowSurface->makeCurrent();

    drawer = new TextureDrawer();

//    pngHelper = new PngHelper();
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

    Log::d("shader change");

    const char *shader = FileFastMap::fastRead(path);

    if (shader != nullptr) {
        Log::d("shader is not null");
        Log::d("shader is %s", shader);
    } else {
        Log::d("shader is empty");
    }

    drawer->onShaderChange(shader);

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


extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_InstantRenderer_textureChangeFromeAsset(JNIEnv *env, jobject instance,
                                                                    jstring assetPath_,
                                                                    jobject assetManager) {
    const char *assetPath = env->GetStringUTFChars(assetPath_, 0);

    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);

    AAsset *pathAsset = AAssetManager_open(mgr, assetPath, AASSET_MODE_UNKNOWN);

    off_t assetLength = AAsset_getLength(pathAsset);

    unsigned char *fileData = (unsigned char *) AAsset_getBuffer(pathAsset);

    int w, h, n;

    unsigned char *content = stbi_load_from_memory(fileData, assetLength, &w, &h, &n, 0);

    mTextureId = loadTexture(w, h, content);

    drawer->onTextureChange(mTextureId);

    env->ReleaseStringUTFChars(assetPath_, assetPath);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_InstantRenderer_textureChagneFromSdcard(JNIEnv *env, jobject instance,
                                                                    jstring filePath_) {
    const char *filePath = env->GetStringUTFChars(filePath_, 0);

    Log::d("path is %s",filePath);

    int w, h, n;


    unsigned char *content = stbi_load(filePath, &w, &h, &n, 0);

//
    PngHelper helper(filePath);

//    TLOGD("png width is %d height is %d and alpha is %d",helper.getWidth(),helper.getHeight(),helper.has_alpha());

//    helper.read_png_file(const_cast<char *>(filePath));
//
//    helper.process_png_file();
//
//    helper.write_png_file("/storage/emulated/0/logo22.png");

    mTextureId = loadTexture(w, h, helper.getPixelData());

    Log::d("on texturechange");

    drawer->onTextureChange(mTextureId);

    env->ReleaseStringUTFChars(filePath_, filePath);
}
