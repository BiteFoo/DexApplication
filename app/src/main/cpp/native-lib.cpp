#include <jni.h>
#include <string>
#include "mlog.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <vm/Dalvik.h>
#include <MDalvik.h>

#define TAG    "mmdalvik"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)

// Created by John.Lu on 2017/9/26.
//
int readUleb128(int *address, int read_count)
{
    int *read_address;
    int result;
    signed int bytes_read;
    signed int value_1;
    signed int value_2;
    signed int value_3;
    read_address = address;
    result = *(char *)address;
    bytes_read = 1;
    if ( (unsigned int)result > 0x7F )
    {
        value_1 = *((char *)read_address + 1);
        result = result & 0x7F | ((value_1 & 0x7F) << 7);
        bytes_read = 2;
        if ( value_1 > 127 )
        {
            value_2 = *((char *)read_address + 2);
            result |= (value_2 & 0x7F) << 14;
            bytes_read = 3;
            if ( value_2 > 127 )
            {
                value_3 = *((char *)read_address + 3);
                bytes_read = 4;
                result |= (value_3 & 0x7F) << 21;
                if ( value_3 > 127 )
                {
                    bytes_read = 5;
                    result |= *((char *)read_address + 4) << 28;
                }
            }
        }
    }
    *(int *)read_count = bytes_read;
    return result;
}

int * skipUleb128(int num, int *address)
{
    int read_count;
    int *read_address;
    int i;

    read_count = num;
    read_address = address;
    for ( i = 0; read_count; read_address = (int *)((char *)read_address + i) )
    {
        readUleb128(read_address, (int)&i);
        --read_count;
    }
    return read_address;
}

/**
 * 找寻dex 头部信息
 * */
int findmagic(void *search_start_position)
{
    int result=0;
    char dest[10];
    memcpy(&dest,"dex\n035",8);
    if(memcmp(search_start_position,&dest,8) != 0)
        result = 0;
    else
        result = 1;
    return result;
}

/**
 * 查找函数 ，根据函数名称做为判断依据
 * target_string ：指的是需要替换的函数
 *
 * */
int getStrIdx(int search_start_position, char *target_string,int size )
{

    ALOGE("call getStrIdx %s ",target_string);
    int index;
    int stringidsoff;
    int stringdataoff;
    int *stringaddress;
    int string_num_mutf8;

    if(*(int *)(search_start_position+56))
    {
        index = 0;
        stringidsoff = search_start_position + *(int *)(search_start_position+60);
        while(1)
        {
            stringdataoff = *(int *)stringidsoff;
            stringidsoff +=4;
            stringaddress = (int *)(search_start_position + stringdataoff);
            string_num_mutf8 = 0;
            if( readUleb128(stringaddress,(int)&string_num_mutf8) == size && !strncmp((char *)stringaddress + string_num_mutf8,target_string,size) ) {
                ALOGE("------------->>> found target %s  , index  = %d ",target_string,index);
                break;
            }
            ++index;
            if(*(int *)(search_start_position+56) <= index )
            { index = -1;break;}
        }

    }else
    {
        index = -1;
    }
    return index;
}
/**
 * 查找方法类型
 * */
signed int  getTypeIdx(int search_start_position, int strIdx)
{
    int typeIdsSize;
    int typeIdsOff;
    int typeid_to_stringid;
    signed int result;
    int next_typeIdsOff;
    int next_typeid_to_stringid;

    typeIdsSize = *(int *)(search_start_position + 64);
    if ( !typeIdsSize )
        return -1;
    typeIdsOff = search_start_position + *(int *)(search_start_position + 68);
    typeid_to_stringid = *(int *)typeIdsOff;
    result = 0;
    next_typeIdsOff = typeIdsOff + 4;
    if ( typeid_to_stringid != strIdx )
    {
        while ( 1 )
        {
            ++result;
            if ( result == typeIdsSize )
                break;
            next_typeid_to_stringid = *(int *)next_typeIdsOff;
            next_typeIdsOff += 4;
            if ( next_typeid_to_stringid == strIdx )
                return result;
        }
        return -1;
    }
    return result;
}
/**
 * 查找对应的类
 * */
