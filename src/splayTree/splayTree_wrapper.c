/*
 * splayTree_wrapper.c
 *
 * Wrapper functions for simple interpreter interface.
 */

#include <stdlib.h>
#include <stdarg.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_wrapper.h"
#include "splayTree.h"

void *
interpCreate(void)
{
  return splayTreeCreate(interpAlloc,
                         interpDealloc,
                         interpDuplicateEntry,
                         interpDeleteEntry,
                         interpDebug,
                         interpComp,
                         NULL);
}

void *
interpFind(void * const tree, void * const entry)
{
  return splayTreeFind(tree, entry);
}

void *
interpInsert(void * const tree, void * const entry)
{
  return splayTreeInsert(tree, entry);
}

void *
interpRemove(void * const tree, void * const entry)
{
  return splayTreeRemove(tree, entry);
}

void 
interpClear(void * const tree)
{
  splayTreeClear(tree);
}

void 
interpDestroy(void * const tree)
{
  splayTreeDestroy(tree);
}

size_t
interpGetDepth(const void * const tree)
{
  return splayTreeGetDepth(tree);
}

size_t
interpGetSize(const void * const tree)
{
  return splayTreeGetSize(tree);
}

void *
interpGetMin(void * const tree)
{
  return splayTreeGetMin(tree);
}

void *
interpGetMax(void * const tree)
{
  return splayTreeGetMax(tree);
}

void *
interpGetFirst(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLast(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetNext(void * const tree, const void * const entry)
{
  return splayTreeGetNext(tree, entry);
}

void *
interpGetPrevious(void * const tree, const void * const entry)
{
  return splayTreeGetPrevious(tree, entry);
}

void *
interpGetUpper(void * const tree, const void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLower(void * const tree, const void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

bool
interpWalk(void * const tree, const interpWalkFunc_t walk)
{
  return splayTreeWalk(tree, walk);
}

bool
interpCheck(void * const tree)
{
  return splayTreeCheck(tree);
}

bool
interpSort(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

bool
interpBalance(void * const tree)
{
  return splayTreeBalance(tree);
}

void 
interpPop(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return;
}

void *
interpPeek(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return 0;
}

void *
interpPush(void * const tree, void * const entry)
{
  (void)tree;
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
