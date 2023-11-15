# MQTT 源码移植

#### 目标

在应用层使用mqtt库和平台通信。

#### 移植原因

mqtt java端接口匮乏，不能定制



#### 一. 源码下载

```shell
git clone https://github.com/eclipse/paho.mqtt.c.git
```

主要用的就是paho.mqtt.c  是C语言实现的一个库，也有Java的，本次移植主要是基于C端，原因就是更为灵活。



#### 二. 新建一个Android Studio工程

模块化考虑，在新建的工程里新建一个Android Native module，用于封装mqtt的库，提供java接口,目录结构如下图

<img src="C:\Users\chao.jiang\AppData\Roaming\Typora\typora-user-images\image-20231115193730943.png" alt="image-20231115193730943" align="left"/>

#### 三. 参考源码提供的Android.mk

<img src="C:\Users\chao.jiang\AppData\Roaming\Typora\typora-user-images\image-20231115182011223.png" alt="image-20231115182011223" align="left"/>

Android.mk的主要内容，对于应用层的移植，我们需要根据这个文件转换成CMakeList.txt

```shell

# 通用源文件列表，每个库都会依赖
libpaho-mqtt3_local_src_c_files_common := \
	$(libpaho-mqtt3_lib_path)/MQTTProtocolClient.c \
	$(libpaho-mqtt3_lib_path)/Tree.c \
	$(libpaho-mqtt3_lib_path)/Heap.c \
	$(libpaho-mqtt3_lib_path)/MQTTPacket.c \
	$(libpaho-mqtt3_lib_path)/Clients.c \
	$(libpaho-mqtt3_lib_path)/Thread.c \
	$(libpaho-mqtt3_lib_path)/utf-8.c \
	$(libpaho-mqtt3_lib_path)/StackTrace.c \
	$(libpaho-mqtt3_lib_path)/MQTTProtocolOut.c \
	$(libpaho-mqtt3_lib_path)/Socket.c \
	$(libpaho-mqtt3_lib_path)/Log.c \
	$(libpaho-mqtt3_lib_path)/Messages.c \
	$(libpaho-mqtt3_lib_path)/LinkedList.c \
	$(libpaho-mqtt3_lib_path)/MQTTPersistence.c \
	$(libpaho-mqtt3_lib_path)/MQTTPacketOut.c \
	$(libpaho-mqtt3_lib_path)/SocketBuffer.c \
	$(libpaho-mqtt3_lib_path)/MQTTPersistenceDefault.c \
	
# 下面是声明的文件列表，分别用于生成不同的库
# libpaho-mqtt3_local_src_c_files_c 文件列表
libpaho-mqtt3_local_src_c_files_c := \
	$(libpaho-mqtt3_lib_path)/MQTTClient.c \
# libpaho-mqtt3_local_src_c_files_cs 文件列表
libpaho-mqtt3_local_src_c_files_cs := \
	$(libpaho-mqtt3_lib_path)/MQTTClient.c \
	$(libpaho-mqtt3_lib_path)/SSLSocket.c \
# libpaho-mqtt3_local_src_c_files_a 文件列表
libpaho-mqtt3_local_src_c_files_a := \
	$(libpaho-mqtt3_lib_path)/MQTTAsync.c \
# libpaho-mqtt3_local_src_c_files_as 文件列表
libpaho-mqtt3_local_src_c_files_as := \
	$(libpaho-mqtt3_lib_path)/MQTTAsync.c \
	$(libpaho-mqtt3_lib_path)/SSLSocket.c \

#########################################  静态库  #####################################################
.......................................................................................................
#########################################  动态库  #####################################################

# libpaho-mqtt3c
include $(CLEAR_VARS)
LOCAL_MODULE    := libpaho-mqtt3c
LOCAL_SHARED_LIBRARIES := libdl
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/$(libpaho-mqtt3_lib_path)
LOCAL_C_INCLUDES:= $(libpaho-mqtt3_c_includes)
LOCAL_SRC_FILES := $(libpaho-mqtt3_local_src_c_files_common) $(libpaho-mqtt3_local_src_c_files_c)
include $(BUILD_SHARED_LIBRARY)
# libpaho-mqtt3cs
include $(CLEAR_VARS)
LOCAL_MODULE    := libpaho-mqtt3cs
LOCAL_SHARED_LIBRARIES := libcrypto libssl libdl
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/$(libpaho-mqtt3_lib_path)
LOCAL_C_INCLUDES:= $(libpaho-mqtt3_c_includes)
LOCAL_CFLAGS += -DOPENSSL
LOCAL_SRC_FILES := $(libpaho-mqtt3_local_src_c_files_common) $(libpaho-mqtt3_local_src_c_files_cs)
include $(BUILD_SHARED_LIBRARY)
# libpaho-mqtt3a
include $(CLEAR_VARS)
LOCAL_MODULE    := libpaho-mqtt3a
LOCAL_SHARED_LIBRARIES := libdl
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/${libpaho-mqtt3_lib_path}
LOCAL_C_INCLUDES:= $(libpaho-mqtt3_c_includes)
LOCAL_SRC_FILES := $(libpaho-mqtt3_local_src_c_files_common) $(libpaho-mqtt3_local_src_c_files_a)
include $(BUILD_SHARED_LIBRARY)
# libpaho-mqtt3as
include $(CLEAR_VARS)
LOCAL_MODULE    := libpaho-mqtt3as
LOCAL_SHARED_LIBRARIES := libcrypto libssl libdl
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/${libpaho-mqtt3_lib_path}
LOCAL_CFLAGS += -DOPENSSL
LOCAL_C_INCLUDES:= $(libpaho-mqtt3_c_includes)
LOCAL_SRC_FILES := $(libpaho-mqtt3_local_src_c_files_common) $(libpaho-mqtt3_local_src_c_files_as)
include $(BUILD_SHARED_LIBRARY)
```