int getClassItem(int search_start_position, int class_typeIdx)
{
    int classDefsSize;

    int classDefsOff;
    int result;
    int classIdx;
    int count;

    classDefsSize = *(int *)(search_start_position + 96);

    classDefsOff = *(int *)(search_start_position + 100);
    result = 0;
    if ( classDefsSize )
    {
        classIdx = search_start_position + classDefsOff;
        result = classIdx;
        if ( *(int *)classIdx != class_typeIdx )
        {
            count = 0;
            while ( 1 )
            {
                ++count;
                if ( count == classDefsSize )
                    break;
                result += 32;
                if ( *(int *)(result) == class_typeIdx )
                    return result;
            }
            result = 0;
        }
    }
    return result;
}
/**
 * 获取方法
 * */
int  getMethodIdx(int search_start_position, int method_strIdx, int class_typeIdx)
{
    int methodIdsSize;
    int classIdx;
    signed int result;

    methodIdsSize = *(int *)(search_start_position + 88);
    if ( methodIdsSize )
    {
        classIdx = search_start_position + *(int *)(search_start_position + 92);
        result = 0;
        while ( *(short *)classIdx != class_typeIdx || *(int *)(classIdx + 4) != method_strIdx )
        {
            ++result;
            classIdx += 8;
            if ( result == methodIdsSize )
            {result = -1;break;}
        }
    }
    else
    {

        result = -1;
    }
    return result;
}
/**
 *
 * 查找对那个的code结构体
 * */
int getCodeItem(int search_start_position, int class_def_item_address, int methodIdx)
{

    int *classDataOff;
    int staticFieldsSize;
    int *classDataOff_new_start;
    int instanceFieldsSize;

    int directMethodsSize;
    int virtualMethodSize;

    int *after_skipstaticfield_address;
    int *DexMethod_start_address;
    int result;
    int DexMethod_methodIdx;
    int *DexMethod_accessFlagsstart_address;
    int Uleb_bytes_read;
    int tmp;

    classDataOff = (int *)(*(int *)(class_def_item_address + 24) + search_start_position);//
    LOGD(" classDataOff = %x", classDataOff);


    Uleb_bytes_read = 0;//每次读取的uleb格式的字节数
    staticFieldsSize = readUleb128(classDataOff, (int)&Uleb_bytes_read); //读取静态字段
    LOGD("staticFieldsSize= %x",staticFieldsSize);

    classDataOff_new_start = (int *)((char *)classDataOff + Uleb_bytes_read);
    LOGD("staticFieldsSize_addr= %x",classDataOff_new_start);
    instanceFieldsSize = readUleb128(classDataOff_new_start, (int)&Uleb_bytes_read);
    LOGD("instanceFieldsSize= %x",instanceFieldsSize);
    classDataOff_new_start = (int *)((char *)classDataOff_new_start + Uleb_bytes_read);
    LOGD("instanceFieldsSize_addr= %x",classDataOff_new_start);

    directMethodsSize = readUleb128(classDataOff_new_start, (int)&Uleb_bytes_read);
    LOGD("directMethodsSize= %x",directMethodsSize);
    classDataOff_new_start  = (int *)((char *)classDataOff_new_start + Uleb_bytes_read);
    LOGD("directMethod_addr= %x",classDataOff_new_start);
    virtualMethodSize = readUleb128(classDataOff_new_start, (int)&Uleb_bytes_read);
    LOGD("virtualMethodsSize= %x",virtualMethodSize);
    after_skipstaticfield_address = skipUleb128(2 * staticFieldsSize, (int *)((char *)classDataOff_new_start + Uleb_bytes_read));
    LOGD("after_skipstaticfield_address = %x",  after_skipstaticfield_address);
    DexMethod_start_address = skipUleb128(2 * instanceFieldsSize, after_skipstaticfield_address);
    LOGD("DexMethod_start_address = %x", DexMethod_start_address);
    result = 0;
    if ( directMethodsSize )
    {
//        ALOGE("----------------directMethodIdx %d ",methodIdx);
        DexMethod_methodIdx = 0;
        int DexMethod_methodIdx_tmp = 0;
        do
        {
            DexMethod_methodIdx_tmp = 0;
            DexMethod_methodIdx = readUleb128(DexMethod_start_address, (int)&Uleb_bytes_read);
            DexMethod_methodIdx_tmp = readUleb128(DexMethod_start_address, (int)&Uleb_bytes_read);
            ALOGE("---------------  directMethodIdx %x ",DexMethod_methodIdx);

//            LOGD("DexMethod_direct_methodIdx = %x", DexMethod_methodIdx);
//            LOGD("DexMethod_direct_methodIdx_tmp = %x", DexMethod_methodIdx_tmp);
            DexMethod_accessFlagsstart_address = (int *)((char *)DexMethod_start_address + Uleb_bytes_read);
            if ( DexMethod_methodIdx == methodIdx )
            {
                ALOGE("----------------found   directMethodIdx %x ",DexMethod_methodIdx);

                readUleb128(DexMethod_accessFlagsstart_address, (int)&Uleb_bytes_read);
                return readUleb128((int *)((char *)DexMethod_accessFlagsstart_address + Uleb_bytes_read), (int)&Uleb_bytes_read) +  search_start_position;
            }
            --directMethodsSize;
            DexMethod_start_address = skipUleb128(2, DexMethod_accessFlagsstart_address);
        }while ( directMethodsSize );
        result = 0;
    }
    if ( virtualMethodSize )
    {
        DexMethod_methodIdx = 0;
            ALOGE("+++++++++++++++virtualMethodIdx = %x ",methodIdx);
        int DexMethod_methodIdx_tmp = 0;
        do
        {
            DexMethod_methodIdx_tmp = 0;
            DexMethod_methodIdx = readUleb128(DexMethod_start_address, (int)&Uleb_bytes_read);
            DexMethod_methodIdx_tmp = readUleb128(DexMethod_start_address, (int)&Uleb_bytes_read);
            ALOGE("+++++++++++++++DexMethod_methodIdx = %x ",DexMethod_methodIdx);
            DexMethod_accessFlagsstart_address = (int *)((char *)DexMethod_start_address + Uleb_bytes_read);
            if ( DexMethod_methodIdx == methodIdx )
            {
                ALOGE("---------------- found method  %x ",DexMethod_methodIdx);
                readUleb128(DexMethod_accessFlagsstart_address, (int)&Uleb_bytes_read);
                return readUleb128((int *)((char *)DexMethod_accessFlagsstart_address + Uleb_bytes_read), (int)&Uleb_bytes_read) +  search_start_position;
            }
            --virtualMethodSize;
            DexMethod_start_address = skipUleb128(2, DexMethod_accessFlagsstart_address);
        }while ( virtualMethodSize );
        result = 0;
    }
    return result;
}

