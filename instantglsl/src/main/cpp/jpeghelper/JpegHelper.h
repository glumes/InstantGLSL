//
// Created by glumes on 2019-07-01.
//

#ifndef INSTANTGLSL_JPEGHELPER_H
#define INSTANTGLSL_JPEGHELPER_H

#include <string>

#ifdef __cplusplus
extern "C"{
#endif
#include "jpeglib.h"
#include "jerror.h"
#ifdef __cplusplus
};
#endif
class JpegHelper {

public:
    int read_jpeg_file(const char* jpeg_file, unsigned char ** rgb_buffer,int * size,int *width,int *height);
};


#endif //INSTANTGLSL_JPEGHELPER_H
