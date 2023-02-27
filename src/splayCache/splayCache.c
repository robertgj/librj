/**
 * \file splayCache.c 
 *
 * A cache_t implementation. \e splayCacheGetDepth() is
 * implemented by recursively descending the tree from the root so
 * that it doesn't modify the tree. There are several ways to avoid
 * this recursion: \n 
 *   - maintain a depth-wise stack of parent nodes. For a splay tree, 
 *     this stack may need to be the same as the number of entries 
 *     in the tree. \n 
 *   - add a parent pointer to each node \n
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "splayCache.h"
#include "splayCache_private.h"

/**
 * Private helper function for splay tree cache implementation.
 *
 * Rotate node right maintaining order.
 *
 \verbatim
   root             l
    /\     -->     /\
   l  r          ll  root
  /\                  /\
ll  rr              rr  r
 \endverbatim
 *
 * \param cache pointer to \c splayCache_t
 */
static
void
splayCacheRotateRight(splayCache_t * const cache)
{
  splayCacheEntry_t *node = (cache->root)->left;

  (cache->root)->left = node->right;
  node->right = cache->root;
  cache->root = node;
}

/**
 * Private helper function for splay tree cache implementation
 *
 * Rotate node left maintaining order.
 *
 \verbatim
   root               r
    /\     -->        /\
   l  r           root  rr
      /\           /\
    ll  rr        l  ll
 \endverbatim
 *
 * \param cache pointer to \c splayCache_t
 */
static
void
splayCacheRotateLeft(splayCache_t * const cache)
{
  splayCacheEntry_t *node = (cache->root)->right;

  (cache->root)->right = node->left;
  node->left = cache->root;
  cache->root = node;
}

/**
 * Private helper function for splay tree cache implementation.
 *
 * Link root to right child of node, which becomes root.
 * 
 \verbatim
   left  root  right        left      B     right
          /\         --->            /\
         A  B                          root
                                        /
                                       A
 \endverbatim
 *
 * \param cache pointer to \c splayCache_t
 * \param node pointer to pointer to \c splayCacheEntry_t to be moved to root
 */
static
void
splayCacheLinkRight(splayCache_t * const cache, splayCacheEntry_t ** const node)
{
  (*node)->right = cache->root;
  *node = cache->root;
  cache->root = (cache->root)->right;
}

/**
 * Private helper function for splay tree cache implementation.
 *
 * Link root to left child of node, which becomes root.
 *
 \verbatim
 left  root  right         left   A      right
        /\           --->        /\
       A  B                   root
                                \
                                 B
 \endverbatim
 *
 * \param cache pointer to \c splayCache_t
 * \param node pointer to pointer to \c splayCacheEntry_t to be moved to root
 */
static
void
splayCacheLinkLeft(splayCache_t * const cache, splayCacheEntry_t ** const node)
{
  (*node)->left = cache->root;
  *node = cache->root;
  cache->root = (cache->root)->left;
}

/**
 * Private helper function for splay tree cache implementation.
 *
 * Attach left and right sub-trees to root.
 *
 \verbatim
 left  root  right           root
        /\                    /\
       A  B        --->   left  right
                            \     /
                             A   B
 \endverbatim
 *
 * \param cache pointer to \c splayTree_t
 * \param node pointer to \c splayTreeEntry_t 
 * \param left pointer to \c splayTreeEntry_t left hand tree
 * \param right pointer to \c splayTreeEntry_t right hand tree
 */
static
void
splayCacheAssemble(splayCache_t * const cache, 
                   splayCacheEntry_t * const node, 
                   splayCacheEntry_t * const left, 
                   splayCacheEntry_t * const right)
{
  left->right = (cache->root)->left;
  right->left = (cache->root)->right;
  (cache->root)->left = node->right;
  (cache->root)->right = node->left;
}

/**
 * Private helper function for splay tree cache implementation.
 *
 * Splay the entry to the root of the tree. 
 *
 * \param cache pointer to \c splayCache_t
 * \param entry \c void pointer to caller's \c entry data
 */
