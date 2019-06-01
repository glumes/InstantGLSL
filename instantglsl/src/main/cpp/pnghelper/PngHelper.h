//
// Created by glumes on 2019-06-01.
//

#ifndef INSTANTGLSL_PNGHELPER_H
#define INSTANTGLSL_PNGHELPER_H

#include <png.h>
#include <pngconf.h>
#include <string>

using namespace std;

class PngHelper {

private:
    const string file_name;
    unsigned char *data;
    unsigned int width, height;
    int bit_depth, color_type, interlace_type;
    int compression_type, filter_method;

public:

    PngHelper(const string &file_name);

    ~PngHelper();

    unsigned int getWidth();

    unsigned int getHeight();

    bool has_alpha();

    unsigned char *getData();

};


#endif //INSTANTGLSL_PNGHELPER_H
