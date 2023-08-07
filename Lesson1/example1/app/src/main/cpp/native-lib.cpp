#include <jni.h>
#include <string>
#include <android/log.h>
#define TAG "jni"

#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__) // 定义LOGE类型


extern "C" JNIEXPORT jstring JNICALL
Java_com_jiangc_example1_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_jiangc_example1_MainActivity_add(JNIEnv *env, jobject thiz, jint a, jint b) {
    LOGE("a + b =  %d\n", a+b);
    return a + b;
}