package com.jiangc.ndkdr;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.jiangc.ndkdr.databinding.ActivityMainBinding;


public class MainActivity extends AppCompatActivity {

    // Used to load the 'ndkdr' library on application startup.
    static {
        System.loadLibrary("ndkdr");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;
        tv.setText(stringFromJNI() + "   " + add(1,2) + "    " + test());
    }

    /**
     * A native method that is implemented by the 'ndkdr' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native int add(int a, int b);
    public native int test();
}