package com.jiangc.ndkthread;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.jiangc.ndkthread.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    // Used to load the 'ndkthread' library on application startup.
    static {
        System.loadLibrary("ndkthread");
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
        Log.e(TAG, "onCreate: " + getClassLoader());
        Log.e(TAG, "onCreate: " + getClassLoader().getParent());
        Log.e(TAG, "onCreate: " + getClassLoader().getParent().getParent());
        Log.e(TAG, "onCreate: " + isSameObject(MainActivity.class.getClassLoader()));
//
//        new Thread(() -> Log.e(TAG, "run: " + Thread.currentThread().getContextClassLoader())).start();

    }

    /**
     * A native method that is implemented by the 'ndkthread' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * 判断从java传递的classloader和jni中接收的是否是一个classloader
     *
     * @param obj1 MainActivity.class.getClassLoader()
     * @return true: 是一个，false：不是一个
     */
    public native boolean isSameObject(Object obj1);

    /**
     * 供jni调用获取应用的class loader
     *
     * @return class loader
     */
    public static ClassLoader getAppClassLoader() {
        return MainActivity.class.getClassLoader();
    }


    /**
     * 两数相加
     * @param a
     * @param b
     * @return
     */
    public  int add(int a, int b) {
        return a + b;
    }
}