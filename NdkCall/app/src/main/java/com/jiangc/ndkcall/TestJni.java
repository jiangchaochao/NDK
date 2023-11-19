package com.jiangc.ndkcall;

import android.util.Log;

public class TestJni {
    private static final String TAG = "TestJni";
    // Used to load the 'ndkcall' library on application startup.
    static {
        System.loadLibrary("ndkcall");
    }
    /**
     * A native method that is implemented by the 'ndkcall' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public static native void callStaticFunc();

    public native void findClass();

    public int add(int a, int b){
        Log.e(TAG, "add: " + a + "   " + b);
        return a + b;
    }

    public static int sAdd(int a, int b){
        Log.e(TAG, "sAdd: " + a + "    "+b );
        return a + b;
    }
}
