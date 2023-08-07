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

        add(1,2);
        byteTest((byte) 'a', (byte) 'b');
        charTest('a', '中');
        longTest(234, 123);
        floatTest(1.2f, 1.3f);
        doubleTest(1.234, 1.235);
        String hello_world = StringTest("hello world");
        Log.e(TAG, "onCreate:StringTest:    " + hello_world );
        int[] array = new int[10];
        for (int i = 0; i < 10; i++) {
            array[i] = i;
        }
        int[] ints = intArrayTest(array, 10);
        for (int anInt : ints) {
            Log.e(TAG, "onCreate: ints = " + anInt);
        }
    }

    /**
     * A native method that is implemented by the 'example1' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    // 基本类型操作
    public native void add(int a, int b);
    public native void byteTest(byte a, byte b);
    public native void charTest(char a, char b);
    public native void longTest(long a, long b);
    public native void floatTest(float a, float b);
    public native void doubleTest(double a, double b);

    // 引用类型操作
    // String
    public native String StringTest(String str);
    // array
    public native int[] intArrayTest(int[] array, int length);
    // object 这种这里先不讲，等讲到函数的时候我们再补充

}