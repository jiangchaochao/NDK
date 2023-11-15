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