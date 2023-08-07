//
// Created by jiangc on 2023/7/29.
//

#ifndef SECONDLESSON_TEST_H
#define SECONDLESSON_TEST_H
#include <jni.h>
#include <android/log.h>
#define TAG "test-jni"

#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__) // 定义LOGE类型

#ifdef __cplusplus
extern "C"{
#endif

int add(int a, int b);
#ifdef __cplusplus
}
#endif
#endif //SECONDLESSON_TEST_H
