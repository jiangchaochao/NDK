package com.ecarx.mqtt;

import android.content.res.AssetManager;

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

    /**
     * 创建一个加密传输的mqtt client
     *
     * @param path 文件路径
     * @return 成功：true  失败：false
     */
    public native boolean createAssetMqttClientSsl(String path);


    private static class SingletonHolder {
        private static final MqttManager INSTANCE = new MqttManager();
    }


    public static MqttManager getInstance() {
        return SingletonHolder.INSTANCE;
    }


}