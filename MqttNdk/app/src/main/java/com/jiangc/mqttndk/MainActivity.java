package com.jiangc.mqttndk;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.ecarx.mqtt.MqttManager;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        new Thread(() -> MqttManager.getInstance().createAssetMqttClientSsl(Utils.getInstance().getPath(MainActivity.this, "emqxsl-ca.crt"))).start();
    }
}