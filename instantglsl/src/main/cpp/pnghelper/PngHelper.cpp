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

#define PNG_BYTES_TO_CHECK 4

PngHelper::PngHelper(const string &file_name) :
        mFileName(file_name),
        mPixelData(nullptr),
        mWidth(0), mHeight(0),
        mBitDepth(0), mColorType(0),
        mInterlaceType(0),
        mCompressionType(0),
        mFilterType(0) {

    FILE *fp = fopen(mFileName.c_str(), "rb");
    if (fp == nullptr) {
        fclose(fp);
        return;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png == nullptr) {
        fclose(fp);
        return;
    }

    png_infop infop = png_create_info_struct(png);
    if (infop == nullptr) {
        png_destroy_read_struct(&png, nullptr, nullptr);
        fclose(fp);
        return;
    }


    png_init_io(png, fp);

    // 检查文件是否 png 格式
    char buf[PNG_BYTES_TO_CHECK];

    if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK) {
        return;
    }

    if (!png_sig_cmp(reinterpret_cast<png_const_bytep>(buf), 0, PNG_BYTES_TO_CHECK)) {
        LOGD("is png");
    } else {
        LOGD("is not png");
    };

    // 如果不执行这句就复位文件指针，否则会出现黑屏
    // rewind(fp);
    png_set_sig_bytes(png, PNG_BYTES_TO_CHECK);


    png_read_png(png, infop,
                 (PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND),
                 nullptr);

    infop->row_pointers;
    mWidth = png_get_image_width(png, infop);
    mHeight = png_get_image_height(png, infop);
    mColorType = png_get_color_type(png, infop);
    mBitDepth = png_get_bit_depth(png, infop);

    LOGD("width is %d,height is %d colortype is %d bitdepth is %d", mWidth, mHeight, mColorType,
         mBitDepth);


    png_get_IHDR(png, infop, &mWidth, &mHeight, &mBitDepth, &mColorType, &mInterlaceType,
                 &mCompressionType, &mFilterType);

//    unsigned int row_bytes = png_get_rowbytes(png, infop);
//    if (mPixelData != nullptr) {
//        delete[] mPixelData;
//        mPixelData = nullptr;
//    }
//    mPixelData = new unsigned char[row_bytes * mHeight];
//    png_bytepp rows = png_get_rows(png, infop);
//    for (int i = 0; i < mHeight; ++i) {
//        memcpy(mPixelData + (row_bytes * i), rows[i], row_bytes);
//    }

    row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * mHeight);
    for (int y = 0; y < mHeight; y++) {
        row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(png, infop));
    }

    png_read_image(png, row_pointers);

    png_destroy_read_struct(&png, &infop, nullptr);
};


PngHelper::~PngHelper() {
    if (mPixelData != nullptr) {
        delete[] mPixelData;
        mPixelData = nullptr;
    }
}

unsigned int PngHelper::getWidth() {
    return mWidth;
}

unsigned int PngHelper::getHeight() {
    return mHeight;
}

unsigned char *PngHelper::getPixelData() {
    return mPixelData;
}

bool PngHelper::has_alpha() {
    if (mColorType == PNG_COLOR_TYPE_RGBA) {
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

    mWidth = png_get_image_width(png, info);
    mHeight = png_get_image_height(png, info);
    mColorType = png_get_color_type(png, info);
    mBitDepth = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if (mBitDepth == 16)
        png_set_strip_16(png);

    if (mColorType == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if (mColorType == PNG_COLOR_TYPE_GRAY && mBitDepth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (mColorType == PNG_COLOR_TYPE_RGB ||
        mColorType == PNG_COLOR_TYPE_GRAY ||
        mColorType == PNG_COLOR_TYPE_PALETTE) {
        LOGD("set filter");
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    } else {
        LOGD("SET no filter");
        png_set_filler(png, 0x00, PNG_FILLER_AFTER);
    }

    if (mColorType == PNG_COLOR_TYPE_GRAY ||
        mColorType == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * mHeight);
    for (int y = 0; y < mHeight; y++) {
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

    if (setjmp(png_jmpbuf(png))){
        // 做一些清理工作
        LOGD("error happen");
        abort();
    }

    png_init_io(png, fp);

    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
            png,
            info,
            mWidth, mHeight,
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

    for (int y = 0; y < mHeight; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);

    fclose(fp);
}

void PngHelper::process_png_file() {
    for (int y = 0; y < mHeight; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < mWidth; x++) {
            png_bytep px = &(row[x * 4]);
            // Do something awesome for each pixel here...
            //printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
        }
    }
}

bool PngHelper::check_if_png(FILE *fp) {
    char buf[PNG_BYTES_TO_CHECK];
    if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK) {
        return false;
    }
    return png_sig_cmp(reinterpret_cast<png_const_bytep>(buf), 0, PNG_BYTES_TO_CHECK);
}

