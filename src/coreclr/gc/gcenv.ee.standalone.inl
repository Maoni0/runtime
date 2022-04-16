//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

#ifndef __GCTOENV_EE_STANDALONE_INL__
#define __GCTOENV_EE_STANDALONE_INL__

#include "gcinterface.h"
#include "env/gcenv.ee.h"

// The singular interface instance. All calls in GCToEEInterface
// will be fowarded to this interface instance.
extern IGCToCLR* g_theGCToCLR;

// When we are building the GC in a standalone environment, we
// will be dispatching virtually against g_theGCToCLR to call
// into the EE. This class provides an identical API to the existing
// GCToEEInterface, but only forwards the call onto the global
// g_theGCToCLR instance.
inline void GCToEEInterface::SuspendEE(SUSPEND_REASON reason)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->SuspendEE(reason);
}

inline void GCToEEInterface::RestartEE(bool bFinishedGC)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->RestartEE(bFinishedGC);
}

inline void GCToEEInterface::GcScanRoots(promote_func* fn, int condemned, int max_gen, ScanContext* sc)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->GcScanRoots(fn, condemned, max_gen, sc);
}

inline void GCToEEInterface::GcStartWork(int condemned, int max_gen)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->GcStartWork(condemned, max_gen);
}

inline void GCToEEInterface::AfterGcScanRoots(int condemned, int max_gen, ScanContext* sc)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->AfterGcScanRoots(condemned, max_gen, sc);
}

inline void GCToEEInterface::GcBeforeBGCSweepWork()
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->GcBeforeBGCSweepWork();
}

inline void GCToEEInterface::GcDone(int condemned)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->GcDone(condemned);
}

inline bool GCToEEInterface::RefCountedHandleCallbacks(Object * pObject)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->RefCountedHandleCallbacks(pObject);
}

inline void GCToEEInterface::SyncBlockCacheWeakPtrScan(HANDLESCANPROC scanProc, uintptr_t lp1, uintptr_t lp2)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->SyncBlockCacheWeakPtrScan(scanProc, lp1, lp2);
}

inline void GCToEEInterface::SyncBlockCacheDemote(int max_gen)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->SyncBlockCacheDemote(max_gen);
}

inline void GCToEEInterface::SyncBlockCachePromotionsGranted(int max_gen)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->SyncBlockCachePromotionsGranted(max_gen);
}

inline uint32_t GCToEEInterface::GetActiveSyncBlockCount()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetActiveSyncBlockCount();
}

inline bool GCToEEInterface::IsPreemptiveGCDisabled()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->IsPreemptiveGCDisabled();
}

inline bool GCToEEInterface::EnablePreemptiveGC()
{
    assert(g_theGCToCLR != nullptr);
    return  g_theGCToCLR->EnablePreemptiveGC();
}

inline void GCToEEInterface::DisablePreemptiveGC()
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->DisablePreemptiveGC();
}

inline Thread* GCToEEInterface::GetThread()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetThread();
}

inline gc_alloc_context * GCToEEInterface::GetAllocContext()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetAllocContext();
}

inline void GCToEEInterface::GcEnumAllocContexts(enum_alloc_context_func* fn, void* param)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->GcEnumAllocContexts(fn, param);
}

inline uint8_t *GCToEEInterface::GetLoaderAllocatorObjectForGC(Object* pObject)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetLoaderAllocatorObjectForGC(pObject);
}

inline void GCToEEInterface::DiagGCStart(int gen, bool isInduced)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->DiagGCStart(gen, isInduced);
}

inline void GCToEEInterface::DiagUpdateGenerationBounds()
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->DiagUpdateGenerationBounds();
}

inline void GCToEEInterface::DiagGCEnd(size_t index, int gen, int reason, bool fConcurrent)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->DiagGCEnd(index, gen, reason, fConcurrent);
}

inline void GCToEEInterface::DiagWalkFReachableObjects(void* gcContext)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->DiagWalkFReachableObjects(gcContext);
}

inline void GCToEEInterface::DiagWalkSurvivors(void* gcContext)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->DiagWalkSurvivors(gcContext);
}

