# 关键函数之函数签名

#### 一. 基础知识

##### 1.1 访问域

Java有两类域：实例域和静态域。类的每个实例都有自己的实例域副本，而每个类的所有实例共享同一个静态域。

了解Java的都知道，在一个类加载器中，类的class只有一个，但可以有很多类的实例对象。所以在我们使用JNI访问Java的方法时，会有两种不同的方法。



##### 1.2 域ID

在前面章节中我们讲解了Java调用jni的方法，Java调用jni方法只需要注意函数的命名规则即可，但在jni访问Java的方法时需要获得访问域的ID，访问域有两种，同样，域ID也有两种。一种是静态域ID，一种是实例域ID,如下所示：

###### 1.2.1 获取类

1.2.1.1 用对象引用获得jclass

通过传入Java对象（jobject类型变量），获取Java类对象。

```c++
jclass clazz;
clazz = env->GetObjectClass(env, instance);
```

1.2.1.2 通过包名类名获取jclass

通过传入完整的包名/类名获取java类对应的C/C++环境下的jclass类型的变量，返回值：Java字节码class对象，对应C++中的jclass对象。注意：包名中的   .   使用   /   来替代

```c++
jclass clazz;
clazz = env->FindClass("com/jiangc/jni/TestJni");
```



##### 1.3 签名规则

以下是签名规则，在jni的环境中调用Java的方法或者获取Field时，需要用到签名，签名规则如下：

| java 类型             | 签名                   |
| --------------------- | ---------------------- |
| Boolean               | Z                      |
| Byte                  | B                      |
| Char                  | C                      |
| Short                 | S                      |
| Int                   | I                      |
| Long                  | J                      |
| Float                 | F                      |
| Double                | D                      |
| fully-qualified-class | Lfully-qualified-class |
| type[]                | [type                  |
| method type           | (arg-type)ret-type     |



例子：

```java
package com.example.javap;

public class TestJni {
    static {
        System.loadLibrary("javap");
    }

	// ()Ljava/lang/String
    public native String HelloWorld();


	// (II)I
    public int add(int a, int b){
        return a + b;
    }
	// ()Z
    public boolean isTrue(){
        return true;
    }

}
```



###### 1.3.1 使用javap生成函数签名

```shell
javac TestJni.java
javap -s -p -classpatch . TestJni

javap -s -p -classpath . TestJni
警告: 文件 .\TestJni.class 不包含类 TestJni
Compiled from "TestJni.java"
public class com.example.javap.TestJni {
  public com.example.javap.TestJni();
    descriptor: ()V

  public native java.lang.String HelloWorld();
    descriptor: ()Ljava/lang/String;

  public int add(int, int);
    descriptor: (II)I

  public boolean isTrue();
    descriptor: ()Z

  static {};
    descriptor: ()V
}
```



##### 1.4 设置Android Studio javap

打开Android  Studio ，File->Settings->Tools->External Tools扩展工具栏，如下图：

![image-20231119174245513](C:\Users\jiangchao\AppData\Roaming\Typora\typora-user-images\image-20231119174245513.png)

点击+号按钮

![image-20231119174704232](C:\Users\jiangchao\AppData\Roaming\Typora\typora-user-images\image-20231119174704232.png)

填写信息如下：

```shell
Program:  javap的绝对路径
Arguments：-s -p $FileClass$
Working directory: $OutputPath$
```

首先编译一下项目，然后通过右键javap获取函数签名

![image-20231119174910736](C:\Users\jiangchao\AppData\Roaming\Typora\typora-user-images\image-20231119174910736.png)

获取到的签名如下：

![image-20231119175155744](C:\Users\jiangchao\AppData\Roaming\Typora\typora-user-images\image-20231119175155744.png)

这样我们就可以相对比较快捷的得到函数签名了。



#### 二. 通过jni访问java方法

##### 2.1 调用实例对象的方法

```c++
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_javap_TestJni_HelloWorld(JNIEnv *env, jobject thiz) {

    jmethodID jmethodId;
    jclass clazz = env->GetObjectClass(thiz);
    if (nullptr == clazz) {
        LOGE("jclass is null\n");
    }
    jmethodId = env->GetMethodID(clazz, "add", "(II)I");
    jint result;
    result = env->CallIntMethod(thiz, jmethodId, 1, 2);
    LOGE("result = %d\n", result);
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_javap_TestJni_callStaticFunc(JNIEnv *env, jobject thiz) {

    jclass clazz;
    // com.example.javap
    clazz = env->FindClass("com/example/javap/TestJni");
    jmethodID jmethodId = env->GetStaticMethodID(clazz, "sAdd", "(II)I");

    jint result = env->CallStaticIntMethod(clazz, jmethodId, 1, 2);
    LOGE("call static result : %d\n", result);
}
```









