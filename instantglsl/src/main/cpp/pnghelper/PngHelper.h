//
// Created by glumes on 2019-06-01.
//

#ifndef INSTANTGLSL_PNGHELPER_H
#define INSTANTGLSL_PNGHELPER_H

#include <png.h>
#include <pngconf.h>
#include <string>
#include <pngpriv.h>

using namespace std;

class PngHelper {

private:
    const string mFileName;
    unsigned char *mPixelData;
    png_uint_32 mWidth, mHeight;
    int mBitDepth, mColorType, mInterlaceType;
    int mCompressionType, mFilterType;
    png_bytep *row_pointers;

public:

    PngHelper() = default;

    PngHelper(const string &file_name);

    ~PngHelper();

    unsigned int getWidth();

    unsigned int getHeight();

    bool has_alpha();

    unsigned char *getPixelData();

    void read_png_file(char *filename);

    void write_png_file(char *filename);

    void process_png_file();
};


#endif //INSTANTGLSL_PNGHELPER_H
