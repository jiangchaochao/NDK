#include <jni.h>
#include <string>
#include "test.h"

#include <android/log.h>
#define TAG "jni"

#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__) // 定义LOGE类型
extern "C" JNIEXPORT jstring JNICALL
Java_com_jiangc_example3_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    test t;
    int a = t.add(1,2);

    LOGE("t.add(1,2) = %d\n", a);

    return env->NewStringUTF(hello.c_str());
}