inline void GCToEEInterface::DiagWalkLOHSurvivors(void* gcContext)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->DiagWalkLOHSurvivors(gcContext);
}

inline void GCToEEInterface::DiagWalkBGCSurvivors(void* gcContext)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->DiagWalkBGCSurvivors(gcContext);
}

inline void GCToEEInterface::StompWriteBarrier(WriteBarrierParameters* args)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->StompWriteBarrier(args);
}

inline void GCToEEInterface::EnableFinalization(bool foundFinalizers)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->EnableFinalization(foundFinalizers);
}

inline void GCToEEInterface::HandleFatalError(unsigned int exitCode)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->HandleFatalError(exitCode);
}

inline bool GCToEEInterface::ShouldFinalizeObjectForUnload(void* pDomain, Object* obj)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->ShouldFinalizeObjectForUnload(pDomain, obj);
}

inline bool GCToEEInterface::ForceFullGCToBeBlocking()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->ForceFullGCToBeBlocking();
}

inline bool GCToEEInterface::EagerFinalized(Object* obj)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->EagerFinalized(obj);
}

inline MethodTable* GCToEEInterface::GetFreeObjectMethodTable()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetFreeObjectMethodTable();
}

inline bool GCToEEInterface::GetBooleanConfigValue(const char* key, bool* value)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetBooleanConfigValue(key, value);
}

inline bool GCToEEInterface::GetIntConfigValue(const char* key, int64_t* value)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetIntConfigValue(key, value);
}

inline bool GCToEEInterface::GetStringConfigValue(const char* key, const char** value)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetStringConfigValue(key, value);
}

inline void GCToEEInterface::FreeStringConfigValue(const char* value)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->FreeStringConfigValue(value);
}

inline bool GCToEEInterface::IsGCThread()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->IsGCThread();
}

inline bool GCToEEInterface::WasCurrentThreadCreatedByGC()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->WasCurrentThreadCreatedByGC();
}

inline bool GCToEEInterface::CreateThread(void (*threadStart)(void*), void* arg, bool is_suspendable, const char* name)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->CreateThread(threadStart, arg, is_suspendable, name);
}

inline void GCToEEInterface::WalkAsyncPinnedForPromotion(Object* object, ScanContext* sc, promote_func* callback)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->WalkAsyncPinnedForPromotion(object, sc, callback);
}

inline void GCToEEInterface::WalkAsyncPinned(Object* object, void* context, void(*callback)(Object*, Object*, void*))
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->WalkAsyncPinned(object, context, callback);
}

inline IGCToCLREventSink* GCToEEInterface::EventSink()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->EventSink();
}

inline uint32_t GCToEEInterface::GetDefaultDomainIndex()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetDefaultDomainIndex();   
}

inline void *GCToEEInterface::GetAppDomainAtIndex(uint32_t appDomainIndex)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetAppDomainAtIndex(appDomainIndex);
}

inline void *GCToEEInterface::GetAppDomainForObject(Object *obj)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetAppDomainForObject(obj);
}

inline bool GCToEEInterface::AppDomainCanAccessHandleTable(uint32_t appDomainIndex)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->AppDomainCanAccessHandleTable(appDomainIndex);
}

inline uint32_t GCToEEInterface::GetIndexOfAppDomainBeingUnloaded()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetIndexOfAppDomainBeingUnloaded();
}

inline uint32_t GCToEEInterface::GetTotalNumSizedRefHandles()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetTotalNumSizedRefHandles();
}

inline bool GCToEEInterface::AppDomainIsRudeUnload(void *appDomain)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->AppDomainIsRudeUnload(appDomain);
}

inline bool GCToEEInterface::AnalyzeSurvivorsRequested(int condemnedGeneration)
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->AnalyzeSurvivorsRequested(condemnedGeneration);
}

inline void GCToEEInterface::AnalyzeSurvivorsFinished(int condemnedGeneration)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->AnalyzeSurvivorsFinished(condemnedGeneration);
}

