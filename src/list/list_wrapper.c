/*
 * list_wrapper.c
 *
 * Wrapper functions for simple interpreter interface.
 */

#include <stdlib.h>
#include <stdarg.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_wrapper.h"
#include "list.h"

void *
interpCreate(void)
{
  return listCreate(interpAlloc, 
                    interpDealloc,
                    interpDuplicateEntry, 
                    interpDeleteEntry,
                    interpDebug, 
                    interpComp, 
                    NULL);
}

void *
interpFind(void * const list, void * const entry)
{
  return listFind(list, entry);
}

void *
interpInsert(void * const list, void * const entry)
{
  return listInsert(list, entry);
}

void *
interpRemove(void * const list, void * const entry)
{
  return listRemove(list, entry);
}

void 
interpClear(void * const list)
{
  listClear(list);
}

void 
interpDestroy(void * const list)
{
  listDestroy(list);
}

size_t
interpGetDepth(const void * const list)
{
  (void)list;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

size_t
interpGetSize(const void * const list)
{
  return listGetSize(list);
}

void *
interpGetMin(void * const list)
{
  (void)list;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetMax(void * const list)
{
  (void)list;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetFirst(void * const list)
{
  return listGetFirst(list);
}

void *
interpGetLast(void * const list)
{
  return listGetLast(list);
}

void *
interpGetNext(void * const list, const void * const entry)
{
  return listGetNext(list, entry);
}

void *
interpGetPrevious(void * const list, const void * const entry)
{
  return listGetPrevious(list, entry);
}

void *
interpGetUpper(void * const list, const void * const entry)
{
  (void)list;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLower(void * const list, const void * const entry)
{
  (void)list;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

bool
interpCheck(void * const list)
{
  (void)list;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

bool
interpWalk(void * const list, const interpWalkFunc_t walk)
{
  (void)list;
  (void)walk;
  return listWalk(list, walk);
}

bool
interpSort(void * const list)
{
  (void)list;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

bool
interpBalance(void * const list)
{
  (void)list;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

void
interpPop(void * const list)
{
  (void)list;
  interpError(__func__, __LINE__, "Not implemented!");
  return;
}

void *
interpPeek(void *list)
{
  (void)list;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

void *
interpPush(void * const list, void * const entry)
{
  (void)list;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpCopy(void * const dst, void * const src)
{
  return listCopy(dst, src);
}
