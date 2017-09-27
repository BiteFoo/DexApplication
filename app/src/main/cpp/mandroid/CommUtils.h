//
// Created by John.Lu on 2017/9/26.
//

#ifndef DEXAPPLICATION_COMMUTILS_H
#define DEXAPPLICATION_COMMUTILS_H

#include <jni.h>
#ifndef  PAGESIZE
#define  PAGESIZE PAGE_SIZE
#endif
#define  PAGE_MASK (~(PAGE_SIZE) -1)
#define  PAGE_START (x) ((x) & PAGE_MASK)
#define  PAGE_OFFSET (x) ((x) ~PAGE_MASK)
#define  PAGE_END (x) PAGE_START((x) + (PAGE_SIZE -1 ))


bool  safe_mul(unsigned* , unsigned a , unsigned b);
bool safe_add(unsigned * , unsigned a, unsigned b);

jint changeCode (JNIEnv *env, jobject thisObj);
#endif //DEXAPPLICATION_COMMUTILS_H
