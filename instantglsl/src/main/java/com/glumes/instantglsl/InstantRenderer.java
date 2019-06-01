package com.glumes.instantglsl;

import android.content.res.AssetManager;
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

    public native void textureChangeFromeAsset(String assetPath,AssetManager assetManager);

    public native void textureChagneFromSdcard(String filePath);

    public native void reportError();

    public native void destroy();
}