/**
 * 方法入口
 * */
jint changeCode (JNIEnv *env, jobject thisObj) {
    jint result = 1;
    char *s;
    void *start;
    void *end;
    FILE *fp;
    fp = fopen("/proc/self/maps", "r");
    if (fp != NULL) {
        char line[2048];
        while (fgets(line, sizeof line, fp) != NULL) /* read a line */
        {
            if (strstr(line, "/data/dalvik-cache/data@app@com.dexapplication") != NULL) {
                if (strstr(line, "classes.dex") != NULL) {
                    ALOGE("[============] maps line: %s",line );
                    s = strchr(line, '-');
                    ALOGE("[============] maps s: %s",s);
                    if (s == NULL)
                        LOGD(" Error: string NULL");

                    *s++ = '\0';
                    start = (void *) strtoul(line, NULL, 16);
                    end = (void *) strtoul(s, NULL, 16);
                    LOGD(" startAddress = %x", (unsigned int) start);
                    LOGD(" endAddress = %x", (unsigned int) end);
                    break;
                }
            }
        }
        fclose(fp);
    }
    long page_size = sysconf(39);
    ALOGE("pagesize %d",page_size);
    unsigned int start_address = (unsigned int) start;
    unsigned int end_address = (unsigned int) end;
    //*******************
    int search_start_page;
    unsigned int dex_search_start = end_address;
    int search_start_position;
    search_start_page = dex_search_start - dex_search_start % page_size;
    do {
        search_start_page -= page_size;
        //这里读取的是odex（内存中的dex其实是odex数据） 需要往后移动0x28个长度，然后才是dex.035的数据
        search_start_position = search_start_page + 40; //0x28 = 40
    } while (!findmagic((void *) (search_start_page + 40)));
    LOGD(" search_start_page = %x", search_start_page);
    LOGD(" search_start_position = %x", search_start_position);
    LOGD("string = %s", (char *) search_start_position); //找到dex头部 地址
    int class_strIdx = 0;
    //查找类
    class_strIdx = getStrIdx(search_start_position, "Lcom/dexapplication/MainActivity;",
                             strlen("Lcom/dexapplication/MainActivity;"));
    LOGD("class_strIdx = %x", class_strIdx);
    int method_strIdx = 0;
    //找到方法 ret1
    ALOGE("[=====]try to find  method  ret1 method idx");
    method_strIdx = getStrIdx(search_start_position, "ret1", strlen("ret1"));
    ALOGE("[=====] method_strIdx = %x", method_strIdx);
    if(method_strIdx == 0)
    {
        LOGD("************* method ret1 addr is 0");
    } else{
        LOGD("************* method ret1 addr is %d ",method_strIdx);
    }
    int class_typeIdx = 0;
    //找到对应的方法类型
    class_typeIdx = getTypeIdx(search_start_position, class_strIdx);
    ALOGE("class_typeIdx = %x", class_typeIdx);

    int class_def_item_address = 0;


    class_def_item_address = getClassItem(search_start_position, class_typeIdx);
    ALOGE("class_def_item_address = %x", class_def_item_address);

    int methodIdx = 0;

    methodIdx = getMethodIdx(search_start_position, method_strIdx, class_typeIdx);
    ALOGE("methodIdx = %x", methodIdx);

    int codeItem_address = 0;

    ALOGE("mmmm  search_start_position =%x  ,class_def_item_address =%x  ,methodIdx=%x ",search_start_position,class_def_item_address,methodIdx);
    codeItem_address = getCodeItem(search_start_position, class_def_item_address, methodIdx);
    ALOGE("codeItem_address = %x", codeItem_address);

     if(codeItem_address ==  0){
         ALOGE("not found CodeItem address ");
         return -1;
     }

    void *code_insns_address;
    code_insns_address = (void *) (codeItem_address + 16);
    ALOGE("code_insns_address = %x", code_insns_address);
    void *codeinsns_page_address = (void *) (codeItem_address + 16 - (codeItem_address + 16) % (unsigned int) page_size);
    ALOGE("codeinsns_page_address = %x", codeinsns_page_address);
    if (codeinsns_page_address == NULL || (int*)(codeinsns_page_address) == 0){
        LOGD("code insns page address is NULL ");
        return -1;
    }

    mprotect(codeinsns_page_address, page_size, 3);
    char inject[] = {0x90, 0x00, 0x02, 0x03, 0x0f, 0x00};

    char ins[] = {0x12, 0x20,0x0f ,0x00}; //ret2()
    memcpy(code_insns_address, &ins, 4);
    return 0;
}

//******************


/**
 * 使用java传递方法体的方式，更改运行时的insns指令
 *
 * 在这里更改方法
 * FromReflectedMethod
 * */
char ins[] ={0x12, 0x20,0x0f ,0x00 };
void chgMethod(JNIEnv* env ,jobject method){

    /*         12 10 0f 00  --> ret1
     *  12 20 0f 00 --> ret2   
     * */

    ALOGE(" chgMethod .... call ");
    Method* me = (Method*)  env->FromReflectedMethod(method);
    ALOGE(" chgMethod .... call insns = %d , name = %s , code = %x",me->insSize ,me->name  ,me->insns);
    me->insns = (const u2*) ins;
}

/**
 *
 * 还没完成，暂时先不用。
 * */

void chamMethodByDalvik(){
    const char* module ="/data/dalvik-cache/data@app@com.dexapplication";
    call_changeCode(module);
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_dexapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject  thiz /* this */,jobject method) {
    std::string hello = "Hello from C++ ,ready to change ";
    ALOGE(" native  .... call ");
    chgMethod(env,method);
//    chamMethodByDalvik();
    return env->NewStringUTF(hello.c_str());
}
