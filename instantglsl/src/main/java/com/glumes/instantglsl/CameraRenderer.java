package com.glumes.instantglsl;

import android.view.Surface;

/**
 * Created by glumes on 17/09/2018
 */
public class CameraRenderer {

    static {
        System.loadLibrary("cameraglsl");
    }

    public native void onSurfaceCreated(Surface surface);

    public native void onSurfaceChanged(int width, int height);

    public native void onSurfaceDestroyed();

    public native void draw(int textureId, float[] matrix);

    public native int getTextureId();

    public native void swapBuffers();

    public native void shaderChange(String path);
}
