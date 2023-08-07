#include <jni.h>
#include <string>
#include <jni.h>
#include <android/log.h>

#define TAG "jni"

#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__) // 定义LOGE类型
extern "C" JNIEXPORT jstring JNICALL
Java_com_jiangc_example1_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jiangc_example1_MainActivity_add(JNIEnv *env, jobject thiz, jint a, jint b) {

    LOGE("jint ================================   a = %d b = %d\n", a, b);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jiangc_example1_MainActivity_byteTest(JNIEnv *env, jobject thiz, jbyte a, jbyte b) {
    LOGE("jbyte =============================== a = %c b = %c\n", a, b);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_jiangc_example1_MainActivity_charTest(JNIEnv *env, jobject thiz, jchar a, jchar b) {
    int codePoint = (int) b;
    char utf8Char[4];
    if (codePoint < 0x80) {    // 128
        utf8Char[0] = (char) codePoint;
        utf8Char[1] = '\0';
    } else if (codePoint < 0x800) {    // 对于UTF-8编码来说，对于范围在0x80到0x7FF之间的Unicode字符用两个字节
        // 0x7ff
        utf8Char[0] = (char)(0xC0 | (codePoint >> 6));
        utf8Char[1] = (char)(0x80 | (codePoint & 0x3F));
        utf8Char[2] = '\0';
    }else{
        utf8Char[0] = (char)(0xE0 | (codePoint >> 12));
        utf8Char[1] = (char)(0x80 | ((codePoint >> 6) & 0x3F));
        utf8Char[2] = (char)(0x80 | (codePoint & 0x3F));
        utf8Char[3] = '\0';
    }
    LOGE("jchar ============================== a = %c  b = %s\n", a, utf8Char);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jiangc_example1_MainActivity_longTest(JNIEnv *env, jobject thiz, jlong a, jlong b) {
    LOGE("jlong ============================ aa = %lld bb = %lld\n", a, b);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jiangc_example1_MainActivity_floatTest(JNIEnv *env, jobject thiz, jfloat a, jfloat b) {
    LOGE("jfloat ======================================= a = %f  b = %f\n", a, b);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_jiangc_example1_MainActivity_doubleTest(JNIEnv *env, jobject thiz, jdouble a, jdouble b) {
    LOGE("jdouble ============================================  a = %f b = %f\n", a, b);
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_jiangc_example1_MainActivity_StringTest(JNIEnv *env, jobject thiz, jstring str) {
    /**
     * TODO 获取jstring转化成C字符串
     * string: 要转换的Java字符串（jstring对象）
     * isCopy: 一个可选的指向jboolean变量的指针。当isCopy不为NULL时，
     * 函数将通过它返回一个标志，表示返回的C字符串是从Java字符串拷贝而来（JNI_TRUE）
     * 还是直接指向Java字符串的内部数据（JNI_FALSE）。
     *
     * 函数返回值：
     * 如果成功，返回一个指向UTF-8编码的C字符串的指针。
     * 如果发生错误（如内存不足），返回NULL。
     *
     * TODO： 最终，这里调用的都是三个参数的 GetStringUTFChars(JNIEnv*, jstring, jboolean*);
     *        只是，封装了一次罢了，这里我们先不管env是什么，后面再讲，先使用两个参数的函数
     */
    const char *cString = env->GetStringUTFChars(str, NULL);
    if (NULL == cString) {
        LOGE("获取C字符串失败\n");
        return env->NewStringUTF("error");
    }
    // 打印
    LOGE("GetStringUTFChars: %s\n", cString);

    // TODO: 这里注意，一定一定要记得释放，否则会内存泄漏
    // TODO: 即便返回的字符串直接指向java字符串的内部数据，也不要修改它，不然会导致一些不确定的行为。
    env->ReleaseStringUTFChars(str, cString);
    return env->NewStringUTF("success");
}

extern "C"
JNIEXPORT jintArray JNICALL
Java_com_jiangc_example1_MainActivity_intArrayTest(JNIEnv *env, jobject thiz, jintArray array,
                                                   jint length) {
    // 对于数组，传到C/C++这里统一为引用类型
    // jni提供了两种方式访问数组
    // TODO 1. 数组复制
    int *pArray = nullptr;
    pArray = static_cast<int *>(malloc(length * sizeof(int)));
    // 数组复制到了pArray中
    // 对应了 Get<Type>ArrayRegion
    env->GetIntArrayRegion(array, 0, length, pArray);
    LOGE("begin pArray[3] = %d\n", pArray[3]);
    // 这个时候如果修改pArray中的元素，并不会影响到java
    pArray[3] = 9;
    LOGE("after pArray[3] = %d\n", pArray[3]);
    // 如果想给数组提交到java,可以使用下面的方法
    // 对应了Set<Type>ArrayRegion
    env->SetIntArrayRegion(array, 0, length, pArray);

    for (int i = 0; i < length; ++i) {
        LOGE("pArray[%d] = %d\n", i, pArray[i]);
    }
    // TODO：创建一个数组
    // 对应了 New<Type>Array
    jintArray javaArray = env->NewIntArray(length);
    if (nullptr == javaArray) {
        LOGE("数组创建失败，返回空");
        return nullptr;
    }
    LOGE("int 数组创建成功");
    // 在创建数组完成后我们还不能直接操作，需要获取到它的指针才行
    // 对应了 Get<Type>ArrayElements
    jint *elements = env->GetIntArrayElements(array, nullptr);
    if (nullptr == elements) {
        // 这里是删除这个引用，因为是局部的，所以用local，这里先不关心，后面会讲
        env->DeleteLocalRef(javaArray);
        return nullptr;
    }
    // 通过指针访问数组
    for (int i = 0; i < length; ++i) {
        elements[i] = i + 1;
    }

    // TODO 在获取到数组指针之后也要记得释放，否则会内存泄漏
    /**
     * 这个函数，前面两个参数一看就知道了，主要说明一下最后一个参数
     * mode: 释放数组元素时的标志
     * 主要有以下取值：
     * 0： 对Java的数组进行更新并释放C/C++的数组
     * JNI_COMMIT ：对Java的数组进行更新但是不释放C/C++的数组，一般用于周期性的修改java数组
     * JNI_ABORT：对Java的数组不进行更新,释放C/C++的数组
     */
     // 对应了 Release<Type>ArrayElements
    env->ReleaseIntArrayElements(javaArray, elements, 0);

    free(pArray);
    // 返回
    return javaArray;
}




