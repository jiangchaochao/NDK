# JNI 数据类型讲解和基础使用

#### 一. 为什么要讲这些东西

​		因为在jni中，数据类型和java中的数据类型是不同的，在了解java和jni类型之间的关系之后，我们才能更好的在java和native之间传递数据。



#### 二. 数据类型的分类

##### 2.1 基本类型

| java     | jni       | C/C++        | 大小           |
| -------- | --------- | ------------ | -------------- |
| boolean  | jboolean  | uint8_t      | 无符号8位      |
| byte     | jbyte     | int8_t       | 有符号8位      |
| **char** | **jchar** | **uint16_t** | **无符号16位** |
| short    | jshort    | int16_t      | 有符号16位     |
| int      | jint      | int32_t      | 有符号32位     |
| long     | jlong     | int64_t      | 有符号64位     |
| float    | jfloat    | float        | 32位           |
| double   | jdouble   | double       | 64位           |

##### 2.2 引用类型

| java                | jni           | C       | C++             |
| ------------------- | ------------- | ------- | --------------- |
| java.lang.Class     | jclass        | jobject | _jclass*        |
| java.lang.Throwable | jthrowable    | jobject | _jthrowable*    |
| java.lang.String    | jstring       | jobject | _jstring *      |
| Other objects       | jobject       | void *  | _jobject*       |
| java.lang.Object[]  | jobjectArray  | jarray  | _jobjectArray*  |
| boolean[]           | jbooleanArray | jarray  | _jbooleanArray* |
| byte[]              | jbyteArray    | jarray  | _jbyteArray*    |
| char[]              | jcharArray    | jarray  | _jcharArray*    |
| short[]             | jshortArray   | jarray  | _jshortArray*   |
| int[]               | jintArray     | jarray  | _jintArray*     |
| long[]              | jlongArray    | jarray  | _jlongArray*    |
| float[]             | jfloatArray   | jarray  | _jfloatArray*   |
| double[]            | jdoubleArray  | jarray  | _jdoubleArray*  |
| Other arrays        | Jarray        | jarray  | jarray*         |



对于引用类型，我们知道，在java中，所有类的父类都是java.long.Object,由于C语言中没有类的概念，所以在C语言中，使用void *代替，这是一个万能类型，在C++中定义了一个空的类 class _jobject {};来代替java中的类。



基本类型在jni.h中的定义：

```c
/* Primitive types that match up with Java equivalents. */
typedef uint8_t  jboolean; /* unsigned 8 bits */
typedef int8_t   jbyte;    /* signed 8 bits */
typedef uint16_t jchar;    /* unsigned 16 bits */
typedef int16_t  jshort;   /* signed 16 bits */
typedef int32_t  jint;     /* signed 32 bits */
typedef int64_t  jlong;    /* signed 64 bits */
typedef float    jfloat;   /* 32-bit IEEE 754 */
typedef double   jdouble;  /* 64-bit IEEE 754 */

/* "cardinal indices and sizes" */
typedef jint     jsize;
```





以下是使用C语言时jni.h中定义的数据类型

```c
/*
 * Reference types, in C.
 */
typedef void*           jobject;
typedef jobject         jclass;
typedef jobject         jstring;
typedef jobject         jarray;
typedef jarray          jobjectArray;
typedef jarray          jbooleanArray;
typedef jarray          jbyteArray;
typedef jarray          jcharArray;
typedef jarray          jshortArray;
typedef jarray          jintArray;
typedef jarray          jlongArray;
typedef jarray          jfloatArray;
typedef jarray          jdoubleArray;
typedef jobject         jthrowable;
typedef jobject         jweak;
```



以下是使用C++时jni.h定义的数据类型

```cpp
/*
 * Reference types, in C++
 */
class _jobject {};
class _jclass : public _jobject {};
class _jstring : public _jobject {};
class _jarray : public _jobject {};
class _jobjectArray : public _jarray {};
class _jbooleanArray : public _jarray {};
class _jbyteArray : public _jarray {};
class _jcharArray : public _jarray {};
class _jshortArray : public _jarray {};
class _jintArray : public _jarray {};
class _jlongArray : public _jarray {};
class _jfloatArray : public _jarray {};
class _jdoubleArray : public _jarray {};
class _jthrowable : public _jobject {};

typedef _jobject*       jobject;
typedef _jclass*        jclass;
typedef _jstring*       jstring;
typedef _jarray*        jarray;
typedef _jobjectArray*  jobjectArray;
typedef _jbooleanArray* jbooleanArray;
typedef _jbyteArray*    jbyteArray;
typedef _jcharArray*    jcharArray;
typedef _jshortArray*   jshortArray;
typedef _jintArray*     jintArray;
typedef _jlongArray*    jlongArray;
typedef _jfloatArray*   jfloatArray;
typedef _jdoubleArray*  jdoubleArray;
typedef _jthrowable*    jthrowable;
typedef _jobject*       jweak;
```



typedef 是为类型定义别名，从上面的代码来看，除了基本类型，在引用类型中，C最终是可以看成是void *,

而C++可以看成_jobject*。

全部写成void*    或者_jobject*    也是可以的,这里这么写主要是为了可读性和可扩展性。

#### 三. 字符集

在讲解数据类型的操作函数之前，先看一下字符集，为什么要讲解字符集，回到前面仔细观察一下基本类型的char，这个类型，在java中和C/C++中占用的空间是不同的。熟悉C的朋友都知道，C语言中的char类型占用的是一个字节，而在java中char是占用了两个字节的，而jchar也是占用了两个字节。这是因为java和C/C++使用的字符集不同，java使用的是Unicode字符集，而C使用的是ASCII字符集。



