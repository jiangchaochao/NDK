package com.jiangc.ndkcall;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.jiangc.ndkcall.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";


    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        TestJni jni = new TestJni();
        Log.e(TAG, "onCreate: " + jni.stringFromJNI());

        TestJni.callStaticFunc();

        jni.findClass();
    }


}