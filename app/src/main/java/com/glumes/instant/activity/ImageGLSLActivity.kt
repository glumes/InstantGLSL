package com.glumes.instant.activity

import android.Manifest
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.view.SurfaceHolder
import com.glumes.instant.R
import com.glumes.instant.utils.IMAGE_ACTION
import com.glumes.instant.utils.PermissionsUtils
import com.glumes.instantglsl.InstantRenderer
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File

class ImageGLSLActivity : AppCompatActivity() {

    lateinit var mInstantRenderer: InstantRenderer
    lateinit var mContext: Context
    lateinit var mShaderReceiver: ShaderReceiver

    val assetPath3 = "texture/logo2.png"
    var sdPath = Environment.getExternalStorageDirectory().toString() + File.separator + "logo.png"
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        mInstantRenderer = InstantRenderer()
        mContext = this
        mShaderReceiver = ShaderReceiver()

        val permisssion = Array(2) {
            Manifest.permission.READ_EXTERNAL_STORAGE
            Manifest.permission.WRITE_EXTERNAL_STORAGE
        }

        PermissionsUtils.checkAndRequestMorePermissions(this, permisssion, 1) { }

        surfaceview.holder.addCallback(object : SurfaceHolder.Callback {

            override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {

//                mInstantRenderer.textureChangeFromeAsset(assetPath3, assets)
                mInstantRenderer.textureChagneFromSdcard(sdPath)

                mInstantRenderer.renderer()
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {

            }

            override fun surfaceCreated(holder: SurfaceHolder) {
                mInstantRenderer.init(holder.surface)
            }
        })

    }

    override fun onResume() {
        super.onResume()
        val filter = IntentFilter()
        filter.addAction(IMAGE_ACTION)
        filter.priority = Int.MAX_VALUE
        registerReceiver(mShaderReceiver, filter)
    }

    override fun onPause() {
        super.onPause()
        unregisterReceiver(mShaderReceiver)
    }

    inner class ShaderReceiver : BroadcastReceiver() {

        override fun onReceive(context: Context?, intent: Intent) {
            val path = Environment.getExternalStorageDirectory().toString() + "/" + intent.getStringExtra("shaderPath")
            mInstantRenderer.shaderChange(path)
            mInstantRenderer.renderer()
        }
    }
}
