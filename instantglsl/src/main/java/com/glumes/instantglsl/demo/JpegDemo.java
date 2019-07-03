package com.glumes.instantglsl.demo;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;

import com.glumes.instantglsl.ImagePath;

import java.io.File;

/**
 * @Author glumes
 */
public class JpegDemo {

    static {
        System.loadLibrary("thirdparty");
    }

    private native int compressBitmap(Bitmap bitmap, String file);

    private native int decompressBitmap(Bitmap bitmap);

    public void compressBitmap() {

        Bitmap bitmap = BitmapFactory.decodeFile(ImagePath.JPG_IMAGE_PATH);

        String path = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "compress.jpg";

        compressBitmap(bitmap,path);

    }
}
