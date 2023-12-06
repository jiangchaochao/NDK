#include <jni.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "openssl/ssl.h"
#include "openssl/rand.h"
#include "MQTTClient.h"
#include "Logging.h"
// 定义是否使用加密
#define USE_SSL 1
#if USE_SSL
// 这个是临时申请的一个免费的测试服务器，自己部署的ssl有点问题
#define ADDRESS "ssl://ve96d599.ala.cn-hangzhou.emqxsl.cn:8883"
#else
// 本地连接，需要部署一个服务器
#define ADDRESS "mqtt://10.0.2.2:1883"
#endif

// 连接信息定义
#define USERNAME "test"
#define PASSWORD "test"
#define CLIENTID "c-client"
#define QOS 0
#define TOPIC "emqx/c-test"
#define TIMEOUT 10000L


MQTTClient client;


MQTTClient_SSLOptions configureSSLOptions(const char *path) {
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    SSL_library_init();
    MQTTClient_SSLOptions ssl_opts = MQTTClient_SSLOptions_initializer;
    ssl_opts.enableServerCertAuth = 1;
    ssl_opts.trustStore = path;
    ssl_opts.sslVersion = MQTT_SSL_VERSION_TLS_1_2;
    SSL_CTX  *ctx = SSL_CTX_new(TLSv1_2_client_method());
    SSL_CTX_set_cipher_list(ctx, "DEFAULT");
    SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
    ssl_opts.ssl_psk_context = ctx;
    return ssl_opts;
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_ecarx_mqtt_MqttManager_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_ecarx_mqtt_MqttManager_createAssetMqttClientSsl(JNIEnv *env, jobject thiz, jstring path) {
    int rc;
    const char *utf8 = env->GetStringUTFChars(path, NULL);
    ALOGD("path = %s\n", utf8);

    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, nullptr);
    conn_opts.username = USERNAME;
    conn_opts.password = PASSWORD;
#if USE_SSL
    MQTTClient_SSLOptions ssl_opts = configureSSLOptions(utf8);
    conn_opts.ssl = &ssl_opts;
#endif

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        ALOGE("Failed to connect, return code %d\n", rc);
        return false;
    }

    ALOGI("mqtt connect success!!!\n");





    env->ReleaseStringUTFChars(path, utf8);
    return true;
}