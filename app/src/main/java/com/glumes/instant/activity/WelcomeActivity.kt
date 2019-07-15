package com.glumes.instant.activity

import android.Manifest
import android.content.Intent
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import com.glumes.instant.R
import com.glumes.instant.utils.FileUtils
import com.glumes.instant.utils.PermissionsUtils
import kotlinx.android.synthetic.main.activity_welcome.*

class WelcomeActivity : AppCompatActivity() {


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_welcome)

        val permisssion = Array(2) {
            Manifest.permission.READ_EXTERNAL_STORAGE
            Manifest.permission.WRITE_EXTERNAL_STORAGE
        }

        PermissionsUtils.checkAndRequestMorePermissions(this, permisssion, 1) {
            FileUtils.copyAssertFileToSDCard(this)
        }

        FileUtils.copyAssertFileToSDCard(this)

        imageglsl.setOnClickListener {
            startActivity(Intent(this,ImageGLSLActivity::class.java))
        }
    }
}
