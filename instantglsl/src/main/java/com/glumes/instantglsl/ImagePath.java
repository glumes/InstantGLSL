package com.glumes.instantglsl;

import android.os.Environment;

import java.io.File;

/**
 * @Author glumes
 */
public class ImagePath {
    public static final String TEXTURE_PATH = Environment.getExternalStorageDirectory().getAbsoluteFile().toString()
            + File.separator + "texture.jpg";
    public static final String JPG_IMAGE_PATH = Environment.getExternalStorageDirectory().getAbsoluteFile().toString()
            + File.separator + "image_output.jpg";
    public static final String PNG_IMAGE_PATH = Environment.getExternalStorageDirectory().getAbsoluteFile().toString()
            + File.separator + "logo.jpg";
}
