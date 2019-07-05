package com.glumes.instant.activity

import android.Manifest
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import com.glumes.instant.R
import com.glumes.instant.utils.PermissionsUtils
import com.glumes.instantglsl.demo.JpegDemo
import kotlinx.android.synthetic.main.activity_demo.*

class DemoActivity : AppCompatActivity(), View.OnClickListener {

    lateinit var mJpegDemo: JpegDemo
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_demo)
        val permisssion = Array(2) {
            Manifest.permission.READ_EXTERNAL_STORAGE
            Manifest.permission.WRITE_EXTERNAL_STORAGE
        }

        PermissionsUtils.checkAndRequestMorePermissions(this, permisssion, 1) { }

        jpegcompress.setOnClickListener(this)
        mJpegDemo = JpegDemo()
    }

    override fun onClick(v: View) {
        when (v.id) {
            R.id.jpegcompress -> mJpegDemo.compressBitmap()
        }
    }
}
