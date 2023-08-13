#include <jni.h>

//
// Created by jiangc on 2023/8/13.
//

JNIEXPORT jstring JNICALL
Java_com_jiangc_example1_MainActivity_cTest(struct JNINativeInterface*   * env, jobject thiz) {
    const char * hello = "Hello from C";
    return (*env)->NewStringUTF(env, hello);
}