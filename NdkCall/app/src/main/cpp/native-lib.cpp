#include <jni.h>
#include <string>
#include <android/log.h>

#define TAG "jiangc"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, TAG,__VA_ARGS__)


extern "C"
JNIEXPORT jstring JNICALL
Java_com_jiangc_ndkcall_TestJni_stringFromJNI(JNIEnv *env, jobject thiz) {

    jclass clazz = env->GetObjectClass(thiz);
    jmethodID jmethodId = env->GetMethodID(clazz, "add", "(II)I");
    jint result = env->CallIntMethod(thiz, jmethodId, 1, 2);
    LOGE("not static call add: result = %d\n", result);


    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jiangc_ndkcall_TestJni_callStaticFunc(JNIEnv *env, jclass clazz) {
    jmethodID jmethodId = env->GetStaticMethodID(clazz, "sAdd", "(II)I");
    jint result = env->CallStaticIntMethod(clazz, jmethodId, 1, 3);
    LOGE("static call add: result = %d\n", result);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_jiangc_ndkcall_TestJni_findClass(JNIEnv *env, jobject thiz) {
    jclass clazz = env->FindClass("com/jiangc/ndkcall/TestJni");
    jmethodID jmethodId = env->GetMethodID(clazz, "add", "(II)I");
    jint result = env->CallIntMethod(thiz, jmethodId, 1, 5);
    LOGE("find class add result: %d\n", result);
}