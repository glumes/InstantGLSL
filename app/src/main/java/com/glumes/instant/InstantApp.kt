package com.glumes.instant

import android.app.Application
import android.os.Environment
import io.reactivex.Observable
import io.reactivex.Scheduler
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.io.File
import java.io.FileOutputStream
import java.io.IOException

class InstantApp : Application() {

    override fun onCreate() {
        super.onCreate()
        LogUtil.setLog(true)
        Observable.fromCallable {
            copyAssertFileToSDCard()
            true
        }.subscribeOn(Schedulers.io()).observeOn(AndroidSchedulers.mainThread())
                .subscribe()
    }

    private fun copyAssertFileToSDCard() {

        LogUtil.d("copy Assert File To SDCard start")
        val pathPrefix = Environment.getExternalStorageDirectory().toString() + File.separator
        try {
            val fileNames = applicationContext.assets.list("files/")
            for (name in fileNames!!) {
                val path = pathPrefix + name
                val file = File(path)
                if (file.exists()) {
                    LogUtil.d("file is exist $name")
                    continue
                }
                val inputStream = applicationContext.assets.open("files/$name")
                val fos = FileOutputStream(file)
                val buffer = ByteArray(1024)
                var byteCount = 0
                while (inputStream.read().also { byteCount = it } != -1) {
                    fos.write(buffer, 0, byteCount)
                }
                fos.flush()
                inputStream.close()
                fos.close()
                LogUtil.d("move file : $name to $path")
            }
        } catch (e: IOException) {
            LogUtil.e(e.message)
        }

    }
}