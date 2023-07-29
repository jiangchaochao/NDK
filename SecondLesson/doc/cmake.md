# Cmake常用语法讲解



#### 一. 补充第一讲不清楚的地方

**前言**

​		第一讲播出后，有部分朋友说有些地方一带而过不适合小白，还要自己去查，失去了扫盲的意义，这里做一个补充。

​		`当然了，所有的讲解都不可能适合所有人，扫盲也是一样，仅仅是引个路，想要学习的更深入，缺少不了自己的钻研与努力。`



#####  1.  预编译库和源码编译的库有什么不同吗？

​		答：没有什么不同，只是编译时机不同

##### 2. 什么是动态库

​		答： 动态库是一种可重复利用的代码和数据的集合，它可以在程序运行的时候动态的加在和连接到应用程序中。**在编译时动态库不会被连接到应用程序中**，而是在程序运行时被加载到内存中。**动态库的主要优点是它可以被多个程序共享**，并且可以在运行时更新或替换。这意味着一个**库被更新时，无需重新编译整个程序，只需要替换对应的库文件就可以了，这样可以降低程序维护的成本，提供更好的灵活性。**

##### 3. 什么是静态库

​		答： 静态库也是一种包含了可重复利用的代码和数据的集合，它和动态库最大的不同就是，动**态库在程序运行时，需要的时候才会加载链接，而静态库在编译的时候就会被直接链接到程序中，成为程序的一部分。**

相对动态库而言，静态库是独立存在的，动态库是共享的，这是他们最重要的区别之一，也正是因为这个原因，静态库相对于动态库而言，对其他的依赖更低。运行速度相对静态库会快一些，因为程序在执行过程中不需要再额外加载和链接动作。



##### 4. 什么时候用静态库，什么时候用动态库？

​		答： 一般情况下，可以从几个方面考虑：

​			a. 内存： 一些通用的库可以使用动态库，它是共享的，相对节省内存，比如log，utils

​			b. 存储：动态库只需要一份就可以了，所以依赖越多，相对空间节省越多

 			c. 环境: 如果是标准环境，比如pc,对于通用库一般依赖动态库(比如系统库)，但如果是一些环境情况不清楚，或者为了减少兼容性问题，则选择静态库

​			 d. 速度： 静态库运行速度比动态库快(理论上是，实际上基本察觉不出)



##### 5. ffmpeg 编译方式

```shell
#!/bin/bash

# 指定ndk路径
NDK=/usr/local/android-ndk-r21d

# 指定平台路径
PLATFORM=$NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot

# 指定交叉编译链
TOOLCHHAINS=$NDK/toolchains/llvm/prebuilt/linux-x86_64

#可变参数
API=""
ABI=""
ARCH=""
CPU=""
CC=""
CXX=""
CROSS_PREFIX=""
OPTIMIZE_CFLAGS=""
#关闭ASM:仅在X86架构上使用，实际使用发现--disable-x86asm并没有什么用，在Android API>= 23时还是会出现 has text relocations的问题，其他ABI没有问题，所以X86在编译的时候需要加上 --disable-asm
DISABLE_ASM=""
#输出路径
PREFIX=./android


API=21

function buildFF
{
	echo "开始编译ffmpeg $ABI"
	
	./configure \
		--prefix=$PREFIX/$ABI \
		--target-os=android \
		--cross-prefix=$CROSS_PREFIX \
		--arch=$ARCH \
		--cpu=$CPU \
		--sysroot=$PLATFORM \
		--extra-cflags="-I$PLATFORM/usr/include -fPIC -DANDROID -mfpu=neon -mfloat-abi=softfp $OPTIMIZE_CFLAGS" \
		--cc=$CC   \
		--ar=$AR   \
		--cxx=$CXX \
		--enable-shared \
		--enable-runtime-cpudetect \
		--enable-gpl \
		--enable-cross-compile \
		--enable-jni \
		--enable-mediacodec \
		--enable-decoder=h264_mediacodec \
		--enable-hwaccel=h264_mediacodec \
		--disable-x86asm \
		--disable-debug \
		--disable-static \
		--disable-doc \
		--disable-ffmpeg \
		--disable-ffplay \
		--disable-ffprobe \
		--disable-postproc \
		--disable-avdevice \
		--disable-symver \
		--disable-stripping \
		$DISABLE_ASM

		make -j4
		make install

	echo "编译结束"
}

# armv7-a 
function build_armv7()
{
	API=21
	ABI=armeabi-v7a
	ARCH=arm
	CPU=armv7-a
	CC=$TOOLCHHAINS/bin/armv7a-linux-androideabi$API-clang
	CXX=$TOOLCHHAINS/bin/armv7a-linux-androideabi$API-clang++
	CROSS_PREFIX=$TOOLCHHAINS/bin/arm-linux-androideabi-
	DISABLE_ASM=""
	# 编译
	buildFF
}


# armv8-a aarch64
function build_arm64()
{
	API=21
	ABI=arm64-v8a
	ARCH=arm64
	CPU=armv8-a
	CC=$TOOLCHHAINS/bin/aarch64-linux-android$API-clang
	CXX=$TOOLCHHAINS/bin/aarch64-linux-android$API-clang++
	CROSS_PREFIX=$TOOLCHHAINS/bin/aarch64-linux-android-
	OPTIMIZE_CFLAGS="-march=$CPU"
	DISABLE_ASM=""
	# 编译
	buildFF

}

# x86 i686

function build_x86()
{
	API=21
	ABI=x86
	ARCH=x86
	CPU=x86
	CC=$TOOLCHHAINS/bin/i686-linux-android$API-clang
	CXX=$TOOLCHHAINS/bin/i686-linux-android$API-clang++
	CROSS_PREFIX=$TOOLCHHAINS/bin/i686-linux-android-
	OPTIMIZE_CFLAGS="-march=i686  -mno-stackrealign"
	DISABLE_ASM="--disable-asm"
	# 编译
	buildFF

}

# x86_64
function build_x86_64()
{

	API=21
	ABI=x86_64
	ARCH=x86_64
	CPU=x86-64
	CC=$TOOLCHHAINS/bin/x86_64-linux-android$API-clang
	CXX=$TOOLCHHAINS/bin/x86_64-linux-android$API-clang++
	CROSS_PREFIX=$TOOLCHHAINS/bin/x86_64-linux-android-
	OPTIMIZE_CFLAGS="-march=$CPU"
	DISABLE_ASM=""
	# 编译
	buildFF
}


# all
function build_all()
{
	make clean
	build_armv7
	make clean
	build_arm64
	make clean
	build_x86
	make clean
	build_x86_64
}


# 编译全部
build_all

```