static
void
splayCacheSplay(splayCache_t * const cache, void * const entry) 
{
  splayCacheEntry_t node, *left, *right;
  compare_e comp;

  node.left = node.right = NULL;
  left = right = &node;
  while ((comp = (cache->compare)(entry, 
                                  (cache->root)->entry, 
                                  cache->user)) != compareEqual)
    {
      if (comp == compareLesser)
        {
          if ((cache->root)->left == NULL) 
            {
              break;
            }
          if ((cache->compare)(entry, 
                               ((cache->root)->left)->entry, 
                               cache->user) == compareLesser)
            {
              splayCacheRotateRight(cache);
              if ((cache->root)->left == NULL)
                {
                  break;
                }
            }
          splayCacheLinkLeft(cache, &right);
        }
      else if (comp == compareGreater) 
        {
          if ((cache->root)->right == NULL) 
            {
              break;
            }
          if ((cache->compare)(entry, 
                               ((cache->root)->right)->entry,
                               cache->user) == compareGreater)
            {
              splayCacheRotateLeft(cache);
              if ((cache->root)->right == NULL) 
                {
                  break;
                }
            }
          splayCacheLinkRight(cache, &left);
        }
      else
        {
          break;
        }
    }

  splayCacheAssemble(cache, &node, left, right);
  return;
}

/**
 * Private helper function for splay tree cache implementation.
 *
 * Insert an entry into the cache splay tree. 
 *
 * \param cache pointer to \c splayCache_t
 * \param node pointer to node to be installed as tree root node
 * \return pointer to \c splayTreeEntry_t root node. \c NULL indicates
 * failure.
 */
static
splayCacheEntry_t *
splayCacheInsertEntry(splayCache_t * const cache, splayCacheEntry_t * const node)
{
  if ((cache == NULL) || (node == NULL))
    {
      return NULL;
    }

  if(cache->root == NULL)
    {
      node->left = node->right = NULL;
    }
  else 
    {
      compare_e comp;

      splayCacheSplay(cache, node->entry);
      comp = (cache->compare)(node->entry, (cache->root)->entry, cache->user);
      if(comp == compareLesser) 
        {
          node->left = (cache->root)->left;
          node->right = cache->root;
          (cache->root)->left = NULL;
        }
      else if (comp == compareGreater) 
        {
          node->right = (cache->root)->right;
          node->left = cache->root;
          (cache->root)->right = NULL;
        }
      else 
        {
          /* Shouldn't get here! */
          return NULL;
        }
    }

  cache->root = node;

  return node;
}

/**
 * Private helper function for splay tree cache implementation.
 *
 * Remove an entry from the cache splay tree. Doesn't deallocate
 * associated memory.
 *
 * \param cache pointer to \c splayCache_t
 * \param entry \c void pointer to callers entry data
 * \return pointer to \c splayTreeEntry_t node removed from tree
 */
static
splayCacheEntry_t * 
splayCacheRemoveEntry(splayCache_t * const cache, void * const entry) 
{
  if ((cache == NULL) || (entry == NULL) || (cache->root == NULL))
    {
      return NULL;
    }

  splayCacheSplay(cache, entry);
  if ((cache->compare)(entry, (cache->root)->entry, cache->user) 
      == compareEqual) 
    {
      splayCacheEntry_t *node;

      node = cache->root;
      if ((cache->root)->left == NULL) 
        {
          cache->root = (cache->root)->right;
        }
      else 
        {
          splayCacheEntry_t *tmp;

          tmp = (cache->root)->right;
          cache->root = (cache->root)->left;
          splayCacheSplay(cache, entry);
          (cache->root)->right = tmp;
        }

      return node;
    }

  return NULL;
}

/**
 * Private helper function for splay tree cache implementation.
 *
 * Find an entry in the cache splay tree. 
 *
 * \param cache pointer to \c splayCache_t
 * \param entry \c void pointer to callers entry data
 * \return pointer to \c splayTreeEntry_t node with corresponding entry
 */