基本上，Android.mk中的重点就是上面的描述了，包含了几个模块：

1. 通用源码列表
2. 不同库的源码列表
3. 生成不同库的依赖

库的作用如下：

paho-mqtt3a ： 一般实际开发中就是使用这个，a表示的是异步消息推送（asynchronous）。
paho-mqtt3as ： as表示的是 异步+加密（asynchronous+OpenSSL）。
paho-mqtt3c ： c 表示的应该是同步（Synchronize），一般性能较差，是发送+等待模式。
paho-mqtt3cs ： cs表示的是同步+加密（asynchronous+OpenSSL）。

转换成CMakeLists如下：这是最终的CMakeList.txt，具体的过程，请继续往下看

```cmake
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("mqtt")
message("openssl/${CMAKE_ANDROID_ARCH_ABI}/include")
# 包含子目录相关头文件
include_directories(mqttsrc)

# 通用源文件
set(libpaho-mqtt3_local_src_c_files_common
        mqttsrc/MQTTProtocolClient.c
        mqttsrc/Tree.c
        mqttsrc/Heap.c
        mqttsrc/MQTTPacket.c
        mqttsrc/Clients.c
        mqttsrc/Thread.c
        mqttsrc/utf-8.c
        mqttsrc/StackTrace.c
        mqttsrc/MQTTProtocolOut.c
        mqttsrc/Socket.c
        mqttsrc/Log.c
        mqttsrc/Messages.c
        mqttsrc/LinkedList.c
        mqttsrc/MQTTPersistence.c
        mqttsrc/SocketBuffer.c
        mqttsrc/MQTTPersistenceDefault.c
        mqttsrc/MQTTProperties.c
        mqttsrc/MQTTTime.c
        mqttsrc/MQTTPacketOut.c
        mqttsrc/WebSocket.c
        mqttsrc/Base64.c
        mqttsrc/Proxy.c
        mqttsrc/SHA1.c
        )

# libpaho-mqtt3_local_src_c_files_c
set(libpaho-mqtt3_local_src_c_files_c
        mqttsrc/MQTTClient.c
        )

# libpaho-mqtt3_local_src_c_files_cs
set(libpaho-mqtt3_local_src_c_files_cs
        mqttsrc/MQTTClient.c
        mqttsrc/SSLSocket.c
        )

# libpaho-mqtt3_local_src_c_files_a
set(libpaho-mqtt3_local_src_c_files_a
        mqttsrc/MQTTAsyncUtils.c
        mqttsrc/MQTTAsync.c
        )

# libpaho-mqtt3_local_src_c_files_as
set(libpaho-mqtt3_local_src_c_files_as
        mqttsrc/MQTTAsyncUtils.c
        mqttsrc/MQTTAsync.c
        mqttsrc/SSLSocket.c
        )
# 导入预编译库
# crypto
add_library(crypto SHARED IMPORTED)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/${CMAKE_ANDROID_ARCH_ABI}/libcrypto.so)
## openssl
add_library(openssl SHARED IMPORTED)
set_target_properties(openssl PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/${CMAKE_ANDROID_ARCH_ABI}/libssl.so)

add_library(libpaho-mqtt3c SHARED
        ${libpaho-mqtt3_local_src_c_files_common}
        ${libpaho-mqtt3_local_src_c_files_c}
        )
target_link_libraries(libpaho-mqtt3c dl)

add_library(libpaho-mqtt3cs SHARED
        ${libpaho-mqtt3_local_src_c_files_common}
        ${libpaho-mqtt3_local_src_c_files_cs}
        )
target_link_libraries(libpaho-mqtt3cs openssl dl)


add_library(libpaho-mqtt3a SHARED
        ${libpaho-mqtt3_local_src_c_files_common}
        ${libpaho-mqtt3_local_src_c_files_a}
        )
target_link_libraries(libpaho-mqtt3a dl)
#
add_library(libpaho-mqtt3as SHARED
        ${libpaho-mqtt3_local_src_c_files_common}
        ${libpaho-mqtt3_local_src_c_files_as}
        )
target_link_libraries(libpaho-mqtt3as openssl dl)
#
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        ${SOURCE_FILES}
        mqtt.cpp)
target_include_directories(mqtt PUBLIC openssl/${CMAKE_ANDROID_ARCH_ABI}/include)
#
target_link_libraries(mqtt
        libpaho-mqtt3c
        libpaho-mqtt3cs
        libpaho-mqtt3a
        libpaho-mqtt3as
        dl
        crypto
        openssl
        android
        log)
```



