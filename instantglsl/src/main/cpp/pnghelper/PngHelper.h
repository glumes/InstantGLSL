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
    int mWidth, mHeight;
    int mBitDepth, mColorType, mInterlaceType;
    int mCompressionType, mFilterType;
    png_bytep *row_pointers;

public:

    PngHelper() = default;

    PngHelper(const string &file_name) :
            mFileName(file_name),
            mPixelData(nullptr),
            mWidth(0), mHeight(0),
            mBitDepth(0), mColorType(0),
            mInterlaceType(0),
            mCompressionType(0),
            mFilterType(0){

//        FILE *fp = fopen(mFileName.c_str(), "rb");
//        if (fp == nullptr){
//            fclose(fp);
//            return ;
//        }

    };

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
