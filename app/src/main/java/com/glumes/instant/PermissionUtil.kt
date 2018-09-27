package com.glumes.instant

import android.app.Activity
import android.content.Context
import android.content.pm.PackageManager
import android.support.v4.app.ActivityCompat
import android.support.v4.content.ContextCompat


class PermissionUtil {

    companion object {
        fun checkPermission(context: Context, permission: String): Boolean {
            return ContextCompat.checkSelfPermission(context, permission) == PackageManager.PERMISSION_GRANTED
        }

        fun requestPermission(context: Context, permission: String, requestCode: Int) {
            ActivityCompat.requestPermissions(context as Activity, Array(1) { permission }, requestCode)
        }

        fun checkAndRequestPermission(context: Context, permission: String, requestCode: Int) {
            if (!checkPermission(context, permission)) {
                requestPermission(context, permission, requestCode)
            }
        }
    }


}