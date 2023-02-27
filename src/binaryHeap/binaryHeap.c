/** 
 * \file  binaryHeap.c
 *
 * A \e binaryHeap_t implementation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "binaryHeap.h"
#include "binaryHeap_private.h"

/**
 * Private helper function for binary heap implementation.
 *
 * Find the left child of a binary heap index.
 *
 * \param i binary heap parent index.
 * \return left child index of binary heap index.
 */
static size_t 
binaryHeapLeft(const size_t i)
{
  return (2*i)+1;
}

/**
 * Private helper function for binary heap implementation.
 *
 * Find the right child index of a binary heap index.
 *
 * \param i binary heap parent index.
 * \return right child index of a binary heap index.
 */
static size_t 
binaryHeapRight(const size_t i)
{
  return 2*(i+1);
}

/**
 * Private helper function for binary heap implementation.
 *
 * Find the parent index of a binary heap index.
 *
 * \param i binary heap index.
 * \return parent index of binary heap index.
 */
static size_t 
binaryHeapParent(const size_t i)
{
  if (i == 0)
    {
      return 0;
    }
  else 
    {
      return (i-1)/2;
    }
}

/**
 * Private helper function for binary heap implementation.
 *
 * Swap the entries in a binary heap.
 *
 * \param binaryHeap pointer to \e binaryHeap_t
 * \param i index of an entry in the binary heap
 * \param j index of an entry in the binary heap
 */
static void
binaryHeapSwap(binaryHeap_t * const binaryHeap, const size_t i, const size_t j)

{
  void *tmp;

  if (binaryHeap == NULL)
    {
      return;
    }

  if ((i >= binaryHeap->size) || (j >= binaryHeap->size))
    {
      binaryHeap->debug(__func__, __LINE__, binaryHeap->user, 
                        "invalid heap index!");
      return;
    }

  tmp = (binaryHeap->heap)[i];
  (binaryHeap->heap)[i] = (binaryHeap->heap)[j];
  (binaryHeap->heap)[j] = tmp;
}

/**
 * Private helper function for binary heap implementation.
 *
 * Compare the entries in a binary heap. 
 *
 * \param binaryHeap pointer to \e binaryHeap_t
 * \param l index of an entry in the binary heap
 * \param r index of an entry in the binary heap
 * \return true if heap[l] > heap[r]. Otherwise false.
 */
