# 常用CMake 语法



#### 一. cmake 版本号声明

```cmake
# 声明一个我们使用的最小版本
cmake_minimum_required(VERSION 3.10.2)
```



#### 二. 设置项目名称

```cmake
# 这个项目名称一般和生成的库名称相同
project(test)
```



#### 三. 添加头文件搜索目录

```cmake
include_directories(../../../include)
```



#### 四. 添加源文件

```cmake
# 使用变量添加
set(SOURCES test.cpp  xxx.cpp)
# 添加所有
FILE(GLOB SRCS "*.CPP" "*.h")
```



#### 五. 生成一个库

```cmake
add_library( # Sets the name of the library.
        srt-lib         # 库的名字

        # Sets the library as a shared library.
        SHARED          # 动态库

        # Provides a relative path to your source file(s).
        srt-lib.cpp   srt1-lib.cpp)    # 源文件列表
```



#### 六. 搜索一个库(预构建库)

```cmake
find_library( # Sets the name of the path variable.
        log-lib   # 可以理解别名

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log    # 这个是liblog.so 在ndk目录中自带的一个库
        )
```



#### 七. 设置一个变量

```cmake
# 设置LIBDIR为 ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/${ANDROID_ABI}
set(LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/${ANDROID_ABI})
```



#### 八. 导入预构建库

```cmake
# 导入静态库
add_library(ssl STATIC IMPORTED)
set_target_properties(srt PROPERTIES IMPORTED_LOCATION ${LIBDIR}/libssl.a)
# 导入动态库
add_library(ssl SHARED IMPORTED)
set_target_properties(srt PROPERTIES IMPORTED_LOCATION ${LIBDIR}/libssl.so)
```





#### 九. 链接库

```cmake
target_link_libraries( # Specifies the target library.
        test

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
```



#### 十. 设置库的输出目录

```cmake
set_target_properties(test PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_${ARCH}})

```



