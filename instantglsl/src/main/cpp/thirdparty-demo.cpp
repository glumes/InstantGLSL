//
// Created by glumes on 2019-07-02.
//

#include <jni.h>
#include <Log.h>
#include <JpegHelper.h>
#include <android/bitmap.h>

extern "C"
JNIEXPORT jint JNICALL
Java_com_glumes_instantglsl_demo_JpegDemo_compressBitmap(JNIEnv *env, jobject instance,
                                                         jobject bitmap, jstring file_) {
    const char *file = env->GetStringUTFChars(file_, 0);

    Log::d("test");

    AndroidBitmapInfo bitmapInfo;
    int ret;
    if ((ret = AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) < 0) {
        Log::d("get bitmap info failed");
        return 0;
    }

    if (bitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
        Log::d("format is rgba_8888");
    }

    u_int8_t *bitmapPixels;
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, reinterpret_cast<void **>(&bitmapPixels))) <
        0) {
        Log::d("lock pixels failed");
        return 0;
    }


    uint32_t width = bitmapInfo.width;
    uint32_t height = bitmapInfo.height;
    uint32_t format = bitmapInfo.format;

    uint32_t newWidth = bitmapInfo.width;
    uint32_t newHeight = bitmapInfo.height;

    uint32_t *newBitmapPixels = new uint32_t[newWidth * newHeight];

    int whereToGet = 0;

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; x++) {
            uint32_t pixel = ((uint32_t *) bitmapPixels)[whereToGet++];
            newBitmapPixels[newWidth * (newHeight - 1 - y) + x] = pixel;
        }
    }


    u_int8_t r, g, b;
    int color;
    u_int8_t *data;
    u_int8_t *tmpData;


    data = (u_int8_t *) malloc(width * height * 3);
    tmpData = data;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
                color = (*(int *) (bitmapPixels));
                b = (color >> 16) & 0xFF;
                g = (color >> 8) & 0xFF;
                r = (color >> 0) & 0xFF;
                *data = r;
                *(data + 1) = g;
                *(data + 2) = b;
                data += 3;
                bitmapPixels += 4;
            } else {
                Log::d("format error");
                return 0;
            }
        }
    }

    JpegHelper jpegHelper;

    jpegHelper.generateJpeg(tmpData, width, height, 50, file,
                            0);

    AndroidBitmap_unlockPixels(env, bitmap);

    env->ReleaseStringUTFChars(file_, file);

    return 0;
}