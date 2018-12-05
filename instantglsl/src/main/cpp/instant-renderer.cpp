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


using namespace std;
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
                                                                               jstring jpath,
                                                                               jstring jassetpath,
                                                                               jobject assetManager) {
    const char *path = env->GetStringUTFChars(jpath, 0);


    const char *asset_path = env->GetStringUTFChars(jassetpath, 0);


    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);

    AAsset *pathAsset = AAssetManager_open(mgr, path, AASSET_MODE_UNKNOWN);

    off_t assetLength = AAsset_getLength(pathAsset);

    off_t start = 0;


    int fd = AAsset_openFileDescriptor(pathAsset, &start, &assetLength);


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


extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_instantglsl_InstantRenderer_textureChange__Ljava_lang_String_2Ljava_lang_String_2Landroid_content_res_AssetManager_2(
        JNIEnv *env, jobject instance, jstring path_, jstring assetPath_, jobject assetManager) {


    const char *path = env->GetStringUTFChars(path_, 0);
    const char *assetPath = env->GetStringUTFChars(assetPath_, 0);


    AAssetManager *mgr = AAssetManager_fromJava(env, assetManager);

    AAsset *pathAsset = AAssetManager_open(mgr, assetPath, AASSET_MODE_UNKNOWN);

    off_t assetLength = AAsset_getLength(pathAsset);

    off_t start = 0;

    int fd = AAsset_openFileDescriptor(pathAsset, &start, &assetLength);

    TLOGD("path is %d", fd);

    char filename[1024] = "temp";

    char buf[1024];

    snprintf(filename, 1024, "/proc/%ld/fd/%d", (long) getpid(), fd);

    TLOGD("file descriptor path  file name is %s", filename);

    readlink(filename, buf, 1024);

    TLOGD("file descriptor path is %s", buf);


    int w, h, n;

    TLOGD("texture path is %s", path);

//    const char *testpath = "/data/app/com.glumes.instant-izI2rnCBtWMvHbOSBSCDKg==/base.apk/texture/texture.jpg";

//    unsigned char *data = stbi_load(path, &w, &h, &n, 0);


    // read content from asset
    unsigned char *fileData = (unsigned char *) AAsset_getBuffer(pathAsset);

    //
    unsigned char *contnet = stbi_load_from_memory(fileData, assetLength, &w, &h, &n, 0);

    TLOGD("w is %d,height is %d,n is %d", w, h, n);

    mTextureId = loadTexture(w, h, contnet);

    drawer->onTextureChange(mTextureId);

    env->ReleaseStringUTFChars(path_, path);
    env->ReleaseStringUTFChars(assetPath_, assetPath);
}