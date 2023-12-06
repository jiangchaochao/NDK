#include <jni.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "Logging.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_ecarx_mqtt_MqttManager_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_ecarx_mqtt_MqttManager_createAssetMqttClientSsl(JNIEnv *env, jobject thiz, jstring path) {
    const char *utf8 = env->GetStringUTFChars(path, NULL);
    ALOGD("path = %s\n", utf8);
    env->ReleaseStringUTFChars(path, utf8);

    return true;
}