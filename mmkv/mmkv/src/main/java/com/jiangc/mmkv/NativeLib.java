package com.jiangc.mmkv;

public class NativeLib {

    // Used to load the 'mmkv' library on application startup.
    static {
        System.loadLibrary("mmkv");
    }

    /**
     * A native method that is implemented by the 'mmkv' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}