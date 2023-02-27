/*
 * splayCache_wrapper.c
 *
 * Wrapper functions for simple interpreter interface.
 */

#include <stdlib.h>
#include <stdarg.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_wrapper.h"
#include "splayCache.h"

void *
interpCreate(void)
{
  return splayCacheCreate(128, 
                          interpAlloc, 
                          interpDealloc,
                          interpDuplicateEntry,
                          interpDeleteEntry,
                          interpDebug,
                          interpComp,
                          NULL);
}

void *
interpFind(void * const cache, void * const entry)
{
  return splayCacheFind(cache, entry);
}

void *
interpInsert(void * const cache, void * const entry)
{
  return splayCacheInsert(cache, entry);
}

void *
interpRemove(void * const cache, void * const entry)
{
  (void)cache;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void 
interpClear(void * const cache)
{
  splayCacheClear(cache);
}

void 
interpDestroy(void * const cache)
{
  splayCacheDestroy(cache);
}

size_t
interpGetDepth(const void * const cache)
{
  return splayCacheGetDepth(cache);
}

size_t
interpGetSize(const void * const cache)
{
  return splayCacheGetSize(cache);
}

void *
interpGetMin(void * const cache)
{
  (void)cache;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetMax(void * const cache)
{
  (void)cache;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetFirst(void * const cache)
{
  (void)cache;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLast(void * const cache)
{
  (void)cache;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetNext(void * const cache, const void * const entry)
{
  (void)cache;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetPrevious(void * const cache, const void * const entry)
{
  (void)cache;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetUpper(void * const cache, const void * const entry)
{
  (void)cache;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLower(void * const cache, const void * const entry)
{
  (void)cache;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

bool
interpCheck(void * const cache)
{
  return splayCacheCheck(cache);
}

bool
interpWalk(void * const cache, const interpWalkFunc_t walk)
{
  return splayCacheWalk(cache, walk);
}

bool
interpSort(void * const cache)
{
  (void)cache;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

bool
interpBalance(void * const cache)
{
  return splayCacheBalance(cache);
}

void 
interpPop(void * const cache)
{
  (void)cache;
  interpError(__func__, __LINE__, "Not implemented!");
  return;
}

void *
interpPeek(void * const cache)
{
  (void)cache;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

void *
interpPush(void * const cache, void * const entry)
{
  (void)cache;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

void *
interpCopy(void * const dst, void * const src)
{
  (void)dst;
  (void)src;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}
