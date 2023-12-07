#include <jni.h>
#include <string>

jclass g_clazz;

jstring native_stringFromJNI(JNIEnv *env, jobject obj) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

jint native_add(JNIEnv *env, jobject thiz, jint a, jint b) {
    return a + b;
}

jint native_test(JNIEnv *env, jobject thiz) {
    return 666;
}

JNINativeMethod nativeMethods[] = {
        {"stringFromJNI", "()Ljava/lang/String;", (void *) native_stringFromJNI},
        {"add",           "(II)I",                (void *) native_add},
        {"test",          "()I",                  (void *) native_test}
};


JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if ((vm->GetEnv((void **) &env, JNI_VERSION_1_6)) != JNI_OK) {
        return JNI_ERR;
    }
    // 获得类的clazz
    g_clazz = env->FindClass("com/jiangc/ndkdr/MainActivity");
    env->RegisterNatives(g_clazz, nativeMethods, sizeof(nativeMethods) / sizeof(JNINativeMethod));
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if ((vm->GetEnv((void **) &env, JNI_VERSION_1_6)) != JNI_OK) {
        return;
    }
    if (nullptr != g_clazz)
        env->UnregisterNatives(g_clazz);
}