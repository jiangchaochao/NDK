# NDK 之函数的动态注册



#### 一. 基础知识

##### 1. JNI_Onload函数

JNI_Onload函数是在库加载的时候自动调用的一个函数，我们只需要实现它，就可以在库加载的时候做一些初始化动作，比如这里的函数动态注册。

```C
/*
 * Prototypes for functions exported by loadable shared libs.  These are
 * called by JNI, not provided by JNI.
 */
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved);
JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved);
```

##### 2.  如何使用JavaVM获取env

```c
// 参数：env，传出参数，ersion：jni的版本
// 作用：用于获取env
jint GetEnv(void** env, jint version)
```

##### 3. JNINativeMethod结构体

```c
typedef struct {
    const char* name;               // java函数名
    const char* signature;          // 函数签名
    void*       fnPtr;              // native函数指针
} JNINativeMethod;
```

##### 4. RegisterNatives函数和UnregisterNatives

```c
/**
 * 注册native函数
 * @param clazz      要注册的java的jclass
 * @param methods    要注册的方法数组
 * @param nMethods   方法的个数
 * @return  如果注册成功，返回 0。如果出现错误，返回一个非零值，代表注册失败的错误码。
 */
jint RegisterNatives(jclass clazz, const JNINativeMethod* methods, jint nMethods);
jint UnregisterNatives(jclass clazz)
```

#### 二. 实战demo

```c
#include <jni.h>

// 本地方法实现
void nativeMethod(JNIEnv *env, jobject obj) {
    // 实现本地方法的逻辑
}

// 定义本地方法数组
static JNINativeMethod methods[] = {
    { "nativeMethodName", "()V", (void *)nativeMethod }
};

// 在 JNI_OnLoad 中注册本地方法
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if ((vm->GetEnv(vm, (void **)&env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass clazz = env->FindClass("com/example/YourClass"); // 修改为你的类名
    if (clazz == NULL) {
        return JNI_ERR;
    }

    if (env->RegisterNatives(clazz, methods, 1) != 0) { // 注册一个本地方法
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

```

#### 三. 动态注册的好处

动态注册本地方法相对于静态注册（即在静态代码块中直接使用 `RegisterNatives` 注册方法）具有一些优势：

1. **灵活性：** 动态注册允许在运行时动态地注册本地方法。这意味着你可以根据特定条件或运行时的需求来注册本地方法，而不是在编译时固定地注册它们。这种灵活性在某些场景下非常有用，特别是当需要根据运行时条件来确定注册哪些方法时。
2. **模块化和动态加载：** 动态注册使得你可以根据需要动态加载不同的 JNI 模块，并在加载模块时注册相关的本地方法。这种模块化的设计允许你更灵活地管理和加载 JNI 功能，而无需在整个应用程序启动时将所有本地方法都注册。
3. **代码整洁性和可维护性：** 动态注册可以将本地方法的注册逻辑从静态代码块中抽离出来，使得代码更清晰和易于维护。通过将注册逻辑封装在单独的函数中，可以更好地组织代码并提高可读性。
4. **优化加载时间：** 如果应用程序具有多个 JNI 模块，并且不是所有的本地方法都在应用启动时需要，动态注册允许按需注册，可以减少启动时间并降低应用程序的内存占用。

尽管动态注册具有这些优势，但需要注意，对于特定的应用场景和项目结构，有时静态注册也可能更加方便和合适。选择使用哪种注册方法通常取决于项目需求、代码结构和维护方面的考量。



