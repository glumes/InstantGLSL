package com.glumes.instantglsl;

import android.view.Surface;

/**
 * Created by glumes on 06/09/2018
 */
public class InstantRenderer {

    static {
        System.loadLibrary("instantglsl");
    }

    public native void init(Surface surface);

    public native void renderer();

    public native void shaderChange(String path);

    public native void textureChange(int textureId);

    public native void textureChange(String path);

    public native void reportError();

    public native void destroy();
}
