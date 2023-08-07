#!/bin/bash

rm build -rf 
rm libs -rf

mkdir build
cd build

ANDROID_NDK="/home/jiangc/Android/Sdk/ndk/25.1.8937393"


ARCHS=('armeabi-v7a' 'arm64-v8a' 'x86' 'x86_64' )




function compile(){

	for i in ${ARCHS[@]}; do
		cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
			-DANDROID_ABI="$i" \
			-DANDROID_NDK=$ANDROID_NDK \
			-DANDROID_PLATFORM=android-22 \
			..
		make 
	done
}

compile

:<<!

cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
	-DANDROID_ABI="armeabi-v7a" \
	-DANDROID_NDK=$ANDROID_NDK \
	-DANDROID_PLATFORM=android-22 \
	..

make 

cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
	-DANDROID_ABI="arm64-v8a" \
	-DANDROID_NDK=$ANDROID_NDK \
	-DANDROID_PLATFORM=android-22 \
	..

make 

cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
	-DANDROID_ABI="x86" \
	-DANDROID_NDK=$ANDROID_NDK \
	-DANDROID_PLATFORM=android-22 \
	..

make 


cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
	-DANDROID_ABI="x86_64" \
	-DANDROID_NDK=$ANDROID_NDK \
	-DANDROID_PLATFORM=android-22 \
	..

make

!


