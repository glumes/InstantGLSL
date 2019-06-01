//
// Created by glumes on 2019-06-01.
//

#include "PngHelper.h"

#include <android/log.h>

#define LOG_TAG ("OpenGLUtil")
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))

PngHelper::PngHelper(const string &file_name) :
        file_name(file_name), data(nullptr),
        width(0), height(0),
        bit_depth(0), color_type(0), interlace_type(0),
        compression_type(0), filter_method(0) {


    FILE *fp = fopen(file_name.c_str(), "rb");
    if (fp == nullptr) {
        return;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    png_infop info = png_create_info_struct(png);

    if (setjmp(png_jmpbuf(png))) {

    }

    png_init_io(png, fp);

    unsigned int sig_bytes = 0;
    png_set_sig_bytes(png, sig_bytes);

    png_read_png(png, info, (PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND),
                 nullptr);
    png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, &interlace_type,
                 &compression_type, &filter_method);

    LOGD("before bit_depth  is %d", bit_depth);
    LOGD("before interlace_type  is %d", interlace_type);
    LOGD("before compress type is %d", compression_type);
    LOGD("before filter method  type is %d", filter_method);

////    png_set_strip_16(png);
//    png_set_filter(png,PNG_FILTER_TYPE_BASE,PNG_FILTER_AVG);
//
//    png_set_IHDR(png,info,width,height,bit_depth,color_type,interlace_type,compression_type,PNG_FILTER_AVG);
//
////    png_read_update_info(png,info);
////
////    png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, &interlace_type,
////                 &compression_type, &filter_method);
//
//    LOGD("bit_depth  is %d", bit_depth);
//    LOGD("interlace_type  is %d", interlace_type);
//    LOGD("compress type is %d", compression_type);
//    LOGD("filter method  type is %d", filter_method);

    unsigned int row_bytes = png_get_rowbytes(png, info);
    if (data != nullptr) {
        delete[] data;
    }

    data = new unsigned char[row_bytes * height];

    png_bytepp rows = png_get_rows(png, info);
    for (int i = 0; i < height; ++i) {
        memcpy(data + (row_bytes * i), rows[i], row_bytes);
    }

    png_destroy_read_struct(&png, &info, nullptr);

}

PngHelper::~PngHelper() {

}

unsigned int PngHelper::getWidth() {
    return width;
}

unsigned int PngHelper::getHeight() {
    return height;
}

unsigned char *PngHelper::getData() {
    return data;
}

bool PngHelper::has_alpha() {
    if (color_type == PNG_COLOR_TYPE_RGBA) {
        return true;
    }
    return false;
}
