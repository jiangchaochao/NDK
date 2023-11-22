#  Opus 1.4 编译脚本

官网地址：<https://www.opus-codec.org/>

仓库地址：<https://gitlab.xiph.org/xiph/opus>

```bash
#!/bin/bash

# 每次编译删除原来的编译文件
rm build -rf
rm install -rf
# 创建临时编译目录，避免污染源文件
mkdir build
# 定义一个数组，存储架构类型，用来循环编译
ARCH_ARRAY=(armeabi-v7a arm64-v8a x86 x86_64)
mkdir install
cd build

for item in "${ARCH_ARRAY【@】}"; do
    mkdir -p install/$item
    echo "$item"
    echo "$ANDROID_HOME"
    rm * -rf
    cmake ..  \
        -DCMAKE_TOOLCHAIN_FILE=${ANDROID_HOME}/ndk/26.1.10909125/build/cmake/android.toolchain.cmake \
        -DANDROID_ABI=$item \
        -DOPUS_BUILD_SHARED_LIBRARY=y
    make -j8
    mkdir -p ../install/$item
    mv *.so  ../install/$item/
done

# 拷贝头文件到安装目录
cp ../include -rf ../install                                                                                     
                                                                                                                 

```
