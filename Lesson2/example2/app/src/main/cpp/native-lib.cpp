#include <jni.h>
#include <string>
#include "test.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_jiangc_secondlesson_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    int res = add(1, 2);
    LOGE("res = %d", res);
    return env->NewStringUTF(hello.c_str());
}