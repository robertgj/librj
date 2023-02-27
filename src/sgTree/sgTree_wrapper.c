/*
 * sgTree_wrapper.c
 *
 * Wrapper functions for simple interpreter interface.
 */

#include <stdlib.h>
#include <stdarg.h>

#include "interp_utility.h"
#include "interp_callbacks.h"
#include "interp_wrapper.h"
#include "sgTree.h"

void *
interpCreate(void)
{
  return sgTreeCreate(interpAlloc,
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
  return sgTreeFind(tree, entry);
}

void *
interpInsert(void * const tree, void * const entry)
{
  return sgTreeInsert(tree, entry);
}

void *
interpRemove(void * const tree, void * const entry)
{
  return sgTreeRemove(tree, entry);
}

void 
interpClear(void * const tree)
{
  sgTreeClear(tree);
}

void 
interpDestroy(void * const tree)
{
  sgTreeDestroy(tree);
}

size_t
interpGetDepth(const void * const tree)
{
  return sgTreeGetDepth(tree);
}

size_t
interpGetSize(const void * const tree)
{
  return sgTreeGetSize(tree);
}

void *
interpGetMin(void * const tree)
{
  return sgTreeGetMin(tree);
}

void *
interpGetMax(void * const tree)
{
  return sgTreeGetMax(tree);
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
  return sgTreeWalk(tree, walk);
}

bool
interpCheck(void * const tree)
{
  return sgTreeCheck(tree);
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
