#  NDK开发关键点知识

#### **前言**

`本章内容为ndk开发中知识的重中之重，如果不理解这些内容，在实际开发中会遇到各种各样的bug。理解本章内容，有助于开发时避免相应的问题，以及出错时的一个解决思路，所以掌握本章内容是ndk开发的一个关键。`



#### 一. NDK开发之JavaVM

​	JNI定义了两个关键数据结构，一个是JavaVM，一个是JNIEnv,在前面我们看jni.h的时候也看到了，只是没有仔细查看他们的定义，这里先看第一个JavaVM，本质上，JavaVM是一个指向函数表的二级指针(在C++版本中，它们是一些类，这些类具有指向函数表的指针，并具有每个通过该函数表间接调用的JNI函数的成员函数。)JavaVM提供"调用接口"函数，我们可以利用此类的函数创建和销毁JavaVM。**理论上，每个进程可以有多个JavaVM，但在Android中只允许有一个。**

上面这段话为官方原话，但对于初学者来讲，可能还不是很好理解，所以我们就以官方所描述的内容进行讲解分析。

首先，说JavaVM本质上是一个二级指针，也就是我们说的指针的指针，要理解这些，可能还需要对指针有一些理解，不然你可能只知道怎么用，但不知道它到底是什么意思，很多初学者对于指针的理解不够，导致一些知识点无法彻底消化，这里我们简单描述一下这些概念。

**首先说指针**：概念，它其实就是内存地址，比如说我们有一个变量，存放在某个位置，那么这个位置的地址就是这个位置的指针，指向这个位置。举个最简单的例子就是，我们一般会在C语言中申请一块空间来存储东西，申请空间一般用malloc，这个函数返回了一个void *类型的返回值，返回的这个值就是这块空间的地址。

我们拿着这个地址就可以去操作这块空间了。

**指针变量**：指针变量首先是一个变量，它和我们平时定义的int，char，float，double等等变量没有任何区别，本身都是用来存数据的，只是int，char，float，double等存的是一个值，而指针变量存放的是一个地址，这个地址指向了一块空间。



理解了这两个概念，我们再说几个常见的概念：

函数指针：指向函数的指针，我们拿着这个指针就可以直接调用这个函数。

结构体指针：指向结构体的指针，我们拿着这个指针就可以操作结构体。

....

所以，简单理解，我们有指针就可以做相关的操作。

指针和指针的指针(所谓二级指针)：

指针，也叫一级指针，它指向的是直接可以操作的内存空间。

指针的指针，也叫二级指针，它指向的是一级指针，一级指针指向的是可以操作的内存空间。如下图，一级指针直接指向可操作的内存空间，指针的指针是指向了指针，然后通过指针再指向可操作的内存空间(图中的102也是一个指针)。

