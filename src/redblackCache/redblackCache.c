/**
 * \file redblackCache.c 
 *
 * A cache implementation using a red-black tree.
 */

/*
 *  This code is a very heavily modified version of the macros in 
 *  the NetBSD source file tree.h. Here are the opening comments from 
 *  that file:
 *
 *
 *  $NetBSD: tree.h,v 1.9 2005/02/26 22:25:34 perry Exp $ 
 *  $OpenBSD: tree.h,v 1.7 2002/10/17 21:51:54 art Exp $
 *
 * Copyright 2002 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* From the <a href="http://en.wikipedia.org/wiki/Red-black_tree">
 * Wikipedia</a> entry:
 *
 * A red-black tree is a binary search tree where each node has 
 * a color attribute, the value of which is either red or black. 
 * In addition to the ordinary requirements imposed on binary search 
 * trees, we make the following additional requirements of any valid 
 * red-black tree:
 *
 *  1. A node is either red or black.\n
 *  2. The root is black.\n
 *  3. All leaves are black.\n
 *  4. Both children of every red node are black.\n
 *  5. All paths from any given node to its leaf nodes contain the 
 *     same number of black nodes.
 *
 * These constraints enforce a critical property of red-black trees: 
 * that the longest possible path from the root to a leaf is no more 
 * than twice as long as the shortest possible path. The result is that 
 * the tree is roughly balanced. Since operations such as inserting,
 * deleting, and finding values requires worst-case time proportional 
 * to the height of the tree, this theoretical upper bound on the 
 * height allows red-black trees to be efficient in the worst-case, 
 * unlike ordinary binary search trees. 
 *
 * To see why these properties guarantee this, it suffices to note
 * that no path can have two red nodes in a row, due to property 4. 
 * The shortest possible path has all black nodes, and the longest
 * possible path alternates between red and black nodes. Since all
 * maximal paths have the same number of black nodes, by property 5,
 * this shows that no path is more than twice as long as any other
 * path.
 *
 * In many presentations of tree data structures, it is possible for
 * a node to have only one child, and leaf nodes contain data. It is
 * possible to present red-black trees in this paradigm, but it
 * changes several of the properties and complicates the algorithms.
 * For this reason, we use "null leaves", which contain no data and
 * merely serve to indicate where the tree ends.  A consequence of
 * this is that all internal (non-leaf) nodes have two children,
 * although one or more of those children may be a null leaf. A 
 * threaded red-black tree uses the pointer space in the leaf nodes
 * to build an ordered linked list of internal nodes. An extra tag
 * is required for each node to distinguish between internal and 
 * thread nodes.
 *
 * <a href="http://en.wikipedia.org/wiki/Wikipedia:Text_of_the_GNU_Free_Documentation_License">GNU Free Documentation License</a>
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "redblackTree.h"
#include "redblackCache.h"
#include "redblackCache_private.h"

/**
 * Private helper function for red-black tree cache implementation.
 *
 * \param amount of memory requested
 * \param user \c void pointer to user data to be echoed by callbacks
 * \return pointer to the memory allocated. \c NULL indicates failure.
 */
void *
redblackCacheAllocFunc(const size_t amount, void * const user)
{
  redblackCache_t *cache = user;

  return (cache->alloc)(amount, cache->user);
}

/**
 * Private helper function for red-black tree cache implementation.
 *
 * Memory deallocation call-back.
 *
 * \param pointer to memory to be deallocated
 * \param user \c void pointer to user data to be echoed by callbacks
 */
void 
redblackCacheDeallocFunc(void * const pointer, void * const user)
{
  redblackCache_t *cache = user;

  (cache->dealloc)(pointer, cache->user);
  return;
}


/**
 * Private helper function for red-black tree cache implementation.
 *
 * Callback function to output a debugging message. Accepts a 
 * variable argument list like \c printf(). 
 *
 * \param function name
 * \param line number
 * \param user \c void pointer to user data to be echoed by callbacks
 * \param format string
 * \param ... variable argument list
 */
void 
redblackCacheDebugFunc(const char *function, 
                       const unsigned int line, 
                       void * const user,
                       const char *format, 
                       ...)
{
  va_list ap;
  const redblackCache_t *cache = user;

  va_start (ap, format);
  (cache->debug)(function, line, cache->user, format, ap);
  va_end(ap);
}

