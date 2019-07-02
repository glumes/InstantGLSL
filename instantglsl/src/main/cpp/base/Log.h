//
// Created by glumes on 2019-07-02.
//

#ifndef INSTANTGLSL_LOG_H
#define INSTANTGLSL_LOG_H

#include <jni.h>
#include <android/log.h>
#include <string>

#define LOG_TAG "instantglsl"

class Log {
public:
    static void d(const char *msg,...);
};


#endif //INSTANTGLSL_LOG_H
