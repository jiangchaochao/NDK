# Android 交叉编译Libusb

#### 环境

官网：<https://libusb.info/>

仓库：<https://github.com/libusb/libusb/tree/master>

    git clone https://github.com/libusb/libusb.git

环境：Ubuntu 22.04

NDK：26.1.10909125

#### 编译

    cd libusb
    cd android/jni
    # 找到自己的ndk目录，例如下面直接使用绝对路径
    ~/Android/Sdk/ndk/26.1.10909125/ndk-build

编译完成后，可以在android/libs下面找到编译好的库

编译好的库可以直接集成到Android中了。
