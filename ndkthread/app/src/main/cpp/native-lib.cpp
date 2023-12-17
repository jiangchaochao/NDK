#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>

#define TAG "jiangc"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, TAG,__VA_ARGS__)
JavaVM *g_vm;
jobject g_app_classloader = nullptr;   // 全局应用class loader
jmethodID g_loadclass_methodid = nullptr; // 全局loadclass 的方法id
jobject g_obj;    //MainObj;


void *thread_func(void *arg) {
    JNIEnv *env;
    if (g_vm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
        return nullptr;
    }
    jclass mainActivity = env->GetObjectClass(g_obj);
//    LOGE("mainActivity = %p\n", mainActivity);
//    jclass clzz = env->FindClass("java/lang/String");
//    LOGE("clzz = %p\n", clzz);
//    jclass jclass1 = env->FindClass("com/jiangc/ndkthread/MainActivity");
//    LOGE("jclass1 = %p\n", jclass1);
    jmethodID jmethodId = env->GetMethodID(mainActivity, "add", "(II)I");
    jint result = env->CallIntMethod(g_obj, jmethodId, 1,1);
    LOGE("result = %d\n", result);

    g_vm->DetachCurrentThread();
    return nullptr;
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_jiangc_ndkthread_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject obj /* this */) {
    std::string hello = "Hello from C++";

    jclass clzzzzz = env->FindClass("java/lang/String");
    LOGE("clzzzzz = %p\n", clzzzzz);
    jclass jclass1111 = env->FindClass("com/jiangc/ndkthread/MainActivity");
    LOGE("jclass1111 = %p\n", jclass1111);
    // 创建一个全局引用。
    g_obj = env->NewGlobalRef(obj);
    pthread_t tid;
    pthread_create(&tid, nullptr, thread_func, nullptr);
    return env->NewStringUTF(hello.c_str());
}


JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    // 获取全局的JavaVM
    g_vm = vm;
    JNIEnv *env;
    // 获取env
    if ((g_vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6)) != JNI_OK) {
        return JNI_ERR;
    }
    // 获取其中一个类的class，大多情况下一个应用的类加载器都是一个，如果不同，可以在这里多次获取
    jclass clazz = env->FindClass("com/jiangc/ndkthread/MainActivity");
    if (nullptr == clazz) {

        return JNI_ERR;
    }
    // 调用类的静态方法,获取方法id
    jmethodID jmethodId = env->GetStaticMethodID(clazz, "getAppClassLoader",
                                                 "()Ljava/lang/ClassLoader;");
    // 调用静态方法获取应用的class loader
    jobject obj = env->CallStaticObjectMethod(clazz, jmethodId);
    // 转化成全局引用，记得释放
    g_app_classloader = env->NewGlobalRef(obj);
    LOGE("g_app_classloader = %p\n", g_app_classloader);
    return JNI_VERSION_1_6;
}


JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    // 获取env
    if ((vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6)) != JNI_OK) {
        return;
    }
    if (nullptr != g_app_classloader) {
        env->DeleteGlobalRef(g_app_classloader);
    }
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_jiangc_ndkthread_MainActivity_isSameObject(JNIEnv *env, jobject thiz, jobject obj1) {
    // 使用IsSameObject函数检查两个对象是否相同
    jboolean result = env->IsSameObject(obj1, g_app_classloader);
    return result;

}