#### 四. openssl编译

保存下面内容为mkssl

```shell
#!/bin/sh

while getopts n:o:a:t:d:h: option
do
 case "${option}"
 in
 n) ANDROID_NDK=${OPTARG};;
 o) OPENSSL_VERSION=${OPTARG};;
 a) API_LEVEL=${OPTARG};;
 t) BUILD_TARGETS=${OPTARG};;
 d) OUT_DIR=${OPTARG};;
 h) HOST_TAG=${OPTARG};;
 *) twentytwo=${OPTARG};;
 esac
done

echo "ANDROID_NDK = $ANDROID_NDK"

echo "OPENSSL_VERSION= $OPENSSL_VERSION"
echo "API_LEVEL = $API_LEVEL"

echo "BUILD_TARGETS = $BUILD_TARGETS"

echo "OUT_DIR= $OUT_DIR"

echo "HOST_TAG = $HOST_TAG"

echo "twentytwo=$twentytwo"



BUILD_DIR=/tmp/openssl_android_build

if [ ! -d openssl-${OPENSSL_VERSION} ]
then
    if [ ! -f openssl-${OPENSSL_VERSION}.tar.gz ]
    then
        wget https://www.openssl.org/source/openssl-${OPENSSL_VERSION}.tar.gz || exit 128
    fi
    tar xzf openssl-${OPENSSL_VERSION}.tar.gz || exit 128
fi

cd openssl-${OPENSSL_VERSION} || exit 128


##### export ndk directory. Required by openssl-build-scripts #####
case ${OPENSSL_VERSION} in
    1.1.1*)
        export ANDROID_NDK_HOME=$ANDROID_NDK
    ;;
    *)
        export ANDROID_NDK_ROOT=$ANDROID_NDK
    ;;
esac

export PATH=$ANDROID_NDK/toolchains/llvm/prebuilt/$HOST_TAG/bin:$PATH

##### build-function #####
build_the_thing() {
    make clean
    ./Configure $SSL_TARGET -D__ANDROID_API__=$API_LEVEL && \
    make -j128 SHLIB_EXT=.so && \
    make install SHLIB_EXT=.so DESTDIR=$DESTDIR || exit 128
}

##### set variables according to build-tagret #####
for build_target in $BUILD_TARGETS
do
    case $build_target in
    armeabi-v7a)
        DESTDIR="$BUILD_DIR/armeabi-v7a"
        SSL_TARGET="android-arm"
    ;;
    x86)
        DESTDIR="$BUILD_DIR/x86"
        SSL_TARGET="android-x86"
    ;;
    x86_64)
        DESTDIR="$BUILD_DIR/x86_64"
        SSL_TARGET="android-x86_64"
    ;;
    arm64-v8a)
        DESTDIR="$BUILD_DIR/arm64-v8a"
        SSL_TARGET="android-arm64"
    ;;
    esac

    rm -rf $DESTDIR
    build_the_thing
#### copy libraries and includes to output-directory #####
    mkdir -p $OUT_DIR/$build_target/include
    cp -R $DESTDIR/usr/local/include/* $OUT_DIR/$build_target/include
    cp -R $DESTDIR/usr/local/ssl/* $OUT_DIR/$build_target/
    mkdir -p $OUT_DIR/$build_target/lib
    cp -R $DESTDIR/usr/local/lib/*.so $OUT_DIR/$build_target/lib
    cp -R $DESTDIR/usr/local/lib/*.a $OUT_DIR/$build_target/lib
done

echo Success

```

