//
// Created by jiangc on 6/12/23.
//

#ifndef MQTTNDK_LOGGING_H
#define MQTTNDK_LOGGING_H
#define TAG "MqttJni"

#include <android/log.h>

//定义TAG之后，我们可以在LogCat通过TAG过滤出NDK打印的日志
// 定义debug信息
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)

// 定义info信息
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)

// 定义warn信息
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG,__VA_ARGS__)

// 定义error信息
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

#endif //MQTTNDK_LOGGING_H