inline void GCToEEInterface::TryAssignObjectToAppDomain(Object *obj, uint32_t appDomainIndex)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->TryAssignObjectToAppDomain(obj, appDomainIndex);
}

inline void GCToEEInterface::SetAppDomain(Object *obj)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->SetAppDomain(obj);
}

inline void GCToEEInterface::RecordAllocatedBytesForHeap(size_t allocatedBytes, uint32_t heapNumber)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->RecordAllocatedBytesForHeap(allocatedBytes, heapNumber);
}

inline void GCToEEInterface::RecordSurvivedBytesForHeap (size_t promotedBytes, uint32_t heapNumber, void *appDomain)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->RecordSurvivedBytesForHeap(promotedBytes, heapNumber, appDomain);
}

inline void GCToEEInterface::RecordTotalSurvivedBytes(size_t totalSurvivedBytes)
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->RecordTotalSurvivedBytes(totalSurvivedBytes);
}

inline void GCToEEInterface::ResetTotalSurvivedBytes()
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->ResetTotalSurvivedBytes();
}

inline void GCToEEInterface::VerifySyncTableEntry()
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->VerifySyncTableEntry();
}

inline GCPerfCounters *GCToEEInterface::GetGCPerfCounters()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->GetGCPerfCounters();
}

inline void GCToEEInterface::ResetRuntimeCheckPerfCounters()
{
    assert(g_theGCToCLR != nullptr);
    g_theGCToCLR->ResetRuntimeCheckPerfCounters();
}

inline bool GCToEEInterface::SupportsWriteWatch()
{
    assert(g_theGCToCLR != nullptr);
    return g_theGCToCLR->SupportsWriteWatch();
}

//inline void* GCToEEInterface::VirtualReserve(size_t size, size_t alignment, uint32_t flags)
//{
//    assert(g_theGCToCLR != nullptr);
//    return g_theGCToCLR->VirtualReserve(size, alignment, flags);
//}
//
//inline bool GCToEEInterface::VirtualRelease(void *address, size_t size)
//{
//    assert(g_theGCToCLR != nullptr);
//    return g_theGCToCLR->VirtualRelease(address, size);
//}
//
//inline bool GCToEEInterface::VirtualCommit(void *address, size_t size, uint32_t node)
//{
//    assert(g_theGCToCLR != nullptr);
//    return g_theGCToCLR->VirtualCommit(address, size, node);
//}
//
//inline bool GCToEEInterface::VirtualDecommit(void *address, size_t size)
//{
//    assert(g_theGCToCLR != nullptr);
//    return g_theGCToCLR->VirtualDecommit(address, size);
//}
//
//inline bool GCToEEInterface::VirtualReset(void *address, size_t size, bool unlock)
//{
//    assert(g_theGCToCLR != nullptr);
//    return g_theGCToCLR->VirtualReset(address, size, unlock);
//}
//
//inline size_t GCToEEInterface::GetVirtualMemoryLimit()
//{
//    assert(g_theGCToCLR != nullptr);
//    return g_theGCToCLR->GetVirtualMemoryLimit();
//}
//
//inline uint64_t GCToEEInterface::GetPhysicalMemoryLimit()
//{
//    assert(g_theGCToCLR != nullptr);
//    return g_theGCToCLR->GetPhysicalMemoryLimit();
//}
//
//inline void GCToEEInterface::GetMemoryStatus(uint32_t* memory_load, uint64_t* available_physical, uint64_t* available_page_file)
//{
//    assert(g_theGCToCLR != nullptr);
//    g_theGCToCLR->GetMemoryStatus(memory_load, available_physical, available_page_file);
//}
//
//inline void* GCToEEInterface::NativeAlloc(size_t size)
//{
//    assert(g_theGCToCLR != nullptr);
//    return g_theGCToCLR->NativeAlloc(size);
//}
//
//inline void GCToEEInterface::NativeFree(void *ptr)
//{
//    assert(g_theGCToCLR != nullptr);
//    g_theGCToCLR->NativeFree(ptr);
//}
//
#endif // __GCTOENV_EE_STANDALONE_INL__
