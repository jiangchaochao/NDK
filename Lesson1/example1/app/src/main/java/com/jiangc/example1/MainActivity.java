package com.jiangc.example1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.jiangc.example1.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    // Used to load the 'example1' library on application startup.
    static {
        System.loadLibrary("example1");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI());


        Log.e(TAG, "onCreate: a + b = " + add(1 ,2));
    }

    /**
     * A native method that is implemented by the 'example1' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * 两个数向加
     * @param a
     * @param b
     * @return
     */
    public native int add(int a, int b);
}