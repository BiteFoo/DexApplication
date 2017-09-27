//
// Created by John.Lu on 2017/9/26.
//

#ifndef DEXAPPLICATION_MLOG_H
#define DEXAPPLICATION_MLOG_H

#include <android/log.h>
#define TAG "CHANGECODE"
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define ALOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型
#define ALOGV(...) __android_log_print(ANDROID_LOG_DEFAULT,TAG ,__VA_ARGS__) // 定义LOGF类型
//ALOGV
//LOG_PRI
#define LOG_PRI(...) __android_log_print(ANDROID_LOG_DEFAULT,TAG ,__VA_ARGS__) // 定义LOGF类型
//LOG_PRI_VA
#define LOG_PRI_VA(...) __android_log_print(ANDROID_LOG_DEFAULT,TAG ,__VA_ARGS__) // 定义LOGF类型
#endif //DEXAPPLICATION_MLOG_H