/**
 * Private helper function for red-black tree cache implementation.
 * 
 * Callback function to compare two entries when searching in the cache. 
 * The entry type is defined by the caller.
 *
 * \param a pointer to an entry defined by the caller
 * \param b pointer to an entry defined by the caller
 * \param user \c void pointer to user data to be echoed by callbacks
 * \return \c compare_e value
 */
compare_e
redblackCacheCompFunc(const void * const a,
                      const void * const b,
                      void * const user)
{
  const redblackCacheEntry_t * const A=a;
  const redblackCacheEntry_t * const B=b;
  redblackCache_t * const cache = user;

  return (cache->compare)(A->entry, B->entry, cache->user);
}

/**
 * Private helper function for red-black tree cache implementation.
 *
 * Move the cache entry (should be the last in the list) to the head 
 * of the linked list. 
 *
 * \param cache pointer to \c redblackCache_t
 * \param cacheEntry pointer to \c redblackTreeEntry_t node to be moved
 * \return pointer to \c redblackCache_t. \c NULL indicates failure
 */
static
redblackCache_t *
redblackCacheMoveToHead(redblackCache_t * const cache, 
                        redblackCacheEntry_t *cacheEntry)
{
  if (cache == NULL)
    {
      return NULL;
    }
  if ((cacheEntry->next == NULL) || (cacheEntry->prev == NULL))
    {
      return NULL;
    }

  /* Unlink cacheEntry */
  (cacheEntry->prev)->next = cacheEntry->next;
  (cacheEntry->next)->prev = cacheEntry->prev;

  /* Update cacheEntry */
  cacheEntry->next = (cache->head).next;
  cacheEntry->prev = &(cache->head);

  /* Link cacheEntry */
  (cacheEntry->next)->prev = (cache->head).next = cacheEntry;

  return cache;
}

/**
 * Private helper function for red-black cache implementation.
 *
 *  Duplicate an entry.
 *
 * \param cache pointer to an instance of \e redblackCache_t
 * \param entry pointer to caller's data 
 * \return pointer to duplicated entry
 */
static
void *
redblackCacheDuplicateEntry(redblackCache_t * const cache, void *entry)
{
  void *new_entry;
  if (cache == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      cache->debug(__func__, __LINE__, cache->user, "Invalid entry==NULL");
      return NULL;
    }

  if (cache->duplicateEntry != NULL)
    {
      new_entry = cache->duplicateEntry(entry, cache->user);
      if (new_entry == NULL)
        {
          cache->debug(__func__, __LINE__, cache->user, 
                      "Couldn't duplicate entry!");
          return NULL;
        }   
    }
  else 
    {
      new_entry = entry;
    }

  return new_entry;
}

redblackCache_t *
redblackCacheCreate(const size_t space,   
                    const redblackCacheAllocFunc_t alloc, 
                    const redblackCacheDeallocFunc_t dealloc,
                    const redblackCacheDuplicateEntryFunc_t duplicateEntry,
                    const redblackCacheDeleteEntryFunc_t deleteEntry,
                    const redblackCacheDebugFunc_t debug,
                    const redblackCacheCompFunc_t comp,
                    void * const user)
{
  redblackCache_t *cache = NULL;

  if (debug == NULL)
    {
      return NULL;
    }
  if (space == 0)
    {
      debug(__func__, __LINE__, user, "Invalid space==0 requested!");
      return NULL;
    }
  if (alloc == NULL)
    {
      debug(__func__, __LINE__, user, "Invalid comp() function!");
      return NULL;
    }
  if (dealloc == NULL)
    {
      debug(__func__, __LINE__, user, "Invalid comp() function!");
      return NULL;
    }
  if (comp == NULL)
    {
      debug(__func__, __LINE__, user, "Invalid comp() function!");
      return NULL;
    }

  /* Allocate space for redblackCache_t */
  cache = alloc(sizeof(redblackCache_t), user);
  if (cache == NULL)
    {
      debug(__func__, __LINE__, user,
            "Can't allocate %d for redblackCache_t",
            sizeof(redblackCache_t));
      return NULL;
    }

  if (cache->tree == NULL)
    {
      dealloc(cache, user);
      debug(__func__, __LINE__, user,
            "Can't create redblackCache_t red-black tree");
      return NULL;
    }

  /* Initialise cache */
  cache->head.prev = NULL;
  cache->head.next = &(cache->tail);
  cache->head.entry = NULL;
  cache->tail.prev = &(cache->head);
  cache->tail.next = NULL;
  cache->tail.entry = NULL;
  cache->alloc = alloc;
  cache->dealloc = dealloc;
  cache->duplicateEntry = duplicateEntry;
  cache->deleteEntry = deleteEntry;
  cache->debug = debug;
  cache->compare = comp;
  cache->user = user;
  cache->space = space;
  cache->size = 0;

  /* Create red-black tree. 
   *
   * Note that cache passes itself to the redblackTree_t callbacks!
   */
  cache->tree = redblackTreeCreate(redblackCacheAllocFunc, 
                                   redblackCacheDeallocFunc,
                                   NULL,
                                   NULL,
                                   redblackCacheDebugFunc, 
                                   redblackCacheCompFunc,
                                   cache);
  if (cache->tree == NULL)
    {
      dealloc(cache, user);
      debug(__func__, __LINE__, user,
            "Can't create redblackCache_t red-black tree");
      return NULL;
    }

  return cache;
}

