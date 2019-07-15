package com.glumes.instant.utils

import android.content.Context
import android.os.Environment
import java.io.File
import java.io.FileOutputStream
import java.io.IOException

class FileUtils {

    companion object {

        fun copyAssertFileToSDCard(context: Context) {

            LogUtil.d("copy Assert File To SDCard start")
            val pathPrefix = Environment.getExternalStorageDirectory().toString() + File.separator
            try {
                val fileNames = context.assets.list("files")
                if (fileNames.isEmpty()){
                    LogUtil.d("get asset file failed")
                }
                for (name in fileNames!!) {
                    val path = pathPrefix + name
                    val file = File(path)
                    if (file.exists()) {
                        LogUtil.d("file is exist $name")
                        continue
                    }
                    val inputStream = context.assets.open("files/$name")
                    val fos = FileOutputStream(file)
                    val buffer = ByteArray(1024)
                    var byteCount = 0
                    while (inputStream.read(buffer).also { byteCount = it } != -1) {
                        fos.write(buffer, 0, byteCount)
                    }
                    fos.flush()
                    inputStream.close()
                    fos.close()
                    LogUtil.d("move file : $name to $path")
                }
            } catch (e: IOException) {
                LogUtil.d(e.message)
            }

        }
    }

}