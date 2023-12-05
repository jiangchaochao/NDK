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
## build options
# 是否开启OPENSSL
SET(PAHO_WITH_SSL TRUE CACHE BOOL "Flag that defines whether to build ssl-enabled binaries too. ")
# 是否开启动态库
SET(PAHO_BUILD_SHARED TRUE CACHE BOOL "Build shared library")
# 是否开启静态库
SET(PAHO_BUILD_STATIC FALSE CACHE BOOL "Build static library")

# 包含子目录相关头文件
include_directories(mqttsrc)
include_directories(openssl/${CMAKE_ANDROID_ARCH_ABI}/include/)

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
# android 需要连接的库
SET(LIBS_SYSTEM c dl)

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

# 如果开启了动态库
IF (PAHO_BUILD_SHARED)
    # 定义一个OBJECT 库，提高编译速度
    ADD_LIBRARY(common_obj OBJECT ${libpaho-mqtt3_local_src_c_files_common})
    SET_TARGET_PROPERTIES(common_obj PROPERTIES
            POSITION_INDEPENDENT_CODE ON
            COMPILE_DEFINITIONS "PAHO_MQTT_EXPORTS=1")
ENDIF ()

# paho-mqtt3c、paho-mqtt3a
IF (PAHO_BUILD_SHARED)
    # libpaho-mqtt3c
    ADD_LIBRARY(paho-mqtt3c SHARED $<TARGET_OBJECTS:common_obj> ${libpaho-mqtt3_local_src_c_files_c})
    # libpaho-mqtt3a
    ADD_LIBRARY(paho-mqtt3a SHARED $<TARGET_OBJECTS:common_obj> ${libpaho-mqtt3_local_src_c_files_a})
    # 连接库
    TARGET_LINK_LIBRARIES(paho-mqtt3c ${LIBS_SYSTEM})
    TARGET_LINK_LIBRARIES(paho-mqtt3a ${LIBS_SYSTEM})
    # 设置库的属性
    SET_TARGET_PROPERTIES(
            paho-mqtt3c paho-mqtt3a PROPERTIES
            VERSION "1.3.13"
            SOVERSION "1"
            COMPILE_DEFINITIONS "PAHO_MQTT_EXPORTS=1")
ENDIF ()

# 如果开启了openssl
IF (PAHO_WITH_SSL)
    IF (PAHO_BUILD_SHARED)
        # 重新编译一个带SSL的OBJECT库
        ADD_LIBRARY(common_ssl_obj OBJECT ${libpaho-mqtt3_local_src_c_files_common})
        # 包含头文件
        TARGET_INCLUDE_DIRECTORIES(common_ssl_obj PUBLIC openssl/${CMAKE_ANDROID_ARCH_ABI}/include/)
        # 设置属性
        SET_PROPERTY(TARGET common_ssl_obj PROPERTY POSITION_INDEPENDENT_CODE ON)
        SET_PROPERTY(TARGET common_ssl_obj PROPERTY COMPILE_DEFINITIONS "OPENSSL=1;PAHO_MQTT_EXPORTS=1")
        ADD_LIBRARY(paho-mqtt3cs SHARED $<TARGET_OBJECTS:common_ssl_obj> ${libpaho-mqtt3_local_src_c_files_cs})
        ADD_LIBRARY(paho-mqtt3as SHARED $<TARGET_OBJECTS:common_ssl_obj> ${libpaho-mqtt3_local_src_c_files_as})
        SET_TARGET_PROPERTIES(
                paho-mqtt3cs paho-mqtt3as PROPERTIES
                VERSION "1.3.13"
                SOVERSION "1"
                COMPILE_DEFINITIONS "OPENSSL=1;PAHO_MQTT_EXPORTS=1")
        FOREACH(TARGET paho-mqtt3cs paho-mqtt3as)
            TARGET_LINK_LIBRARIES(${TARGET}
                    PUBLIC
                    openssl crypto ${LIBS_SYSTEM})
        ENDFOREACH()
    ENDIF ()
ENDIF ()

add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        ${SOURCE_FILES}
        mqtt.cpp)
target_include_directories(mqtt PUBLIC openssl/${CMAKE_ANDROID_ARCH_ABI}/include)
#
target_link_libraries(mqtt
        paho-mqtt3c
        paho-mqtt3cs
        paho-mqtt3a
        paho-mqtt3as
        ${LIBS_SYSTEM}
        crypto
        openssl
        android
        log)