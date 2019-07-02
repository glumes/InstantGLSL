//
// Created by glumes on 2019-07-01.
//

#include "JpegHelper.h"

#include <stdio.h>



//typedef struct my_error_mgr * my_error_ptr;


//void my_error_exit (j_common_ptr cinfo)
//{
//    my_error_ptr myerr = (my_error_ptr) cinfo->err;
//
//    (*cinfo->err->output_message) (cinfo);
//
//    longjmp(myerr->setjmp_buffer, 1);
//}

int
JpegHelper::read_jpeg_file(const char *jpeg_file, unsigned char **rgb_buffer, int *size, int *width,
                           int *height) {
    struct jpeg_decompress_struct cinfo;
    FILE *fp;

    JSAMPARRAY buffer;

    int row_stride = 0;

    unsigned char*  tmp_buffer = nullptr;
    int rgb_size;
    fp = fopen(jpeg_file,"rb");
    if (fp == NULL)
    {
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

    *rgb_buffer = (unsigned char *)malloc(sizeof(char) * rgb_size);    // 分配总内存

    printf("debug--:\nrgb_size: %d, size: %d w: %d h: %d row_stride: %d \n", rgb_size,
           cinfo.image_width*cinfo.image_height*3,
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