static bool 
binaryHeapCompareGreater(binaryHeap_t * const binaryHeap, 
                         const size_t l, const size_t r)
{
  if (binaryHeap == NULL)
    {
      return false;
    }

  if ((l >= binaryHeap->size) || (r >= binaryHeap->size))
    {
      binaryHeap->debug(__func__, __LINE__, binaryHeap->user, 
                        "invalid heap index!");
      return false;
    }

  if ((binaryHeap->compare)
      ((binaryHeap->heap)[l], (binaryHeap->heap)[r], binaryHeap->user) 
      == compareGreater)
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Private helper function for binary heap implementation.
 *
 * Move an entry up the binary heap to a location in which it 
 * satisfies the heap property. 
 *
 * \param binaryHeap pointer to \e binaryHeap_t
 * \param i index of an entry in the binary heap
 */
static void
binaryHeapBubbleUp(binaryHeap_t * const binaryHeap, size_t i)
{
  if (binaryHeap == NULL)
    {
      return;
    }

  size_t p = binaryHeapParent(i);
  while((i>0) && ((binaryHeapCompareGreater)(binaryHeap, p, i) == true))
    {
      binaryHeapSwap(binaryHeap, p, i);
      i = p;
      p = binaryHeapParent(i);
    }

  return;
}

/**
 * Private helper function for binary heap implementation.
 *
 * Move an entry down the binary heap to a location in which it
 * satisfies the heap property. 
 *
 * \param binaryHeap pointer to \e binaryHeap_t
 * \param i index of an entry in the binary heap
 */
static void
binaryHeapTrickleDown(binaryHeap_t * const binaryHeap, size_t i)
{
  bool do_swap;

  /* Sanity checks */
  if (binaryHeap == NULL)
    {
      return;
    }

  if  ((binaryHeap->size == 0) || (i == (binaryHeap->size-1)))
    {
      return;
    }

  if (i >= binaryHeap->size)
    {
      binaryHeap->debug(__func__, __LINE__, binaryHeap->user, 
                        "i(%zd) >= binaryHeap->size(%zd)!", 
                        i, binaryHeap->size);
      return;
    }

  /* Move the i'th entry down the heap */
  do
    {
      size_t n,r,l;

      do_swap = false;

      /* Compare parent with right child */
      r = binaryHeapRight(i);
      if ((r < binaryHeap->size) && 
          (binaryHeapCompareGreater(binaryHeap, i, r) == true))
        {
          do_swap = true;
          /* Compare right and left children */
          l = binaryHeapLeft(i);
          if (binaryHeapCompareGreater(binaryHeap, r, l) == true)
            {
              n = l;
            }
          else
            {
              n = r;
            }
        }
      /* Compare parent with left child */
      else
        {
          l = binaryHeapLeft(i);
          if ((l < binaryHeap->size) && 
              binaryHeapCompareGreater(binaryHeap, i, l) == true)
            {
              do_swap = true;
              n = l;
            }
        }

      /* If necessary, swap the entries */
      if (do_swap == true)
        {
          binaryHeapSwap(binaryHeap, i, n);
          i = n;
        }
    }
  while (do_swap == true);
 
  return;
}

binaryHeap_t *
binaryHeapCreate(const binaryHeapAllocFunc_t alloc, 
                 const binaryHeapDeallocFunc_t dealloc, 
                 const binaryHeapDuplicateEntryFunc_t duplicateEntry, 
                 const binaryHeapDeleteEntryFunc_t deleteEntry, 
                 const binaryHeapDebugFunc_t debug,
                 const binaryHeapCompFunc_t compare,
                 void * const user)
{
  binaryHeap_t *binaryHeap = NULL;

  if (debug == NULL)
    {
      return NULL;
    }
  if (alloc == NULL)
    {
      debug(__func__, __LINE__, user, "Invalid alloc() function!");
      return NULL;
    }
  if (dealloc == NULL)
    {
      debug(__func__, __LINE__, user, "Invalid dealloc() function!");
      return NULL;
    }
  if (compare == NULL)
    {
      debug(__func__, __LINE__, user, "Invalid compare() function!");
      return NULL;
    }

  /* Allocate binary heap */
  binaryHeap = (binaryHeap_t *)alloc(sizeof(binaryHeap_t), user);
  if (binaryHeap == NULL)
    {
      debug(__func__, __LINE__, user, 
            "Couldn't allocate %zd bytes for binaryHeap!", 
            sizeof(binaryHeap_t));
      return NULL;
    }

  /* Allocate heap state */
  (binaryHeap->heap) = (void **)alloc(sizeof(void*),user);
  if (binaryHeap->heap == NULL)
    {
      dealloc(binaryHeap, user);
      debug(__func__, __LINE__, user, 
            "Couldn't allocate %zd bytes for binaryHeap->heap!", 
            sizeof(void *));

      return NULL;
    }

  /* Initalise new binaryHeap_t */
  binaryHeap->alloc = alloc;
  binaryHeap->dealloc = dealloc;
  binaryHeap->duplicateEntry = duplicateEntry;
  binaryHeap->deleteEntry = deleteEntry;
  binaryHeap->debug = debug;
  binaryHeap->compare = compare;
  binaryHeap->size = 0;
  binaryHeap->space = 1;
  binaryHeap->user = user;
  
  return binaryHeap;
}

void *
binaryHeapPush(binaryHeap_t * const binaryHeap, void * const entry)
{
  if (binaryHeap == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      binaryHeap->debug(__func__, __LINE__, binaryHeap->user, 
                        "invalid entry == NULL!");
      return NULL;
    }

  if (binaryHeap->size == binaryHeap->space)
    {
      void **new_heap = NULL;
      size_t new_space = 2*binaryHeap->space;
      size_t new_alloc = sizeof(void *)*new_space;

      new_heap = (void **)binaryHeap->alloc(new_alloc, 
                                                        binaryHeap->user);
      if (new_heap == NULL)
        {
          binaryHeap->debug(__func__, __LINE__, binaryHeap->user, 
                "Couldn't allocate %zd bytes for new binaryHeap->heap!", 
                new_alloc);
          return NULL;
        }

      memcpy(new_heap, binaryHeap->heap, 
             sizeof(void *)*binaryHeap->space);
      binaryHeap->dealloc(binaryHeap->heap, binaryHeap->user);
      binaryHeap->heap = new_heap;

      binaryHeap->space = new_space;
    }

  void *new_entry;
  if (binaryHeap->duplicateEntry != NULL)
    {
      new_entry = binaryHeap->duplicateEntry(entry, binaryHeap->user);
      if (new_entry == NULL)
        {
          binaryHeap->dealloc(new_entry, binaryHeap->user);
          binaryHeap->debug(__func__, __LINE__, binaryHeap->user, 
                            "Couldn't duplicate entry!");
          return NULL;
        }
    }
  else
    {
      new_entry = entry;
    }
  (binaryHeap->heap)[binaryHeap->size] = new_entry;
  binaryHeap->size += 1;
  binaryHeapBubbleUp(binaryHeap, (binaryHeap->size)-1);

  return new_entry;
}

void *
binaryHeapPeek(binaryHeap_t * const binaryHeap)
{
  if (binaryHeap == NULL)
    {
      return NULL;
    }
  if (binaryHeap->heap == NULL)
    {
      binaryHeap->debug(__func__, __LINE__, binaryHeap->user, 
                        "binaryHeap->heap == NULL!");
      return NULL;
    }
  if (binaryHeap->size == 0)
    {
      return NULL;
    }

  return (binaryHeap->heap)[0];
}

void *
binaryHeapPop(binaryHeap_t * const binaryHeap)
{
  if (binaryHeap == NULL)
    {
      return NULL;
    }
  if (binaryHeap->size == 0)
    {
      return NULL;
    }

  void *entry = (binaryHeap->heap)[0];
  if (binaryHeap->deleteEntry != NULL)
    {
      (binaryHeap->deleteEntry)(entry, binaryHeap->user);
      entry = NULL;
    }

  (binaryHeap->heap)[0] = (binaryHeap->heap)[(binaryHeap->size)-1];
  binaryHeap->size -= 1;
  binaryHeapTrickleDown(binaryHeap, 0);

  return entry;
}

void
binaryHeapClear(binaryHeap_t * const binaryHeap)
{
  if (binaryHeap == NULL)
    {
      return;
    }
  if (binaryHeap->size == 0)
    {
      return;
    }
  if (binaryHeap->deleteEntry != NULL)
    {
      size_t heapSize = binaryHeap->size;
      for (size_t i=1; i<=heapSize;i++)
        {
          void *entry = (binaryHeap->heap)[heapSize-i];
          binaryHeap->deleteEntry(entry,binaryHeap->user);
        }
    }

  binaryHeap->size = 0;

 return;
}

void 
binaryHeapDestroy(binaryHeap_t * const binaryHeap)
{
  if (binaryHeap == NULL)
    {
      return;
    }

  /* Get rid of all entries */
  binaryHeapClear(binaryHeap);
  
  /* Free the binaryHeap_t */
  (binaryHeap->dealloc)(binaryHeap->heap, binaryHeap->user);
  (binaryHeap->dealloc)(binaryHeap, binaryHeap->user);
}

size_t 
binaryHeapGetSize(const binaryHeap_t * const binaryHeap)
{
  if (binaryHeap == NULL)
    {
      return 0;
    }

  return binaryHeap->size;
}

size_t 
binaryHeapGetDepth(const binaryHeap_t * const binaryHeap)
{
  if (binaryHeap == NULL)
    {
      return 0;
    }

  binaryHeap->debug(__func__, __LINE__, binaryHeap->user, 
                    "binaryHeapGetDepth() not implemented!");

  return 0;
}

bool
binaryHeapWalk(binaryHeap_t * const binaryHeap, const binaryHeapWalkFunc_t walk)
{
  if (binaryHeap == NULL)
    {
      return false;
    }
  if (walk == NULL)
    {
      binaryHeap->debug(__func__, __LINE__, binaryHeap->user, 
                        "invalid walk() function!");
      return false;
    }

  for (size_t i=0; i<binaryHeap->size; i++)
    {
      fprintf(stdout,"Node %zu : parent %zu : ", i, binaryHeapParent(i));
      if (binaryHeapLeft(i) < binaryHeap->size)
        {
          fprintf(stdout,"children %zu ", binaryHeapLeft(i));
        }
      if (binaryHeapRight(i) < binaryHeap->size)
        {
          fprintf(stdout," , %zu", binaryHeapRight(i));
        }
      fprintf(stdout," : ");

      bool res = walk((binaryHeap->heap)[i], binaryHeap->user);
      if (res != true)
        {
          binaryHeap->debug(__func__, __LINE__, binaryHeap->user, 
                            "Walk function failed at heap index %zd!", i);
          return false;
        }
      fflush(stdout);
    }

  return true;
}

bool
binaryHeapCheck(binaryHeap_t * const binaryHeap)
{
  if (binaryHeap == NULL)
    {
      return false;
    }

  for (size_t i=0; binaryHeapRight(i) < binaryHeap->size; i++)
    {
      if ( binaryHeapLeft(i) < binaryHeap->size)
        {
          if (binaryHeapCompareGreater(binaryHeap, i, binaryHeapLeft(i)))
            {
              binaryHeap->debug
                (__func__, __LINE__, binaryHeap->user, 
                 "Compare failed at parent %zd, left child %zd\n",
                 i, binaryHeapLeft(i));
              return false;
            }
        }
      if (binaryHeapRight(i) < binaryHeap->size)
        {
          if (binaryHeapCompareGreater(binaryHeap, i, binaryHeapRight(i)))
            {
              binaryHeap->debug
                (__func__, __LINE__, binaryHeap->user, 
                 "Compare failed at parent %zd, right child %zd\n",
                 i, binaryHeapRight(i));
              return false;
            }
        }
    }

  return true;
}

