package com.ecarx.mqtt;

public class MqttManager {

    // Used to load the 'mqtt' library on application startup.
    static {
        System.loadLibrary("mqtt");
    }

    /**
     * A native method that is implemented by the 'mqtt' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}