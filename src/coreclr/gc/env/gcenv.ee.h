//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//
// Interface between the GC and EE
//

#ifndef __GCENV_EE_H__
#define __GCENV_EE_H__

#include "gcinterface.h"

class GCToEEInterface
{
public:
    static void SuspendEE(SUSPEND_REASON reason);
    static void RestartEE(bool bFinishedGC); //resume threads.

    // 
    // The GC roots enumeration callback
    //
    static void GcScanRoots(promote_func* fn, int condemned, int max_gen, ScanContext* sc);

    // 
    // Callbacks issues during GC that the execution engine can do its own bookeeping
    //

    // start of GC call back - single threaded
    static void GcStartWork(int condemned, int max_gen); 

    //EE can perform post stack scanning action, while the 
    // user threads are still suspended 
    static void AfterGcScanRoots(int condemned, int max_gen, ScanContext* sc);

    // Called before BGC starts sweeping, the heap is walkable
    static void GcBeforeBGCSweepWork();

    // post-gc callback.
    static void GcDone(int condemned);

    // Promote refcounted handle callback
    static bool RefCountedHandleCallbacks(Object * pObject);

    // Sync block cache management
    static void SyncBlockCacheWeakPtrScan(HANDLESCANPROC scanProc, uintptr_t lp1, uintptr_t lp2);
    static void SyncBlockCacheDemote(int max_gen);
    static void SyncBlockCachePromotionsGranted(int max_gen);
    static uint32_t GetActiveSyncBlockCount();

    // Thread functions
    static bool IsPreemptiveGCDisabled();
    static bool EnablePreemptiveGC();
    static void DisablePreemptiveGC();
    static Thread* GetThread();

    static gc_alloc_context * GetAllocContext();

    static void GcEnumAllocContexts(enum_alloc_context_func* fn, void* param);

    static uint8_t* GetLoaderAllocatorObjectForGC(Object* pObject);

    // Diagnostics methods.
    static void DiagGCStart(int gen, bool isInduced);
    static void DiagUpdateGenerationBounds();
    static void DiagGCEnd(size_t index, int gen, int reason, bool fConcurrent);
    static void DiagWalkFReachableObjects(void* gcContext);
    static void DiagWalkSurvivors(void* gcContext);
    static void DiagWalkLOHSurvivors(void* gcContext);
    static void DiagWalkBGCSurvivors(void* gcContext);
    static void StompWriteBarrier(WriteBarrierParameters* args);

    static void EnableFinalization(bool foundFinalizers);

    static void HandleFatalError(unsigned int exitCode);
    static bool ShouldFinalizeObjectForUnload(void* pDomain, Object* obj);
    static bool ForceFullGCToBeBlocking();
    static bool EagerFinalized(Object* obj);
    static MethodTable* GetFreeObjectMethodTable();
    static bool GetBooleanConfigValue(const char* key, bool* value);
    static bool GetIntConfigValue(const char* key, int64_t* value);
    static bool GetStringConfigValue(const char* key, const char** value);
    static void FreeStringConfigValue(const char* key);
    static bool IsGCThread();
    static bool WasCurrentThreadCreatedByGC();
    static bool CreateThread(void (*threadStart)(void*), void* arg, bool is_suspendable, const char* name);
    static void WalkAsyncPinnedForPromotion(Object* object, ScanContext* sc, promote_func* callback);
    static void WalkAsyncPinned(Object* object, void* context, void(*callback)(Object*, Object*, void*));
    static IGCToCLREventSink* EventSink();

    static uint32_t GetDefaultDomainIndex();
    static void *GetAppDomainAtIndex(uint32_t appDomainIndex);
    static void *GetAppDomainForObject(Object *obj);
    static bool AppDomainCanAccessHandleTable(uint32_t appDomainIndex);
    static uint32_t GetIndexOfAppDomainBeingUnloaded();
    static uint32_t GetTotalNumSizedRefHandles();
    static bool AppDomainIsRudeUnload(void *appDomain);
    static void TryAssignObjectToAppDomain(Object *obj, uint32_t appDomainIndex);
    static void SetAppDomain(Object *obj);

    static bool AnalyzeSurvivorsRequested(int condemnedGeneration);
    static void AnalyzeSurvivorsFinished(int condemnedGeneration);

    static void RecordAllocatedBytesForHeap(size_t allocatedBytes, uint32_t heapNumber);
    static void RecordSurvivedBytesForHeap (size_t promotedBytes, uint32_t heapNumber, void *appDomain);
    static void RecordTotalSurvivedBytes(size_t totalSurvivedBytes);
    static void ResetTotalSurvivedBytes();

    static void VerifySyncTableEntry();

    static GCPerfCounters *GetGCPerfCounters();
    static void ResetRuntimeCheckPerfCounters();

    static bool SupportsWriteWatch();

    static void* VirtualReserve(size_t size, size_t alignment, uint32_t flags);
    static bool VirtualRelease(void *address, size_t size);
    static bool VirtualCommit(void *address, size_t size, uint32_t node = NUMA_NODE_UNDEFINED);
    static bool VirtualDecommit(void *address, size_t size);
    static bool VirtualReset(void *address, size_t size, bool unlock);
    static size_t GetVirtualMemoryLimit();
    static uint64_t GetPhysicalMemoryLimit();
    static void GetMemoryStatus(uint32_t* memory_load, uint64_t* available_physical, uint64_t* available_page_file);

    static void* NativeAlloc(size_t size);
    static void NativeFree(void *ptr);
};

#endif // __GCENV_EE_H__