static
splayCacheEntry_t *
splayCacheFindEntry(splayCache_t * const cache, void * const entry) 
{ 
  if ((cache == NULL) || (cache->root == NULL) || (entry == NULL))
    {
      return NULL;
    }

  splayCacheSplay(cache, entry);
  if ((cache->compare)(entry, (cache->root)->entry, cache->user) 
      == compareEqual) 
    {
      return cache->root;
    }

  return NULL;
}

/**
 * Private helper function for splay tree cache implementation.
 *
 * Recursively find depth of the cache splay tree.
 *
 * \param cacheEntry \c void pointer to callers entry data
 * \return depth so far
 */
static
size_t
splayCacheRecurseDepth(splayCacheEntry_t * const cacheEntry)
{
  if (cacheEntry == NULL)
    {
      return 0;
    }
  else
    {
      size_t leftDepth, rightDepth, maxDepth;

      leftDepth = splayCacheRecurseDepth(cacheEntry->left);
      rightDepth = splayCacheRecurseDepth(cacheEntry->right);
      if (leftDepth > rightDepth)
        {
          maxDepth = leftDepth;
        }
      else
        {
          maxDepth = rightDepth;
        }
      return maxDepth+1;
    }
}

/**
 * Private helper function for splay tree cache implementation.
 *
 * Move the cache entry (should be the last in the list) to the head 
 * of the linked list. 
 *
 * \param cache pointer to \c splayCache_t
 * \param cacheEntry pointer to \c splayTreeEntry_t node to be moved
 * \return pointer to \c splayCache_t. \c NULL indicates failure
 */
