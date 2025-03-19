#include <jni.h>
#include <string>
#include "android/connectivity_manager.h"

// Hook点：ConnectivityManager.getAllNetworks()
extern "C" JNIEXPORT jobjectArray JNICALL
Java_android_net_ConnectivityManager_getAllNetworksFiltered(
        JNIEnv* env,
        jobject thiz) {
    // 原始方法调用
    jclass cls = env->GetObjectClass(thiz);
    jmethodID mid = env->GetMethodID(cls, "getAllNetworks", "()[Landroid/net/Network;");
    auto networks = (jobjectArray)env->CallObjectMethod(thiz, mid);

    // 过滤VPN类型网络
    jsize length = env->GetArrayLength(networks);
    jobjectArray filtered = env->NewObjectArray(length, env->FindClass("android/net/Network"), nullptr);
    
    int newIndex = 0;
    for (int i = 0; i < length; ++i) {
        jobject network = env->GetObjectArrayElement(networks, i);
        jint type = getNetworkType(env, network); // 自定义网络类型检测函数
        if (type != TYPE_VPN) {
            env->SetObjectArrayElement(filtered, newIndex++, network);
        }
    }
    return filtered;
}
