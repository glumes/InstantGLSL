//
// Created by glumes on 2018/9/3.
//

#ifndef OPENGLNATIVE_FILE_READ_H
#define OPENGLNATIVE_FILE_READ_H

#define  FILE_MAP_LOG_TAG    "FileObserver"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_INFO,FILE_MAP_LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,FILE_MAP_LOG_TAG,__VA_ARGS__)

#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <jni.h>
#include <string>

class FileFastMap {

private:
public:

    static const char *fastRead(const char *path);

};


#endif //OPENGLNATIVE_FILE_READ_H
