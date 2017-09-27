//
// Created by John.Lu on 2017/9/26.
//
#include <sys/mman.h>
#include <unistd.h>
#include <vm/Common.h>
#include <libdex/SysUtil.h>
#include <malloc.h>
#include <pthread.h>
#include <stdlib.h>
#include "Extra.h"

void *dvmAllocRegion(size_t byteCount, int prot, const char *name) {
    void *base;
    int fd, ret;

    byteCount = ALIGN_UP_TO_PAGE_SIZE(byteCount);
    fd = ashmem_create_region(name, byteCount);
    if (fd == -1) {
        return NULL;
    }
    base = mmap(NULL, byteCount, prot, MAP_PRIVATE, fd, 0);
    ret = close(fd);
    if (base == MAP_FAILED) {
        return NULL;
    }
    if (ret == -1) {
        munmap(base, byteCount);
        return NULL;
    }
    return base;
}


//***
/*
 * Allocate cache.
 */
AtomicCache* dvmAllocAtomicCache(int numEntries)
{
    AtomicCache* newCache;

    newCache = (AtomicCache*) calloc(1, sizeof(AtomicCache));
    if (newCache == NULL)
        return NULL;

    newCache->numEntries = numEntries;

    newCache->entryAlloc = calloc(1,
                                  sizeof(AtomicCacheEntry) * numEntries + CPU_CACHE_WIDTH);
    if (newCache->entryAlloc == NULL) {
        free(newCache);
        return NULL;
    }
    /*
     * Adjust storage to align on a 32-byte boundary.  Each entry is 16 bytes
     * wide.  This ensures that each cache entry sits on a single CPU cache
     * line.
     */
    assert(sizeof(AtomicCacheEntry) == 16);
    newCache->entries = (AtomicCacheEntry*)
            (((int) newCache->entryAlloc + CPU_CACHE_WIDTH_1) & ~CPU_CACHE_WIDTH_1);

    return newCache;
}

//***********************
void* dvmMalloc(size_t size, int flags){
    void *ptr;

    return  ptr;

}
//**********************thread ******************************

/*
 * glibc-only stack dump function.  Requires link with "--export-dynamic".
 *
 * TODO: move this into libs/cutils and make it work for all platforms.
 */
void dvmPrintNativeBackTrace() {
    size_t MAX_STACK_FRAMES = 64;
    void *stackFrames[MAX_STACK_FRAMES];
//    size_t frameCount = backtrace(stackFrames, MAX_STACK_FRAMES);
//
//    /*
//     * TODO: in practice, we may find that we should use backtrace_symbols_fd
//     * to avoid allocation, rather than use our own custom formatting.
//     */
//    char** strings = backtrace_symbols(stackFrames, frameCount);
//    if (strings == NULL) {
//        ALOGE("backtrace_symbols failed: %s", strerror(errno));
//        return;
//    }
//
//    size_t i;
//    for (i = 0; i < frameCount; ++i) {
//        ALOGW("#%-2d %s", i, strings[i]);
//    }
//    free(strings);
}

/*
 * Abort the VM.  We get here on fatal errors.  Try very hard not to use
 * this; whenever possible, return an error to somebody responsible.
 */
void dvmAbort() {
    /*
     * Leave gDvm.lastMessage on the stack frame which can be decoded in the
     * tombstone file. This is for situations where we only have tombstone files
     * but no logs (ie b/5372634).
     *
     * For example, in the tombstone file you usually see this:
     *
     *   #00  pc 00050ef2  /system/lib/libdvm.so (dvmAbort)
     *   #01  pc 00077670  /system/lib/libdvm.so (_Z15dvmClassStartupv)
     *     :
     *
     * stack:
     *     :
     * #00 beed2658  00000000
     *     beed265c  7379732f
     *     beed2660  2f6d6574
     *     beed2664  6d617266
     *     beed2668  726f7765
     *     beed266c  6f632f6b
     *     beed2670  6a2e6572
     *     beed2674  00007261
     *     beed2678  00000000
     *
     * The ascii values between beed265c and beed2674 belongs to messageBuffer
     * and it can be decoded as "/system/framework/core.jar".
     */
    const int messageLength = 512;
    char messageBuffer[messageLength] = {0};
    int result = 0;

    snprintf(messageBuffer, messageLength, "%s", gDvm.lastMessage);

    /* So that messageBuffer[] looks like useful stuff to the compiler */
    for (int i = 0; i < messageLength && messageBuffer[i]; i++) {
        result += messageBuffer[i];
    }

    ALOGE("VM aborting");

    fflush(NULL);       // flush all open file buffers

    /* JNI-supplied abort hook gets right of first refusal */
    if (gDvm.abortHook != NULL)
        (*gDvm.abortHook)();

    /*
     * On the device, debuggerd will give us a stack trace.
     * On the host, we have to help ourselves.
     */
    dvmPrintNativeBackTrace();

    abort();

    /* notreached */
}