chmod 777 mkssl

执行命令编译: 例如

```shell
./mkssl -n /data/android/Sdk/ndk/25.2.9519653 -a 21 -t "armeabi-v7a" -o 1.1.1l -d ./ -h linux-x86_64
```

根据不同的需求更改-t 后面的参数,例如x86

```shell
./mkssl -n /data/android/Sdk/ndk/25.2.9519653 -a 21 -t "x86" -o 1.1.1l -d ./ -h linux-x86_64
```

脚本的参数含义如下：

-n： NDK的根目录

-a：ABI 的级别

-t：ABI

-o：openssl的版本

-d：输出目录

-h：host的架构



#### 五. 拷贝动态库到指定目录

参考第一张图，将openssl生成的动态库拷贝到mqtt目录下的libs中

<img src="C:\Users\chao.jiang\AppData\Roaming\Typora\typora-user-images\image-20231115194629348.png" alt="image-20231115194629348" align="left"/>

#### 六. 拷贝头文件

参考第一张图，将openssl生成的头文件拷贝到cpp中

<img src="C:\Users\chao.jiang\AppData\Roaming\Typora\typora-user-images\image-20231115194756254.png" alt="image-20231115194756254" align="left"/>

#### 七. 在cmake中包含这些头文件

关键语法如下：这句话的意思就是编译的时候根据不同的ABI去引用不同的头文件，我们这里包含了四个ABI的头文件

```cmake
target_include_directories(mqtt PUBLIC openssl/${CMAKE_ANDROID_ARCH_ABI}/include)
```

#### 八. 编译就行了

#### 九. 封装待续.......