# MMKV 编译简略步骤

#### 1. 首先下载mmkv源码
```
git clone https://github.com/Tencent/MMKV.git
```

#### 2. 创建一个空的Android 工程
#### 3. 创建一个moudle，选择Android Native Livrary
#### 4. 将mmkv中的Core目录直接复制到新建的moudle中
#### 5. 修改CMakeList.txt
关键部分如下
```
# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("mmkv")

# 添加头文件,方便自己的源码中引用Core下的头文件
include_directories(Core)

# 添加mmkv的子CMakeList,在编译时会首先执行子目录的CMakeList
add_subdirectory(Core)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        # 添加源文件，这个源文件可加可不加，随意
        libmmkv.cpp
        mmkv.cpp)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        log)
```

然后直接编译项目即可，具体封装可按照mmkv的api文档进行。详细内容可直接给项目导入Android studio查看
