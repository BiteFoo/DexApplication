//
// Created by John.Lu on 2017/9/26.
//

#ifndef DEXAPPLICATION_EXTRA_H
#define DEXAPPLICATION_EXTRA_H
#define ASHMEM_NAME_LEN 128
#include <fcntl.h>
#include <vm/Common.h>
#include <vm/Dalvik.h>
#include <vm/LinearAlloc.h>
#include "CommUtils.h"


//struct DvmGlobals gDvm;
#define CPU_CACHE_WIDTH         32
#define CPU_CACHE_WIDTH_1       (CPU_CACHE_WIDTH-1)

#define ATOMIC_LOCK_FLAG        (1 << 31)

#define  HASH_TOMBSTONE ((void*) 0xcbcacccd) //invalid ptr value
#define BYTE_OFFSET(_ptr, _offset)  ((void*) (((u1*)(_ptr)) + (_offset)))
/* flags for dvmMalloc */
enum {
    ALLOC_DEFAULT = 0x00,
    ALLOC_DONT_TRACK = 0x01,  /* don't add to internal tracking list */
    ALLOC_NON_MOVING = 0x02,
};
//
///* Primary interface macros */
///* type checking is compiled out if NDEBUG supplied. */
//#define safe_add(_ptr, __a, __b) \
// ({ int __sio(var)(ok) = 0; \
//    typeof(__a) __sio(var)(_a) = (__a); \
//    typeof(__b) __sio(var)(_b) = (__b); \
//    typeof(_ptr) __sio(var)(p) = (_ptr); \
//    if (__sio(m)(assert)(__sio(m)(type_enforce)(__sio(var)(_a), \
//                                                __sio(var)(_b)))) { \
//      if (__sio(m)(smin)(__sio(var)(_a)) <= ((typeof(__sio(var)(_a)))0)) { \
//        __sio(var)(ok) = safe_sadd(__sio(var)(p), \
//                                   __sio(var)(_a), \
//                                   __sio(var)(_b)); \
//      } else { \
//        __sio(var)(ok) = safe_uadd(__sio(var)(p), \
//                                   __sio(var)(_a), \
//                                   __sio(var)(_b)); \
//      } \
//    } \
//    __sio(var)(ok); })
//#define safe_mul(_ptr, __a, __b) \
// ({ int __sio(var)(ok) = 0; \
//    typeof(__a) __sio(var)(_a) = (__a); \
//    typeof(__b) __sio(var)(_b) = (__b); \
//    typeof(_ptr) __sio(var)(p) = (_ptr); \
//    if (__sio(m)(assert)(__sio(m)(type_enforce)(__sio(var)(_a), \
//                                                __sio(var)(_b)))) { \
//      if (__sio(m)(umax)(__sio(var)(_a)) <= ((typeof(__sio(var)(_a)))0)) { \
//        __sio(var)(ok) = safe_smul(__sio(var)(p), \
//                                   __sio(var)(_a), \
//                                   __sio(var)(_b)); \
//      } else { \
//        __sio(var)(ok) = safe_umul(__sio(var)(p), \
//                                   __sio(var)(_a), \
//                                   __sio(var)(_b)); \
//      } \
//    } \
//    __sio(var)(ok); })

inline int
ashmem_create_region(const char *name, size_t len)
{
    return open("/dev/zero", O_RDWR);
}


/*
 * This represents an open, scanned Jar file.  (It's actually for any Zip
 * archive that happens to hold a Dex file.)
 */
struct JarFile {
    ZipArchive  archive;
    //MemMapping  map;
    char*       cacheFileName;
    DvmDex*     pDvmDex;
};


struct AtomicCacheEntry {
    u4          key1;
    u4          key2;
    u4          value;
    volatile u4 version;    /* version and lock flag */
};
struct AtomicCache {
    AtomicCacheEntry*   entries;        /* array of entries */
    int         numEntries;             /* #of entries, must be power of 2 */

    void*       entryAlloc;             /* memory allocated for entries */

    /* cache stats; note we don't guarantee atomic increments for these */
    int         trivial;                /* cache access not required */
    int         fail;                   /* contention failure */
    int         hits;                   /* found entry in cache */
    int         misses;                 /* entry was for other keys */
    int         fills;                  /* entry was empty */
};



/*
 * Our per-thread data.
 *
 * These are allocated on the system heap.
 */

struct Thread{
    /* small unique integer; useful for "thin" locks and debug messages */
    u4          threadId;

};