##### 3.1 Unicode字符集

​		Unicode是一个全球性的字符编码标准，它为世界上几乎所有的字符（包括各种文字、符号、标点符号等）都分配了唯一的代码点。每个代码点用十六进制表示，例如U+0041代表拉丁字母"A"，U+4E2D代表汉字"中"。Unicode字符集的目标是为了包含地球上所有的书写系统。

Unicode有不同的编码方案，最常见的是UTF-8、UTF-16和UTF-32。这些编码方案允许将Unicode代码点转换为字节序列以便存储和传输。

##### 3.2 ASCII字符集

​		ASCII字符集仅包含128个字符，包括英文字母、数字和一些特殊符号。这个字符集不足以表示全球范围内的所有字符，因此对于多语言支持和Unicode字符，C语言需要借助宽字符类型（wchar_t）和相关的函数。



所以，总的来说，Unicode字符集是一个包含全球字符的标准，可以表示各种语言和符号。而C中的char类型在默认情况下使用的是较小的字符集（通常是ASCII或其扩展），需要通过宽字符类型和函数来支持Unicode字符。要在C中完全支持Unicode字符集，推荐使用wchar_t和相关的宽字符函数。

由于上面的不同，所以在使用jchar的时候要格外注意。如果java中的字符串仅包含了ASCII的字符，可以直接将jchar转换成char使用，这是因为ASCII字符在Unicode中的表示和ASCII字符集中的表示是一致的。

但是，如果java中的字符串包含了非ASCII的字符，比如汉字(一个字节无法表示)，则需要注意字符编码的转换，一般这种情况可以使用java中的String类的getBytes方法将字符串转化为字节数组，指定为UTF-8字符集，然后在C中直接使用对应的字符集函数将字节数组转换成char数组。

所以，接下来我们看一下常用的几类数据类型的操作。

#### 四. 常用数据操作函数

##### 4.1 基本数据类型操作

###### 4.1.1 字符转换

```C
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
```



##### 4.2 字符串创建

```c
// 使用示例
jstring str = env->NewStringUTF("hello world");

// 函数含义
// 这个函数在正常情况下会返回一个jstring，可以直接返回给JAVA端使用。
// 使用时需要做判断，如下
if (NULL == str)
{
    // 出错了
}
// 其他深入的用法后面再说，这里先了解简单用法

```



##### 4.3 获取java端的字符串并转化成C/C++端字符串

```c
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

    // TODO: 这里注意，一定一定要记得释放，否则会内存泄漏
    // TODO: 即便返回的字符串直接指向java字符串的内部数据，也不要修改它，不然会导致一些不确定的行为。
    env->ReleaseStringUTFChars(str, cString);
```

函数的使用看上面的代码注释就可以了，需要注意的就是，在不使用的情况下，要对字符串进行释放，而且不要尝试修改获取到的字符串。

##### 4.4 获取字符串的长度

```c
// 这个视频里面没有讲，函数使用也很简单，看一下就知道了
jsize GetStringLength(jstring string);
// 这个函数返回一个jsize，也就是jint，也就是int，视频前面又讲这个类型关系

// 使用方式：
int length = env->GetStringLength(str);

```



##### 4.5 数组操作

注意：**对于数组，我们只讲一种类型，因为其他类型操作方式一样，大家可以自己尝试使用一下其他类型的数组函数**

###### 4.5.1 复制数组到native

```c
/**
 * 这个函数是将java端的数组内容复制到native端提供的数组中
 * @param array       java端数组
 * @param start       从哪里开始复制，比如从0开始
 * @param len         复制几个元素
 * @param buf         native端的数组地址
 */
void GetIntArrayRegion(jintArray array, jsize start, jsize len,
                       jint* buf);
// 使用也很简单

// TODO 1. 数组复制
    int *pArray = nullptr;
    pArray = static_cast<int *>(malloc(length * sizeof(int)));
    // 数组复制到了pArray中
    // 对应了 Get<Type>ArrayRegion
    env->GetIntArrayRegion(array, 0, length, pArray);

```

这样，就可以对数组进行操作了，但是，操作的修改不会被同步到java中，因为是复制的。

如果想将修改同步到java端，需要进行提交操作

###### 4.5.2 数组设置(将native中的数组数据同步到java)

```c
/**
 * 将native的数组内容设置到java端的数组中
 * @param array    被设置的java端的数组
 * @param start    从哪里开始设置
 * @param len      设置几个元素
 * @param buf      native的数组
 */
void SetIntArrayRegion(jintArray array, jsize start, jsize len,
                       const jint* buf);
    // 修改
    pArray[3] = 9;
    LOGE("after pArray[3] = %d\n", pArray[3]);
    // 如果想给数组提交到java,可以使用下面的方法
    // 对应了Set<Type>ArrayRegion
    env->SetIntArrayRegion(array, 0, length, pArray);
```

注意： **一般来讲，我们不会大量使用复制数组，然后将改变的数据再同步到java，这样效率太低，一般使用指针的方式进行操作。**



###### 4.5.3 获取java端数组指针

```c
/**
 * 获取java端数组指针
 * @param array 
 * @param isCopy 
 * @return 
 */
jint* GetIntArrayElements(jintArray array, jboolean* isCopy);
// 函数使用方法如下：
    // 对应了 Get<Type>ArrayElements
    jint *elements = env->GetIntArrayElements(array, nullptr);
    if (nullptr == elements) {
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
```

注意： **和string一样，凡是获取了指针的，一律记得释放，否则内存泄漏。**





所有函数都在jni.h中，这里先讲这么多常用的，大家也可以自己去看，后面用到什么我们再讲解什么。







