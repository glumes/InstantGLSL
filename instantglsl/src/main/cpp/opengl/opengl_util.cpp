//
// Created by glumes on 2018/8/30.
//

#include "opengl_util.h"

GLuint OpenGLUtil::loadShader(GLenum shaderType, const char *shader) {
    GLuint shader_handle = glCreateShader(shaderType);
    if (!shader_handle) {
        return 0;
    }

    glShaderSource(shader_handle, 1, &shader, NULL);

    glCompileShader(shader_handle);

    GLint compile_status = 0;

    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &compile_status);

    if (!compile_status) {

        GLint info_length = 0;

        glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &info_length);

        if (info_length) {
            char *info_log = (char *) malloc(sizeof(char) * info_length);
            if (info_log) {
                glGetShaderInfoLog(shader_handle, info_length, nullptr, info_log);
                LOGE("Error compiling shader:%s\n", info_log);
                free(info_log);
            }
        }
        glDeleteShader(shader_handle);
        shader_handle = 0;

    }
    return shader_handle;
}

GLuint OpenGLUtil::createProgram(const char *pVertexShader, const char *pFragmentShader) {

    GLuint shader_program = glCreateProgram();

    if (!shader_program) {
        return 0;
    }

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexShader);
    if (!vertexShader) {
        return 0;
    }

    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, pFragmentShader);
    if (!fragmentShader) {
        return 0;
    }

    glAttachShader(shader_program, vertexShader);
    glAttachShader(shader_program, fragmentShader);

    glLinkProgram(shader_program);

    GLint link_status = GL_FALSE;

    glGetProgramiv(shader_program, GL_LINK_STATUS, &link_status);

    if (!link_status) {
        GLint info_length = 0;
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_length);

        if (info_length) {
            char *info_log = (char *) malloc(sizeof(char) * info_length);
            glGetProgramInfoLog(shader_program, info_length, NULL, info_log);
            LOGE("Error linking program:%s\n", info_log);
            free(info_log);
        }
        glDeleteProgram(shader_program);
        shader_program = 0;
    }
    return shader_program;
}

GLuint OpenGLUtil::loadTexture(int width, int height, unsigned char *pixels) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    return textureId;
}

GLuint OpenGLUtil::createOESTexture() {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, textureId);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return textureId;
}


void OpenGLUtil::checkGLError(const char *op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGE("after %s() glError (0x%x)\n", op, error);
    }
}

/**
 * 顶点着色器
 */
const char *OpenGLUtil::DEFAULT_VERTEX_SHAER = "uniform mat4 uMVPMatrix; //总变换矩阵\n"
        "attribute vec4 aPosition;  //顶点位置\n"
        "uniform mat4 uSTMatrix;\n"
        "varying vec2 textureCoordinate;\n"
        "attribute vec4 inputTextureCoordinate;\n"
        "void main()\n"
        "{\n"
        "   gl_Position =  aPosition; //根据总变换矩阵计算此次绘制此顶点位置\n"
        "    textureCoordinate = (uSTMatrix * inputTextureCoordinate).xy;\n"
        "}";

const char *OpenGLUtil::DEFAULT_TEXTURE_VERTEX_SHADER = "uniform mat4 uMVPMatrix; //总变换矩阵\n"
        "attribute vec4 aPosition;  //顶点位置\n"
        "varying vec2 textureCoordinate;\n"
        "attribute vec2 inputTextureCoordinate;\n"
        "void main()\n"
        "{\n"
        "   gl_Position =  aPosition; //根据总变换矩阵计算此次绘制此顶点位置\n"
        "    textureCoordinate = inputTextureCoordinate;\n"
        "}";

/**
 * 片段着色器
 */
const char *OpenGLUtil::DEFAULT_TEXTURE_FRAGMENT_SHADER = "precision mediump float;\n"
        "varying vec2 textureCoordinate;//接收从顶点着色器过来的参数\n"
        "uniform sampler2D inputImageTexture;//纹理内容数据\n"
        "void main()\n"
        "{\n"
        "   //给此片元从纹理中采样出颜色值\n"
        "   gl_FragColor = texture2D(inputImageTexture, textureCoordinate);\n"
        "}";

const char *OpenGLUtil::DEFAULT_COLOR_FRAGMENT_SHADER = "precision mediump float;\n"
        "uniform vec4 u_Color\n;"
        "void main()\n"
        "{\n"
        "   gl_FragColor = u_Color;\n"
        "}";


const char *OpenGLUtil::DEFAULT_OES_FRAGMENT_SHADER =
        "#extension GL_OES_EGL_image_external : require\n"
                "precision mediump float;\n"
                "varying vec2 textureCoordinate;\n"
                "uniform samplerExternalOES inputImageTexture;\n"
                "void main()\n"
                "{\n"
                "   gl_FragColor = texture2D(inputImageTexture, textureCoordinate);\n"
                "}";


GLfloat OpenGLUtil::DEFAULT_VERTEX_COORDIANTE[8] = {
        -1, -1,
        1, -1,
        -1, 1,
        1, 1
};


GLfloat OpenGLUtil::DEFAULT_TEXTURE_COORDIANTE[8] = {

        0, 1,
        1, 1,
        0, 0,
        1, 0
};



