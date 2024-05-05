
package com.example.native_app_test2


import android.content.res.AssetManager
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.native_app_test2.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private var binding: ActivityMainBinding? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding!!.root)

        // Call the native function to read the matrix
        readMatrix(assets)
    }

    companion object {
        init {
            System.loadLibrary("native_app_test2")
        }

        /**
         * A native method that is implemented by the 'native_app_test1' native library,
         * which is packaged with this application.
         */
        external fun readMatrix(assetManager: AssetManager)
    }
}