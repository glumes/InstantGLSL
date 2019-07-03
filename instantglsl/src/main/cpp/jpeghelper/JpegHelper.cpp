//
// Created by glumes on 2019-07-01.
//

#include "JpegHelper.h"

#include <stdio.h>
#include <setjmp.h>

struct my_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr *my_error_ptr;

void my_error_exit(j_common_ptr cinfo) {
    my_error_ptr myerr = (my_error_ptr) cinfo->err;

    (*cinfo->err->output_message)(cinfo);

    longjmp(myerr->setjmp_buffer, 1);
}

int
JpegHelper::read_jpeg_file(const char *jpeg_file, unsigned char **rgb_buffer, int *size, int *width,
                           int *height) {
    struct jpeg_decompress_struct cinfo;
    FILE *fp;

    JSAMPARRAY buffer;

    int row_stride = 0;

    unsigned char *tmp_buffer = nullptr;
    int rgb_size;
    fp = fopen(jpeg_file, "rb");
    if (fp == NULL) {
        printf("open file %s failed.\n", jpeg_file);
        return -1;
    }
//
//    cinfo.err = jpeg_std_error(&jerr.pub);
//    jerr.pub.error_exit = my_error_exit;

//    if (setjmp(jerr.setjmp_buffer))
//    {
//        jpeg_destroy_decompress(&cinfo);
//        fclose(fp);
//        return -1;
//    }

    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, fp);

    jpeg_read_header(&cinfo, TRUE);

    //cinfo.out_color_space = JCS_RGB; //JCS_YCbCr;  // 设置输出格式

    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    *width = cinfo.output_width;
    *height = cinfo.output_height;

    rgb_size = row_stride * cinfo.output_height; // 总大小
    *size = rgb_size;

    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    *rgb_buffer = (unsigned char *) malloc(sizeof(char) * rgb_size);    // 分配总内存

    printf("debug--:\nrgb_size: %d, size: %d w: %d h: %d row_stride: %d \n", rgb_size,
           cinfo.image_width * cinfo.image_height * 3,
           cinfo.image_width,
           cinfo.image_height,
           row_stride);
    tmp_buffer = *rgb_buffer;
    while (cinfo.output_scanline < cinfo.output_height) // 解压每一行
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        // 复制到内存
        memcpy(tmp_buffer, buffer[0], row_stride);
        tmp_buffer += row_stride;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(fp);

    return 0;
}

int JpegHelper::generateJpeg(u_int8_t *data, int w, int h, int quality, const char *location,
                             int quality1) {
    int nComponent = 3;
    // 存储信息的结构体
    struct jpeg_compress_struct jpegCompressStruct;

    // 错误设置
    struct my_error_mgr jem;
    // 绑定错误处理的结构对象
    jpegCompressStruct.err = jpeg_std_error(&jem.pub);

    if (setjmp(jem.setjmp_buffer)) {
        return 0;
    }

    FILE *fp = fopen(location, "wb");

    if (fp == nullptr) {
        return 0;
    }

    // 初始化结构信息
    jpeg_create_compress(&jpegCompressStruct);

    jpeg_stdio_dest(&jpegCompressStruct, fp);

    jpegCompressStruct.image_width = w;
    jpegCompressStruct.image_height = h;
    jpegCompressStruct.arith_code = false;
    jpegCompressStruct.input_components = nComponent;
    jpegCompressStruct.in_color_space = JCS_RGB;
    jpeg_set_defaults(&jpegCompressStruct);
    jpegCompressStruct.optimize_coding = quality;

    jpeg_set_quality(&jpegCompressStruct, quality, true);

    jpeg_start_compress(&jpegCompressStruct, true);

    JSAMPROW row_point[1];
    int row_stride = jpegCompressStruct.image_width * nComponent;

    while (jpegCompressStruct.next_scanline < jpegCompressStruct.image_height) {
        row_point[0] = &data[jpegCompressStruct.next_scanline * row_stride];
        jpeg_write_scanlines(&jpegCompressStruct, row_point, 1);
    }

    if (jpegCompressStruct.optimize_coding) {
        Log::d("use optimize");
    } else {
        Log::d("not use optimize");
    }

    jpeg_finish_compress(&jpegCompressStruct);
    jpeg_destroy_compress(&jpegCompressStruct);

    fclose(fp);

    return 1;
}

int JpegHelper::read_jpeg_file(const char *file) {

    FILE *fp = nullptr;
    int row_stride;
    JSAMPARRAY buffer;

    if (fopen(file, "rb") == nullptr) {
        Log::d("open file failed");
        return 0;
    }

    struct jpeg_decompress_struct cinfo;

    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);


    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, fp);

    jpeg_read_header(&cinfo, TRUE);

    Log::d("width is %d height is %d", cinfo.image_width, cinfo.image_height);

    jpeg_start_decompress(&cinfo);


//    row_stride = cinfo.output_width * cinfo.output_components;


    unsigned long width = cinfo.output_width;
    unsigned long height = cinfo.output_height;
    unsigned short depth = cinfo.output_components;
    row_stride = cinfo.output_width * cinfo.output_components;

    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    unsigned char *src_buff;
    src_buff = static_cast<unsigned char *>(malloc(width * height * depth));
    memset(src_buff, 0, sizeof(unsigned char) * width * height * depth);

    unsigned char *point = src_buff;

    while (cinfo.output_scanline < height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(point, *buffer, width * depth);
        point += width * depth;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    free(src_buff);

    fclose(fp);

    return 0;
}

int JpegHelper::write_jpeg_file(const char *filename, int image_height, int image_width,
                                JSAMPLE *image_buffer, int quality) {
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *outfile;

    JSAMPROW row_pointer[1];
    int row_stride;
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(filename, "wb")) == nullptr) {
        Log::d("open file failed");
        return 0;
    }

    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = image_width;
    cinfo.image_height = image_height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    row_stride = image_width * 3;
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    return 0;
}


