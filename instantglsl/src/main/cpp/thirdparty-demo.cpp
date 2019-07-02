//
// Created by glumes on 2019-07-02.
//

#include <jni.h>
#include <Log.h>
#include <JpegHelper.h>

extern "C"
JNIEXPORT jint JNICALL
Java_com_glumes_instantglsl_demo_JpegDemo_compressBitmap(JNIEnv *env, jobject instance,
                                                         jobject bitmap, jstring file_) {
    const char *file = env->GetStringUTFChars(file_, 0);

    Log::d("test");
    env->ReleaseStringUTFChars(file_, file);

    return 0;
}