package com.glumes.instant

import android.Manifest
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.view.SurfaceHolder
import com.glumes.instantglsl.InstantRenderer
import kotlinx.android.synthetic.main.activity_main.*

class ImageGLSLActivity : AppCompatActivity() {


    lateinit var mInstantRenderer: InstantRenderer
    lateinit var mContext: Context
    lateinit var mShaderReceiver: ShaderReceiver
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

        PermissionsUtils.checkAndRequestMorePermissions(this, permisssion, 1, object : PermissionsUtils.PermissionRequestSuccessCallBack {
            override fun onHasPermission() {

            }
        })

        val path = Environment.getExternalStorageDirectory().toString() + "/texture.jpg"
        val shader = Environment.getExternalStorageDirectory().toString() + "/red_shader.glsl"

        surfaceview.holder.addCallback(object : SurfaceHolder.Callback {

            override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
//                val id = TextureHelper.loadTexture(mContext, R.drawable.texture)

//                mInstantRenderer.shaderChange(shader)

                mInstantRenderer.textureChange(path)

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
            Log.d("ShaderReceiver", "ShaderReceiver onReceive")
            val path = Environment.getExternalStorageDirectory().toString() + "/" + intent.getStringExtra("shaderPath")
            Log.d("ShaderReceiver", path)
            mInstantRenderer.shaderChange(path)
            mInstantRenderer.renderer()
        }
    }
}
