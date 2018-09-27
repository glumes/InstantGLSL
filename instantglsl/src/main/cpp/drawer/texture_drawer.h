//
// Created by glumes on 2018/9/6.
//

#ifndef OPENGLNATIVE_TEXTURE_DRAWER_H
#define OPENGLNATIVE_TEXTURE_DRAWER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <opengl_util.h>



#define  TEXTURE_LOG_TAG    "OpenGLUtil"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_INFO,TEXTURE_LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,TEXTURE_LOG_TAG,__VA_ARGS__)

using namespace OpenGLUtil;

class TextureDrawer {

private:

    GLuint mProgram = 0;
    int mWidth;
    int mHeight;

    GLuint mTextureId;

    const GLfloat *mVertexCoordinates;
    const GLfloat *mTextureCoordinates;

    const char *mVertexShader;
    const char *mFragmentShader;

    GLint mPositionHandle;
    GLint mMVPMatrixHandle;
    GLint mTextureCoordHandle;
    GLint mTextureHandle;

    void createProgram();

    void getHandleInShader();

    void enableAttribArray();

    void disableAttribArray();

    void setHandleInShader();

    void bindTexture();

    void unBindTexture();

    void draw();

public:

    TextureDrawer();

    ~TextureDrawer();

    void onShaderChange(const char *shader);

    void onTextureChange(GLuint textureId);

    void renderer();

    void onSizeChange(int width, int height);
};


#endif //OPENGLNATIVE_TEXTURE_DRAWER_H