![image-20230813102620884](https://github.com/jiangchaochao/NDK/blob/main/Lesson4/doc/image-20230813102620884.png)

在了解了上面的基本概念之后，补充一下在C中和C++中调用上的一些区别，我们看一下jni.h

```c++
/*
 * JNI invocation interface.
 */
struct JNIInvokeInterface {
    void*       reserved0;
    void*       reserved1;
    void*       reserved2;

    jint        (*DestroyJavaVM)(JavaVM*);
    jint        (*AttachCurrentThread)(JavaVM*, JNIEnv**, void*);
    jint        (*DetachCurrentThread)(JavaVM*);
    jint        (*GetEnv)(JavaVM*, void**, jint);
    jint        (*AttachCurrentThreadAsDaemon)(JavaVM*, JNIEnv**, void*);
};

/*
 * C++ version.
 */
struct _JavaVM {
    const struct JNIInvokeInterface* functions;

#if defined(__cplusplus)
    jint DestroyJavaVM()
    { return functions->DestroyJavaVM(this); }
    jint AttachCurrentThread(JNIEnv** p_env, void* thr_args)
    { return functions->AttachCurrentThread(this, p_env, thr_args); }
    jint DetachCurrentThread()
    { return functions->DetachCurrentThread(this); }
    jint GetEnv(void** env, jint version)
    { return functions->GetEnv(this, env, version); }
    jint AttachCurrentThreadAsDaemon(JNIEnv** p_env, void* thr_args)
    { return functions->AttachCurrentThreadAsDaemon(this, p_env, thr_args); }
#endif /*__cplusplus*/
};


// 这里，可以看出来，对于C++来说，JavaVM是一个结构体(C++中结构体和C中的不太一样，C++中允许有函数，和类相似)，结构体中有很多的成员函数，这个类等于说是一个代理，它帮我们调用了JNIInvokeInterface中的函数。
#if defined(__cplusplus)
typedef _JNIEnv JNIEnv;
typedef _JavaVM JavaVM;
#else
// 而C语言版本中，JavaVM是一个JNIInvokeInterface*类型的指针变量，这里注意一下，正是因为这样，在C和C++中，才会出现使用上的差别。
typedef const struct JNINativeInterface* JNIEnv;
typedef const struct JNIInvokeInterface* JavaVM;
#endif

```



知道了他们的调用区别之后，再看一下JavaVM到底有什么用,那就要看他的函数了

```C++
/*
 * C++ version.
 */
struct _JavaVM {
    const struct JNIInvokeInterface* functions;

#if defined(__cplusplus)
    jint DestroyJavaVM()
        // 释放JavaVM
    { return functions->DestroyJavaVM(this); }
    // 将当前线程附着到虚拟机
    jint AttachCurrentThread(JNIEnv** p_env, void* thr_args)
    { return functions->AttachCurrentThread(this, p_env, thr_args); }
    // 讲当前线程和虚拟机分离
    jint DetachCurrentThread()
    { return functions->DetachCurrentThread(this); }
    // 获取ENV
    jint GetEnv(void** env, jint version)
    { return functions->GetEnv(this, env, version); }
    // AttachCurrentThreadAsDaemon()函数在JNI中的作用是将当前线程附加到Java虚拟机中作为一个守护线程，以便在非Java线程中调用Java API，并使得当所有非守护线程结束时，Java虚拟机可以退出。
    jint AttachCurrentThreadAsDaemon(JNIEnv** p_env, void* thr_args)
    { return functions->AttachCurrentThreadAsDaemon(this, p_env, thr_args); }
#endif /*__cplusplus*/
};
```



通过上面的注释可以看到，JavaVM提供了获取JNIEnv的函数，将当前线程附着到java虚拟机中以及分离等功能。

本质上就是和javaVM取得联系，使得我们可以和java端进行通信，因为虚拟机并不知道我们C/C++层的线程，所以他们不能直接通信，所以，需要将线程附着到虚拟机上，这样我们就可以获得虚拟机的环境，从而和Java端通信。



#### 二. NDK开发之JNIEnv

​	前面了解了JavaVM之后，我们再看一下JNIEnv，了解一下它是干什么的。同样的，看一下它的实现。

```c++
/*
 * C++ object wrapper.
 *
 * This is usually overlaid on a C struct whose first element is a
 * JNINativeInterface*.  We rely somewhat on compiler behavior.
 */
struct _JNIEnv {
    /* do not rename this; it does not seem to be entirely opaque */
    const struct JNINativeInterface* functions;

#if defined(__cplusplus)


	// ... 省略若干
    jstring NewString(const jchar* unicodeChars, jsize len)
    { return functions->NewString(this, unicodeChars, len); }

    jsize GetStringLength(jstring string)
    { return functions->GetStringLength(this, string); }

    const jchar* GetStringChars(jstring string, jboolean* isCopy)
    { return functions->GetStringChars(this, string, isCopy); }

    void ReleaseStringChars(jstring string, const jchar* chars)
    { functions->ReleaseStringChars(this, string, chars); }

    jstring NewStringUTF(const char* bytes)
    { return functions->NewStringUTF(this, bytes); }

    jsize GetStringUTFLength(jstring string)
    { return functions->GetStringUTFLength(this, string); }

    const char* GetStringUTFChars(jstring string, jboolean* isCopy)
    { return functions->GetStringUTFChars(this, string, isCopy); }

    void ReleaseStringUTFChars(jstring string, const char* utf)
    { functions->ReleaseStringUTFChars(this, string, utf); }

    jsize GetArrayLength(jarray array)
    { return functions->GetArrayLength(this, array); }

    jobjectArray NewObjectArray(jsize length, jclass elementClass,
        jobject initialElement)
    { return functions->NewObjectArray(this, length, elementClass,
        initialElement); }

    jobject GetObjectArrayElement(jobjectArray array, jsize index)
    { return functions->GetObjectArrayElement(this, array, index); }

    void SetObjectArrayElement(jobjectArray array, jsize index, jobject value)
    { functions->SetObjectArrayElement(this, array, index, value); }
	// ... 省略若干
#endif /*__cplusplus*/
};
```

看一下上面的函数，时不是很眼熟，就是前面我们讲过的常用函数的使用，所以JNIEnv的作用就是提供了我们和JavaVM通信的一些接口函数。

另外，它和JavaVM一样，也分别定义了C++和C的版本。原理也是一样的，在C++中通过一个结构体封装了一下，C中直接使用的是指针重命名的。所以调用上也和JavaVM一样。

```c
// C++中
env->xxxxx();
// C中(这里为什么要带*，是因为它是一个指针的指针)
(*env)->xxxxx();
```

#### 三. 全局引用和局部引用和弱全局引用

**局部引用：**

引用在C++和java以及一些面向对象的语言中都存在，对于传递给原生方法的每个参数，以及JNI函数返回的几乎每个对象都属于局部引用，这意味着，局部引用在当前线程中的当前原生方法运行期间有效。在原生方法返回后，即使对象本身继续存在，该引用也无效。

这适用于jobject的所有子类，包括jclass、jstring和jarray。
所有我们在函数中直接获取到的对象都是局部的，如果想让它在全局可用，需要将他变成全局引用。

**这个规则对于JNIEnv不适用，它属于线程。**



**全局引用：**局部引用在函数返回后便不能使用了，但如果想让局部引用在函数返回后依旧可以使用，就要将它变成全局的。

将一个局部引用变成全局引用可以通过NewGLobalRef方法,比如我们不想在每个地方都去获取java的class对象，就可以在一次获取后，将他变成全局的。

```cpp
jclass localClazz;
jclass globalClazz;

jclass localClass = env->FindClass("MyClass");
jclass globalClass = reinterpret_cast<jclass>(env->NewGlobalRef(localClass));

// TODO 记住，一定要记住，在不使用它的时候要进行释放，否则，内存泄漏
env->DeleteGlobalRef(globalClass);
```



以上两种是我们比较常用的，还有一种引用叫弱全局引用

**弱全局引用：**

它是全局引用的一种类型，与全局引用一样，它也可以在方法返回之后，依旧可以使用，但和全局引用不同的是弱全局引用不会阻止潜在的对象被垃圾回收，所以使用的时候记得检测这个对象是否还有效，避免崩溃。

```C++
jclass localClazz;
jclass weakGlobalClazz;

jclass localClass = env->FindClass("MyClass");
// 将局部引用变成弱全局引用
jclass weakGlobalClazz = reinterpret_cast<jclass>(env->NewWeakGlobalRef(localClass));

// 使用时检查,IsSameObject判断两个引用是否引用同一对象，必须使用 IsSameObject 函数，不能用 ==
// 这里和NULL比较，如果不同，说明有效
if (JNI_FALSE == env->IsSameObject(weakGlobalClazz, NULL))
{
    // 说明对象还存活，可以使用
}else{
    // 被回收了，不能再用了
}

```



#### 四. JNI_OnLoad

最后一个知识点，JNI_OnLoad,这个函数我们还没有使用过，这是一个被系统调用的函数，当库被加载的时候，系统会主动调用它。

那么它有什么用处？

一般情况下，我们在这个函数中获取JavaVM 对象保存到全局。 或者在这里动态注册JNI函数。再或者在这里获取一些类的class缓存起来。



```c++
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
        JNIEnv* env;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
            return JNI_ERR;
        }

        // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
        jclass c = env->FindClass("com/example/app/package/MyClass");
        if (c == nullptr) return JNI_ERR;

        // Register your class' native methods.
        static const JNINativeMethod methods[] = {
            {"nativeFoo", "()V", reinterpret_cast<void*>(nativeFoo)},
            {"nativeBar", "(Ljava/lang/String;I)Z", reinterpret_cast<void*>(nativeBar)},
        };
        int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
        if (rc != JNI_OK) return rc;

        return JNI_VERSION_1_6;
    }
```

以上就是今天所讲解的全部内容了。这章内容稍微偏理论，我们将在下节课讲解其他内容时用到上面的所有知识点，所以务必掌握。
