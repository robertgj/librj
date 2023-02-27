/*
 * swTree_wrapper.c
 *
 * Wrapper functions for simple interpreter interface.
 */

#include <stdlib.h>
#include <stdarg.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_wrapper.h"
#include "swTree.h"

void *
interpCreate(void)
{
  return swTreeCreate(interpAlloc,
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
  return swTreeFind(tree, entry);
}

void *
interpInsert(void * const tree, void * const entry)
{
  return swTreeInsert(tree, entry);
}

void *
interpRemove(void * const tree, void * const entry)
{
  return swTreeRemove(tree, entry);
}

void 
interpClear(void * const tree)
{
  swTreeClear(tree);
}

void 
interpDestroy(void * const tree)
{
  swTreeDestroy(tree);
}

size_t
interpGetDepth(const void * const tree)
{
  return swTreeGetDepth(tree);
}

size_t
interpGetSize(const void * const tree)
{
  return swTreeGetSize(tree);
}

void *
interpGetMin(void * const tree)
{
  return swTreeGetMin(tree);
}

void *
interpGetMax(void * const tree)
{
  return swTreeGetMax(tree);
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
interpGetNext(void * const tree,  const void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetPrevious(void * const tree,  const void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetUpper(void * const tree,  const void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpGetLower(void * const tree,  const void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

bool
interpWalk(void * const tree, const interpWalkFunc_t walk)
{
  return swTreeWalk(tree, walk);
}

bool
interpCheck(void * const tree)
{
  return swTreeCheck(tree);
}

bool
interpSort(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return false;
}

bool
interpBalance(void * const tree)
{
  (void)tree;
  interpError(__func__, __LINE__, "Not implemented!");
  return false;
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
  return NULL;
}

void *
interpPush(void * const tree, void * const entry)
{
  (void)tree;
  (void)entry;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}

void *
interpCopy(void * const dst, void * const src)
{
  (void)dst;
  (void)src;
  interpError(__func__, __LINE__, "Not implemented!");
  return NULL;
}