#### 二. CMake的常用语法

 ##### 1. 日志打印 message

```CMAKE

STATUS: 非常重要
WARNING： 警告
AUTHOR_WARNING:警告
SEND_ERROR: 错误，继续执行，但是会跳过生成的步骤
FATAL_ERROR： 错误，终止所有处理过程
message(TYPE "")
```



##### 2. 流程控制

在各种语言中都有流程控制语法，cmake也不例外

```cmake
################################## 首先说最常见的if else  ##################################
# 在讲if else之前，先看一下变量的使用
# 一般使用set(SET)命令声明一个变量
# 下面声明了一个变量BL 它的值是1
set(BL 1)
# 移除一个变量,这样，这个变量就不能用了
unset(BL)


# 下面这些都是代表true
true: 1, ON, YES, TRUE, Y,非0值
# 下面这些代表false
false： 0, OFF， NO， FALSE， N， IGNORE， NOTFOUND

# 比如下面定义一个变量
set(BT FALSE)
set(TEST TRUE)
if (${BT})
	message("if  BT is true")
elseif(${TEST})
	message("if TEST is true")
else()
	message("else else else")
endif() # 这里是结束，别忘了，其中elsif可以不要，直接结束




####################################### 再说 for循环 ######################################

# 在讲for循环之前还要插入讲一下数组的定义

# 下面定义了一个数组,从1 ～ 10
set(array_list 1 2 3 4 5 6 7 8 9 10)

# 第一种，常用的
foreach(i ${array_list})
	message(" i = ${i}")
endforeach()

# 第二种
foreach(i IN LISTS array_list)
	message(" i = ${i}")
endforeach()

# 第三种 RANGE
foreach(i RANGE 1 10 2) # 后面三个数字的意思是，从1～10的范围，每次步长为2, 打印1,3,5,7,9
	message(" i = ${i}")
endforeach()

# 第四种 RANGE
foreach(i RANGE 10)    # 如果只有一个数字，那就是从0开始，到10,打印0～10
	message(" i = ${i}")
endforeach()

# 第五种 直接列表型
foreach(i 1 2 3 4 5 6)   # 直接循环 1~6
	message(" i = ${i}")
endforeach()



#################################### 再说 while 循环 ######################################
格式:
#while(表达式)
#       COMMAND(ARGS...)
#endwhile(表达式)文章来源地址https://www.yii666.com/blog/366023.html

# 示例
while(NOT a STREQUAL "xxx")
	set(a "${a}x")
	message(">>>>>>a = ${a}")
endwhile()
```

#### 三. 多文件包含

对于普通的三方库，我们可能直接就给包含到一个cmake中了，但是有的三方库相当的庞大，并且会依赖其他库，而这些依赖库又是独立的，会独立更新，这时候我们为了方便控制，会使用多cmake文件的方式编译。

多文件方式编译关键点如下：

1. 注意依赖顺序，需要先编译被依赖的，然后再编译当前的库

关键语法：

```cmake
# 包含子目录CMakeLists,这个目录下必须要有CMakeLists.txt
add_subdirectory(test)

# 报行子目录相关头文件,这样才能在主库中使用相关的函数
include_directories(test/include)

# secondlesson 
target_link_libraries( # Specifies the target library.
        secondlesson

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        # 这里可以直接使用子目录生成的这个库
        test
)
```

详细可以参考demo

   