struct DvmGlobals{
    /*
     * Some options from the command line or environment.
     */
    char*       bootClassPathStr;

    /*
     *
     * */
    bool        verifyDexChecksum;
    bool        verboseShutdown;
    /* synthetic classes representing primitive types */
    ClassObject* typeVoid;
    ClassObject* typeBoolean;
    ClassObject* typeByte;
    ClassObject* typeShort;
    ClassObject* typeChar;
    ClassObject* typeInt;
    ClassObject* typeLong;
    ClassObject* typeFloat;
    ClassObject* typeDouble;
    /* the class Class */
    ClassObject* classJavaLangClass;
    /*
    * Bootstrap class loader linear allocator.
    */
    LinearAllocHdr* pBootLoaderAlloc;


    /*
  * Loaded classes, hashed by class name.  Each entry is a ClassObject*,
  * allocated in GC space.
  */
    HashTable*  loadedClasses;

    /*
    * Value for the next class serial number to be assigned.  This is
    * incremented as we load classes.  Failed loads and races may result
    * in some numbers being skipped, and the serial number is not
    * guaranteed to start at 1, so the current value should not be used
    * as a count of loaded classes.
    */
    volatile int classSerialNumber;

    /*
     * Where the VM goes to find system classes.
     */
    ClassPathEntry* bootClassPath;

    /*
 * Classes with a low classSerialNumber are probably in the zygote, and
 * their InitiatingLoaderList is not used, to promote sharing. The list is
 * kept here instead.
 */
    InitiatingLoaderList* initiatingLoaderList;

    //***************
    /*
    * Lock profiling threshold value in milliseconds.  Acquires that
    * exceed threshold are logged.  Acquires within the threshold are
    * logged with a probability of $\frac{time}{threshold}$ .  If the
    * threshold is unset no additional logging occurs.
    */
    void        (*abortHook)(void);
    /* String pointed here will be deposited on the stack frame of dvmAbort */
    const char *lastMessage;
}gDvm;
struct HashEntry{
    unsigned  int hashValue;
    void* data;
}HashEntry;


typedef void (*HashFreeFunc)(void* ptr);
struct HashTable{
    u4 tableSize;
    HashFreeFunc freeFunc;
    u4 numEntries;
    u4  numDeadEntries;
   struct  HashEntry* pEntries;

//    pthread_mutex_t* lock;
};




/* pry the DexFile out of a JarFile */
INLINE DvmDex* dvmGetJarFileDex(JarFile* pJarFile) {
    return pJarFile->pDvmDex;
}

/*
 * Unlock pthread mutex.
 */
INLINE void dvmUnlockMutex(pthread_mutex_t* pMutex);

INLINE bool dvmIsArrayClass(const ClassObject* clazz)
{
    return (clazz->descriptor[0] == '[');
}

//************ObjectInlines *********
INLINE float dvmGetFieldFloat(const Object* obj, int offset) {
    return (( JValue*)BYTE_OFFSET(obj, offset))->f;
}
INLINE double dvmGetFieldDouble(const Object* obj, int offset) {
    return (( JValue*)BYTE_OFFSET(obj, offset))->d;
}
INLINE s8 dvmGetFieldLong(const Object* obj, int offset) {
    return (( JValue*)BYTE_OFFSET(obj, offset))->j;
}
INLINE bool dvmGetFieldBoolean(const Object* obj, int offset) {
    return (( JValue*)BYTE_OFFSET(obj, offset))->z;
}
INLINE s4 dvmGetFieldInt(const Object* obj, int offset) {
    return (( JValue*)BYTE_OFFSET(obj, offset))->i;
}

//***************************

void *dvmAllocRegion(size_t byteCount, int prot, const char *name);
AtomicCache* dvmAllocAtomicCache(int numEntries);
INLINE void dvmInitMutex(pthread_mutex_t* pMutex);
INLINE void dvmLockMutex(pthread_mutex_t* pMutex);
void dvmAbort();
void dvmReleaseTrackedAlloc(Object* obj, Thread* self);
HashTable* dvmHashTableCreate(size_t initialSize, HashFreeFunc freeFunc);
void dvmHashTableClear(HashTable* pHashTable);
void dvmHashTableFree(HashTable* pHashTable);
//***********************
void* dvmMalloc(size_t size, int flags);
#endif //DEXAPPLICATION_EXTRA_H