static
splayCache_t *
splayCacheMoveToHead(splayCache_t * const cache, 
                     splayCacheEntry_t * const cacheEntry)
{
  if ((cache == NULL) || 
      (cacheEntry->next == NULL) || 
      (cacheEntry->prev == NULL))
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
 * Private helper function for splay tree cache implementation.
 *
 *  Duplicate an entry.
 *
 * \param cache pointer to splayCache_t
 * \param entry pointer to caller's data 
 * \return pointer to duplicated entry
 */
static
void *
splayCacheDuplicateEntry(splayCache_t * const cache, void * const entry)
{
  void *new_entry;

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

splayCache_t *
splayCacheCreate(const size_t space,
                 const splayCacheAllocFunc_t alloc, 
                 const splayCacheDeallocFunc_t dealloc,
                 const splayCacheDuplicateEntryFunc_t duplicateEntry,
                 const splayCacheDeleteEntryFunc_t deleteEntry,
                 const splayCacheDebugFunc_t debug,
                 const splayCacheCompFunc_t comp,
                 void * const user)
{
  splayCache_t *cache = NULL;

  if (debug == NULL)
    {
      return NULL;
    }
  if (space == 0)
    {
      debug(__func__, __LINE__, user, "Requested cache space==0!");
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
  if (comp == NULL)
    {
      debug(__func__, __LINE__, user, "Invalid comp() function!");
      return NULL;
    }

  /* Allocate space for splayCache_t */
  cache = alloc(sizeof(splayCache_t), user);
  if (cache == NULL)
    {
      debug(__func__, __LINE__, user, 
            "Can't allocate %d for splayCache_t",
            sizeof(splayCache_t));
      return NULL;
    }

  /* Initialise cache */
  cache->head.prev = NULL;
  cache->head.next = &(cache->tail);
  cache->head.left = NULL;
  cache->head.right = NULL;
  cache->head.entry = NULL;
  cache->tail.prev = &(cache->head);
  cache->tail.next = NULL;
  cache->tail.left = NULL;
  cache->tail.right = NULL;
  cache->tail.entry = NULL;
  cache->root = NULL;
  cache->compare = comp;
  cache->alloc = alloc;
  cache->dealloc = dealloc;
  cache->duplicateEntry = duplicateEntry;
  cache->deleteEntry = deleteEntry;
  cache->debug = debug;
  cache->size = 0;
  cache->user = user;
  cache->space = space;

  return cache;
}

void *
splayCacheFind(splayCache_t * const cache, void * const entry)
{
  splayCacheEntry_t *cacheEntry;

  if ((cache == NULL) || (entry == NULL))
    {
      return NULL;
    }

  /* First see if we find the entry in the cache */
  if ((cacheEntry = splayCacheFindEntry(cache, entry)) == NULL)
    {
      return NULL;
    }

  /* Found! Move it to the head of the list */
  if (splayCacheMoveToHead(cache, cacheEntry) == NULL)
    {
      cache->debug(__func__, __LINE__, cache->user, 
                   "splayCacheMoveToHead() failed");
      return NULL;
    }

  return cacheEntry->entry;
}

void *
splayCacheInsert(splayCache_t * const cache, void * const entry)
{
  splayCacheEntry_t *cacheEntry;

  if ((cache == NULL) || 
      (cache->alloc == NULL) || 
      (entry == NULL))
    {
      return false;
    }

  /* First see if we can find the entry in the tree */
  if ((cacheEntry = splayCacheFindEntry(cache, entry)) != NULL) 
    { 
      /* Move it to the head of the list */
      if (splayCacheMoveToHead(cache, cacheEntry) == NULL)
        {
          cache->debug(__func__, __LINE__, cache->user, 
                       "splayCacheMoveToHead() failed");
          return NULL;
        }

      /* Callback to delete old entry */
      if (cache->deleteEntry != NULL)
        {
          (cache->deleteEntry)(cacheEntry->entry, cache->user);
        }
     
      /* New key/value? */
      cacheEntry->entry = splayCacheDuplicateEntry(cache, entry);
      if (cacheEntry->entry == NULL)
        {
          (cache->debug)(__func__, __LINE__, cache->user,
                        "splayCacheDuplicateEntry() failed!");
          return NULL;
        }

      return cacheEntry->entry;
    }

  /* Entry not found. Install in the cache */
  if (cache->size < cache->space)
    {
      /* Allocate memory */
      if ((cacheEntry = (cache->alloc)(sizeof(splayCacheEntry_t), 
                                       cache->user)) == NULL)
        {
          return NULL;
        }
      cacheEntry->entry = splayCacheDuplicateEntry(cache, entry);
      if (cacheEntry->entry == NULL)
        {
          (cache->dealloc)(cacheEntry, cache->user);
          (cache->debug)(__func__, __LINE__, cache->user,
                        "splayCacheDuplicateEntry() failed!");
          return NULL;
        }

      /* Install in cache */
      if (splayCacheInsertEntry(cache, cacheEntry) == NULL)
        {
          cache->debug(__func__, __LINE__, cache->user, 
                       "splayCacheInsert() failed");
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
      void *lastEntry;

      /* Reuse the last cache entry */
      cacheEntry = (cache->tail).prev;
      lastEntry = cacheEntry->entry;
      if (splayCacheRemoveEntry(cache, lastEntry) != cacheEntry)
        {
          cache->debug(__func__, __LINE__, cache->user, 
                       "splayCacheRemove() failed");
          return NULL;
        }

      /* Callback to delete old entry */
      if (cache->deleteEntry != NULL)
        {
          (cache->deleteEntry)(lastEntry, cache->user);
        }

      /* Install the new entry */
      cacheEntry->entry = splayCacheDuplicateEntry(cache, entry);
      if (cacheEntry->entry == NULL)
        {
          (cache->debug)(__func__, __LINE__, cache->user,
                        "splayCacheDuplicateEntry() failed!");
          return NULL;
        }
      splayCacheInsertEntry(cache, cacheEntry);
      
      /* Move it to the start of the cache list */
      if (splayCacheMoveToHead(cache, cacheEntry) == NULL)
        {
          cache->debug(__func__, __LINE__, cache->user,
                       "splayCacheMoveToHead() failed");
          return NULL;
        }

      return cacheEntry->entry;
    }
}

void 
splayCacheClear(splayCache_t * const cache)
{
  splayCacheEntry_t *cacheEntry, *nextCacheEntry;

  if (cache == NULL)
    {
      return;
    }

  cacheEntry = cache->head.next;
  while (cacheEntry != &(cache->tail))
    {
      nextCacheEntry = cacheEntry->next;
      if (cache->deleteEntry != NULL)
        {
          (cache->deleteEntry)(cacheEntry->entry, cache->user);
        }
      if (cache->dealloc != NULL)
        {
          (cache->dealloc)(cacheEntry, cache->user);
        }
      cacheEntry = nextCacheEntry;
    }

  cache->head.next = &(cache->tail);
  cache->tail.prev = &(cache->head);
  cache->root = NULL;
  cache->size = 0;
}

void 
splayCacheDestroy(splayCache_t * const cache)
{
  if ((cache == NULL) || (cache->dealloc == NULL))
    {
      return;
    }

  splayCacheClear(cache);

  (cache->dealloc)(cache, cache->user);

  return;
}

size_t
splayCacheGetSize(const splayCache_t * const cache)
{
  if (cache == NULL)
    {
      return 0;
    }

  return cache->size;
}

size_t
splayCacheGetDepth(const splayCache_t * const cache)
{
  if ((cache == NULL) || (cache->root == NULL))
    {
      return 0;
    }

  return splayCacheRecurseDepth(cache->root);
}

bool
splayCacheWalk(splayCache_t * const cache,  const splayCacheWalkFunc_t walk)
{
  splayCacheEntry_t *cacheEntry, *nextCacheEntry;

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
splayCacheCheck(splayCache_t * const cache)
{
  splayCacheEntry_t *cacheEntry;

  if ((cache == NULL) || (cache->compare == NULL))
    {
      return false;
    }

  /* For each entry ... */
  cacheEntry = cache->head.next;
  while (cacheEntry != &(cache->tail))
    {
      splayCacheEntry_t *left, *right;

      /* Check left child */
      left = cacheEntry->left;
      if ((left != NULL) &&
          ((cache->compare)(left->entry, cacheEntry->entry, cache->user) 
           != compareLesser))
        {
          return false;
        }

      /* Check right child */
      right = cacheEntry->right;
      if ((right != NULL) &&
          ((cache->compare)(cacheEntry->entry, right->entry, cache->user) 
           != compareLesser))
        {
          return false;
        }

      cacheEntry = cacheEntry->next;
    }

  return true;
}

bool
splayCacheBalance(splayCache_t * const cache)
{
  splayCacheEntry_t *node, **parentp;

  if ((cache == NULL) || (cache->root == NULL))
    {
      return false;
    }

  /* First convert to a list */
  node = cache->root;
  parentp = &(cache->root);
  while (node != NULL)
    {
      if (node->right != NULL)
        {
          splayCacheEntry_t *right;

          /* Rotate left */
          right = node->right;
          node->right = right->left;
          right->left = node;
          node = right;
        }

      else
        {     
          *parentp = node;
          parentp = &(node->left);
          node = node->left;
        }
    }

  /* Check */
  node = cache->root;
  while (node != NULL)
    {
      if (node->right != NULL)
        {
          cache->debug(__func__, __LINE__, cache->user, "node->right!=NULL");
          return false;
        }
      else 
        {
          node = node->left;
        }
    }

  /* Now balance the cache */
  if (cache->size <= 2)
    {
      return true;
    }

  for (size_t depth=cache->size; depth>1; depth=depth/2)
    {
      splayCacheEntry_t *now, *next;

      /* Move alternate nodes to the right child of their left child */
      now = cache->root;
      cache->root = now->left;
      next = now->left;
      if (next == NULL)
        {
          cache->debug(__func__, __LINE__, cache->user, 
                      "next==NULL at depth=%zd", depth);
          return false;
        }
      now->left = next->right; 
      next->right = now; 
      for (size_t i = 1; i<depth/2; i++)
        {
          now = next->left;
          if (now == NULL)
            {
              cache->debug(__func__, __LINE__, cache->user, 
                          "now==NULL at depth=%zd", depth);
              return false;
            }
          next->left = now->left;
          next = next->left;
          if (next == NULL)
            {
              cache->debug(__func__, __LINE__, cache->user, 
                          "next==NULL at depth=%zd", depth);
              return false;
            }
          now->left = next->right;
          next->right = now;
        }
    }
  return true;
}
