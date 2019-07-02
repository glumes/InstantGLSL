//
// Created by glumes on 2019-07-01.
//

#ifndef INSTANTGLSL_JPEGHELPER_H
#define INSTANTGLSL_JPEGHELPER_H

#include <string>

#ifdef __cplusplus
extern "C" {
#endif
#include "jpeglib.h"
#include "jerror.h"
#ifdef __cplusplus
};
#endif

#include "../base/Log.h"

class JpegHelper {

public:
    int read_jpeg_file(const char *jpeg_file, unsigned char **rgb_buffer, int *size, int *width,
                       int *height);

    int generateJpeg(u_int8_t *data, int w, int h, int quality, const char *location,
                     int quality1);
};


#endif //INSTANTGLSL_JPEGHELPER_H