/*
 * Unlock pthread mutex.
 */
INLINE void dvmUnlockMutex(pthread_mutex_t* pMutex)
{
    int cc __attribute__ ((__unused__)) = pthread_mutex_unlock(pMutex);
    assert(cc == 0);
}


/*
 * Initialize a mutex.
 */
INLINE void dvmInitMutex(pthread_mutex_t* pMutex)
{
#ifdef CHECK_MUTEX
    pthread_mutexattr_t attr;
    int cc;
    pthread_mutexattr_init(&attr);
    cc = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
    assert(cc == 0);
    pthread_mutex_init(pMutex, &attr);
    pthread_mutexattr_destroy(&attr);
#else
    pthread_mutex_init(pMutex, NULL);       // default=PTHREAD_MUTEX_FAST_NP
#endif
}

/*
 * Grab a plain mutex.
 */
INLINE void dvmLockMutex(pthread_mutex_t* pMutex)
{
    int cc __attribute__ ((__unused__)) = pthread_mutex_lock(pMutex);
    assert(cc == 0);
}

/*
 * Stop tracking an object.
 *
 * We allow attempts to delete NULL "obj" so that callers don't have to wrap
 * calls with "if != NULL".
 */
void dvmReleaseTrackedAlloc(Object* obj, Thread* self)
{
    if (obj == NULL)
        return;

//    if (self == NULL)
//        self = dvmThreadSelf();
//    assert(self != NULL);
//
//    if (!dvmRemoveFromReferenceTable(&self->internalLocalRefTable,
//                                     self->internalLocalRefTable.table, obj))
//    {
//        ALOGE("threadid=%d: failed to remove %p from internal ref table",
//              self->threadId, obj);
//        dvmAbort();
//    }
}


//*****************hashTable
/*
 * Clear out all entries.
 */
void dvmHashTableClear(HashTable* pHashTable) {
    struct  HashEntry * pEnt =NULL;
    int i;

    pEnt = pHashTable->pEntries;
    for (i = 0; i < pHashTable->tableSize; i++, pHashTable->pEntries++) {
        if (pEnt->data == HASH_TOMBSTONE) {
            // nuke entry
            pEnt->data = NULL;
        } else if (pEnt->data != NULL) {
            // call free func then nuke entry
            if (pHashTable->freeFunc != NULL)
                (*pHashTable->freeFunc)(pEnt->data);
            pEnt->data = NULL;
        }
    }

    pHashTable->numEntries = 0;
    pHashTable->numDeadEntries = 0;
}

/*
 * Create and initialize a hash table.
 */
HashTable* dvmHashTableCreate(size_t initialSize, HashFreeFunc freeFunc)
{
    HashTable* pHashTable;

    assert(initialSize > 0);

    pHashTable = (HashTable*) malloc(sizeof(*pHashTable));
    if (pHashTable == NULL)
        return NULL;

//    dvmInitMutex(&pHashTable->lock);

    pHashTable->tableSize = dexRoundUpPower2(initialSize);
    pHashTable->numEntries = pHashTable->numDeadEntries = 0;
    pHashTable->freeFunc = freeFunc;
    pHashTable->pEntries =
            (HashEntry*) calloc(pHashTable->tableSize, sizeof(HashEntry));
    if (pHashTable->pEntries == NULL) {
        free(pHashTable);
        return NULL;
    }

    return pHashTable;
    }
/*
 * Free the table.
 */
void dvmHashTableFree(HashTable* pHashTable)
{
    if (pHashTable == NULL)
        return;
    dvmHashTableClear(pHashTable);
    free(pHashTable->pEntries);
    free(pHashTable);
}



