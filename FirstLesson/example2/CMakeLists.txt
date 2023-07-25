cmake_minimum_required(VERSION 3.22.1)

project(test)
set(CMAKE_CXX_STANDARD 17)


# 设置NDK路径
set(ANDROID_NDK "/home/jiangc/Android/Sdk/ndk/25.1.8937393" CACHE PATH "Android NDK path")

# 设置架构，用来设置输出目录
set(ARCHS "armeabi-v7a" "arm64-v8a" "x86" "x86_64")


# 设置Android API级别
set(ANDROID_API_LEVEL 22)

# 添加每个架构的输出目录
foreach(ARCH ${ARCHS})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${ARCH} ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ARCH})
endforeach()

# 设置头文件
include_directories(.)

# 设置源文件
set(SOURCES test.cpp)


# 编译某一个架构的
function(build_library ARCH)
	set(CMAKE_ANDROID_ARCH_ABI ${ARCH})
    set(CMAKE_ANDROID_NDK ${ANDROID_NDK})
    set(CMAKE_SYSTEM_NAME Android)
    set(CMAKE_SYSTEM_VERSION ${ANDROID_API_LEVEL})
    add_library(test SHARED ${SOURCES})
    set_target_properties(test PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_${ARCH}})
endfunction()

message("---------------------------------------------   ${ANDROID_ABI}")

build_library(${ANDROID_ABI})