void *
redblackCacheFind(redblackCache_t * const cache, void * const entry)
{
  redblackCacheEntry_t *cacheEntry, dummyCacheEntry;

  if (cache == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      return NULL;
    }

  /* First see if we find the entry in the cache */
  dummyCacheEntry.entry = entry;
  if ((cacheEntry = redblackTreeFind(cache->tree, &dummyCacheEntry)) == NULL)
    {
      return NULL;
    }

  /* Found! Move it to the head of the list */
  if (redblackCacheMoveToHead(cache, cacheEntry) == NULL)
    {
      cache->debug(__func__, __LINE__, cache->user,
                   "redblackCacheMoveToHead() failed");
      return NULL;
    }

  return cacheEntry->entry;
}

void *
redblackCacheInsert(redblackCache_t * const cache, void * const entry)
{
  redblackCacheEntry_t *cacheEntry, dummyCacheEntry;

  if (cache == NULL) 
    {
      return NULL;
    }
  if (cache->alloc == NULL)
    {
      cache->debug(__func__, __LINE__, cache->user, 
                   "Invalid cache->compare() function!");
      return NULL;
    }
  if (entry == NULL)
    {
      cache->debug(__func__, __LINE__, cache->user, "Invalid entry==NULL");
      return NULL;
    }

  /* First see if we can find the entry in the tree */
  dummyCacheEntry.entry = entry;
  if ((cacheEntry = redblackTreeFind(cache->tree, &dummyCacheEntry)) != NULL)
    { 
      /* Move it to the head of the list */
      if (redblackCacheMoveToHead(cache, cacheEntry) == NULL)
        {
          cache->debug(__func__, __LINE__, cache->user,
                       "redblackCacheMoveToHead() failed");
          return NULL;
        }

      /* Callback to delete old entry */
      if (cache->deleteEntry != NULL)
        {
          (cache->deleteEntry)(cacheEntry->entry, cache->user);
        }
     
      /* New key/value? */
      cacheEntry->entry = redblackCacheDuplicateEntry(cache, entry);
      if (cacheEntry->entry == NULL)
        {
          cache->debug(__func__, __LINE__, cache->user, 
                       "Couldn't duplicate entry!");
              return NULL;
        }
      return cacheEntry->entry;
    }

  /* Entry not found. Install in the cache in new memory. */
  if (cache->size < cache->space)
    {
      /* Allocate memory for redblackCacheEntry_t */
      if ((cacheEntry = (cache->alloc)(sizeof(redblackCacheEntry_t),
                                       cache->user)) == NULL)
        {
          cache->debug(__func__, __LINE__, cache->user,
                       "Memory allocation for cacheEntry failed!");
          return NULL;
        }

      /* Install the new entry */
      cacheEntry->entry = redblackCacheDuplicateEntry(cache, entry);
      if (cacheEntry->entry == NULL)
        {
          cache->dealloc(cacheEntry, cache->user);
          cache->debug(__func__, __LINE__, cache->user, 
                       "Couldn't duplicate entry!");
          return NULL;
        }

      /* Install in cache */
      if (redblackTreeInsert(cache->tree, cacheEntry) == NULL)
        {
          cache->debug(__func__, __LINE__, cache->user,
                       "redblackTreeInsert() failed");
          return NULL;
        }

      /* Insert cacheEntry at head of list */
      cacheEntry->next = (cache->head).next;
      cacheEntry->prev = &(cache->head);
      (cacheEntry->next)->prev = (cache->head).next = cacheEntry;

      /* Count the new entry */
      cache->size = cache->size + 1;

      /* Done */
      return cacheEntry->entry;
    }
  else
    {
      /* Reuse the last cache entry */
      cacheEntry = (cache->tail).prev;
      redblackTreeRemove(cache->tree, cacheEntry);

      /* Callback to delete old entry */
      if (cache->deleteEntry != NULL)
        {
          (cache->deleteEntry)(cacheEntry->entry, cache->user);
        }

      /* Install the new entry */
      cacheEntry->entry = redblackCacheDuplicateEntry(cache, entry);
      if (cacheEntry->entry == NULL)
        {
          cache->dealloc(cacheEntry, cache->user);
          cache->debug(__func__, __LINE__, cache->user, 
                       "Couldn't duplicate entry!");
          return NULL;
        }
      redblackTreeInsert(cache->tree, cacheEntry);
      
      /* Move it to the start of the cache list */
      if (redblackCacheMoveToHead(cache, cacheEntry) == NULL)
        {
          cache->debug(__func__, __LINE__, cache->user,
                       "redblackCacheMoveToHead() failed");
          return NULL;
        }

      return cacheEntry->entry;
    }
}

