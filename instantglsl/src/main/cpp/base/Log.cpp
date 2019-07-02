//
// Created by glumes on 2019-07-02.
//

#include "Log.h"

void Log::d(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    __android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG, msg, args);
    va_end(args);
}
