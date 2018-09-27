//
// Created by glumes on 2018/8/30.
//

#ifndef OPENGLNATIVE_OPENGL_UTIL_H
#define OPENGLNATIVE_OPENGL_UTIL_H

#include <jni.h>
#include <string>
#include <iostream>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>
#include <sstream>

#define OPENGL_TAG "OpenGLUtil"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,OPENGL_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,OPENGL_TAG,__VA_ARGS__)

namespace OpenGLUtil {

    GLuint loadShader(GLenum shaderType, const char *shader);

    GLuint createProgram(const char *vertexShader, const char *fragmentShader);

    GLuint loadTexture(int width, int height, unsigned char *pixels);

    GLuint createOESTexture();

    void checkGLError(const char *op);

    extern const char *DEFAULT_VERTEX_SHAER;

    extern const char *DEFAULT_TEXTURE_VERTEX_SHADER;

    extern const char *DEFAULT_TEXTURE_FRAGMENT_SHADER;

    extern const char *DEFAULT_COLOR_FRAGMENT_SHADER;

    extern const char *DEFAULT_OES_FRAGMENT_SHADER;

    extern GLfloat DEFAULT_VERTEX_COORDIANTE[8];
    extern GLfloat DEFAULT_TEXTURE_COORDIANTE[8];
}


#endif //OPENGLNATIVE_OPENGL_UTIL_H