void 
redblackCacheClear(redblackCache_t * const cache)
{
  redblackCacheEntry_t *cacheEntry, *nextCacheEntry;

  if (cache == NULL)
    {
      return;
    }

  /* Clear the tree */
  if (cache->tree != NULL)
    {
      redblackTreeClear(cache->tree);
    }

  /* Clear the list */
  cacheEntry = cache->head.next;
  while (cacheEntry != &(cache->tail))
    {
      nextCacheEntry = cacheEntry->next;
      if (cache->deleteEntry != NULL)
        {
          (cache->deleteEntry)(cacheEntry->entry, cache->user);
        }
      (cache->dealloc)(cacheEntry, cache->user);
      cacheEntry = nextCacheEntry;
    }

  cache->head.next = &(cache->tail);
  cache->tail.prev = &(cache->head);
  cache->size = 0;
}

void 
redblackCacheDestroy(redblackCache_t * const cache)
{
  if ((cache == NULL) || (cache->dealloc == NULL))
    {
      return;
    }

  redblackCacheClear(cache); /* Do this first! */
  redblackTreeDestroy(cache->tree);
  (cache->dealloc)(cache, cache->user);

  return;
}

size_t
redblackCacheGetSize(const redblackCache_t * const cache)
{
  if (cache == NULL)
    {
      return 0;
    }

  return cache->size;
}

size_t
redblackCacheGetDepth(const redblackCache_t * const cache)
{
  if ((cache == NULL) || (cache->tree == NULL))
    {
      return 0;
    }

  return redblackTreeGetDepth(cache->tree);
}

bool
redblackCacheWalk(redblackCache_t * const cache, 
                  const redblackCacheWalkFunc_t walk)
{
  redblackCacheEntry_t *cacheEntry, *nextCacheEntry;

  if ((cache == NULL) || (walk == NULL))
    {
      return false;
    }

  cacheEntry = cache->head.next;
  while (cacheEntry != &(cache->tail))
    {
      nextCacheEntry = cacheEntry->next;
      if (walk(cacheEntry->entry, cache->user) == false)
        {
          return false;
        }

      cacheEntry = nextCacheEntry;
    }

  return true;
}

bool 
redblackCacheCheck(redblackCache_t * const cache)
{
  if ((cache == NULL) || (cache->tree == NULL))
    {
      return false;
    }

  return redblackTreeCheck(cache->tree);
}

