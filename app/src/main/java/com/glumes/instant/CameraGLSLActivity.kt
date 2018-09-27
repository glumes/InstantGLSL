package com.glumes.instant

import android.Manifest
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.graphics.SurfaceTexture
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.glumes.ezcamerakit.EzCamera
import com.glumes.ezcamerakit.EzCameraKit
import com.glumes.ezcamerakit.RequestOptions
import com.glumes.instantglsl.CameraRenderer

class CameraGLSLActivity : AppCompatActivity() {

    private val TAG = "CameraGLSLActivity"

    lateinit var mSurfaceView: SurfaceView
    var mCameraRenderer: CameraRenderer? = null

    var textureId = 0
    lateinit var mSurfaceTexture: SurfaceTexture

    var mCamera: EzCamera? = null
    lateinit var mCameraReceiver: CameraReceiver
    var mFloatArray: FloatArray = FloatArray(16)
    private val frameAvailableListener = SurfaceTexture.OnFrameAvailableListener {
        mSurfaceTexture.updateTexImage()
        mSurfaceTexture.getTransformMatrix(mFloatArray)
        mCameraRenderer?.draw(textureId, mFloatArray)
        mCameraRenderer?.swapBuffers()
    }

    private val callback = object : SurfaceHolder.Callback {
        override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
            mCameraRenderer?.onSurfaceChanged(width, height)
            mCamera!!.startPreview()
        }

        override fun surfaceDestroyed(holder: SurfaceHolder) {
            mCameraRenderer?.onSurfaceDestroyed()
            mCamera!!.stopPreview()
        }

        override fun surfaceCreated(holder: SurfaceHolder) {
            mCameraRenderer?.onSurfaceCreated(holder.surface)
            textureId = mCameraRenderer!!.textureId
            mSurfaceTexture = SurfaceTexture(textureId)
            mCamera = EzCameraKit.with(mSurfaceTexture).apply(RequestOptions.openBackCamera()).open()
            mSurfaceTexture.setOnFrameAvailableListener(frameAvailableListener)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_camera_glsl)

        PermissionUtil.checkAndRequestPermission(this, Manifest.permission.CAMERA, 1)

        mSurfaceView = findViewById(R.id.surfaceview)
        mCameraRenderer = CameraRenderer()
        mCameraReceiver = CameraReceiver()
        mSurfaceView.holder.addCallback(callback)
    }

    override fun onResume() {
        super.onResume()
        val filter = IntentFilter()
        filter.addAction(CAMERA_ACTION)
        filter.priority = Int.MAX_VALUE
        registerReceiver(mCameraReceiver, filter)
    }

    override fun onPause() {
        super.onPause()
        unregisterReceiver(mCameraReceiver)
    }

    inner class CameraReceiver : BroadcastReceiver() {

        override fun onReceive(context: Context?, intent: Intent) {
            val path = Environment.getExternalStorageDirectory().toString() + "/" + intent.getStringExtra("shaderPath")
            mCameraRenderer?.shaderChange(path)
        }
    }
}
