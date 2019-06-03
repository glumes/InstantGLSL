//
// Created by glumes on 2019-06-01.
//

#include "PngHelper.h"

#include <android/log.h>
#include <pngpriv.h>

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


//    FILE *fp = fopen(file_name.c_str(), "rb");
//    if (fp == nullptr) {
//        return;
//    }
//
//    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
//
//    png_infop info = png_create_info_struct(png);
//
//    if (setjmp(png_jmpbuf(png))) {
//
//    }
//
//    png_init_io(png, fp);
//
//    unsigned int sig_bytes = 0;
//    png_set_sig_bytes(png, sig_bytes);
//
//    png_read_png(png, info, (PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND),
//                 nullptr);
//    png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, &interlace_type,
//                 &compression_type, &filter_method);
//
//    LOGD("before bit_depth  is %d", bit_depth);
//    LOGD("before interlace_type  is %d", interlace_type);
//    LOGD("before compress type is %d", compression_type);
//    LOGD("before filter method  type is %d", filter_method);
//
//////    png_set_strip_16(png);
//    png_set_filter(png, PNG_FILTER_TYPE_BASE, PNG_NO_FILTERS);
////
//    png_set_IHDR(png, info, width, height, bit_depth, color_type, interlace_type, compression_type,
//                 PNG_FILTER_AVG);
////
////    png_read_update_info(png, info);
//////
////    png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, &interlace_type,
////                 &compression_type, &filter_method);
//
//    LOGD("bit_depth  is %d", bit_depth);
//    LOGD("interlace_type  is %d", interlace_type);
//    LOGD("compress type is %d", compression_type);
//    LOGD("filter method  type is %d", filter_method);
//
//    unsigned int row_bytes = png_get_rowbytes(png, info);
//    if (data != nullptr) {
//        delete[] data;
//    }
//
//    data = new unsigned char[row_bytes * height];
//
//
//    png_bytepp rows = png_get_rows(png, info);
//    LOGD("row_bytes is %d and height is %d", row_bytes,height);
//    for (int i = 0; i < height; ++i) {
//        memcpy(data + (row_bytes * i), rows[i], row_bytes);
//        LOGD("mem copy");
//    }
//
//    png_destroy_read_struct(&png, &info, nullptr);
//
//    LOGD("destroy read");
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

void PngHelper::read_png_file(char *filename) {
    FILE *fp = fopen(filename, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();

    png_infop info = png_create_info_struct(png);
    if (!info) abort();

    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_read_info(png, info);

    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if (bit_depth == 16)
        png_set_strip_16(png);

    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE) {
        LOGD("set filter");
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    } else {
        LOGD("SET no filter");
        png_set_filler(png, 0x00, PNG_FILLER_AFTER);
    }

//    png_init_filter_functions(png);

//    png_destroy_png_struct(png);

    png_init_filter_functions_neon(png,3);

//    png_read_filter_row(png,info,)

    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    fclose(fp);
}

void PngHelper::write_png_file(char *filename) {
    int y;

    FILE *fp = fopen(filename, "wb");
    if (!fp) abort();

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();

    png_infop info = png_create_info_struct(png);
    if (!info) abort();

    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
            png,
            info,
            width, height,
            8,
            PNG_COLOR_TYPE_RGBA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
    // Use png_set_filler().
    //png_set_filler(png, 0, PNG_FILLER_AFTER);

    png_write_image(png, row_pointers);
    png_write_end(png, NULL);

    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    fclose(fp);
}

void PngHelper::process_png_file() {
    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            // Do something awesome for each pixel here...
            //printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
        }
    }
}

