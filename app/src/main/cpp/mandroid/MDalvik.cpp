//
// Created by John.Lu on 2017/9/27.
//
#include <stdlib.h>
#include <string.h>
#include "MDalvik.h"
#include<dirent.h>
#include "../Android/libdex/DexFile.h"

/**
 * 根据进程名称获取pid
 *
 * */
int find_pid_by_processname(const char* process_name){
    pid_t  pid =-1;
    int id =0;
    DIR* dir;
    FILE *fp;
    char filename[32];
    char cmdline[256];
   struct  dirent* entry;
    if(process_name == NULL){
        ALOGE("process name is NULL");
        return pid;
    }
    dir = opendir("/proc");
    if(dir == NULL){
        ALOGE("opendir /proc error ");
        return pid;
    }
    while ((entry = readdir(dir)) != NULL){
        id =  atoi(entry->d_name);
        if(id != 0){
            sprintf(filename,"/proc/%d/cmdline",id);
            fp = fopen(filename,"r");
            if(fp !=NULL)
            {
                fgets(cmdline, sizeof(cmdline),fp);
                fclose(fp);
                if(strcmp(process_name ,cmdline) == 0)
                {
                    pid = id;
                    ALOGE("found module pid =  %d ",pid);
                    break;
                }
            }
        }
    }

    /*
     * 释放资源
     * */

    if(dir != NULL){
    closedir(dir);
    }

    return pid ;
}

void* get_module_base(const char* module_name,pid_t pid){
    void *start;
    void *end;
    char* s ;
    FILE *fp;
    char filename[32];

    if(pid < 0){
        sprintf(filename, "/proc/self/maps",pid);
    }
    else{
            sprintf(filename, "/proc/%d/maps",pid);
    }
    fp = fopen(filename, "r");
    if (fp != NULL) {
        char line[2048];
        while (fgets(line, sizeof line, fp) != NULL) /* read a line */
        {
            if (strstr(line, module_name) != NULL) {
                if (strstr(line, "classes.dex") != NULL) {
                    ALOGE("line %s ",line);
                    s = strchr(line, '-');
                    if (s == NULL)
                        ALOGD(" Error: string NULL");
                    *s++ = '\0';
                    start = (void *) strtoul(line, NULL, 16);
                    end = (void *) strtoul(s, NULL, 16);
                    ALOGD(" startAddress = %x", (unsigned int) start); //读取首尾地址
                    ALOGD(" endAddress = %x", (unsigned int) end);
                    break;
                }
            }
        }
        fclose(fp);
    }
  return start;
}
/**
 *接口方法
 *
 * Android4.4.4版本 5.0级以上版本暂时还没通用
 *
 * */
void call_changeCode(const char* moduleName ){
    pid_t  pid =find_pid_by_processname("com.dexapplication");
    long baseaddr = (long) get_module_base(moduleName, pid);
//    u1 *pDex = (u1 *) get_module_base(moduleName, pid);
//  ALOGE(" modulepid = %d  ",pid);
//    if(pDex !=NULL){
//        ALOGE("");
//       DexFile *pDexFile =  dexFileParse(pDex, sizeof(pDex),kDexParseContinueOnError);
//        if(pDexFile == NULL)
//        {
//            ALOGE("parse dexfile error");
//            return;
//        }
//        else{
//            ALOGE("parse dexfile ok");
//
//        }
//    }


}







