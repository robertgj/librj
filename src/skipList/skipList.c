/**
 * \file skipList.c
 *
 * A skipList_t implementation.
 */

#include <stdlib.h>
#include <stdbool.h>

#include "jsw_slib.h"
#include "skipList.h"
#include "skipList_private.h"

/**
 * Private helper function for skip list implementation.
 *
 * Shim between the \e skipList_t \e compare function and \e jsw_skip_t.
 *
 * \param a pointer to an entry defined by the caller
 * \param b pointer to an entry defined by the caller
 * \param priv \e void pointer to private data, intended to be an instance of
 * \e skipList_t
 * \return \e int indicating the result of the comparison
 */
static int skipList_cmp (const void * const a, 
                         const void * const b, 
                         void * const priv)
{
  const skipList_t *skip = (const skipList_t *)priv;
  compare_e c = (skip->compare)(a, b, skip->user);
  int res;
  switch (c)
    {
    case compareLesser:
      res = -1;
      break;
    case compareEqual:
      res = 0;
      break;
    case compareGreater:
      res = 1;
      break;
    default:
      res = 0;
      (skip->debug)(__func__, __LINE__, skip->user, 
                    "Bad compare_e returned from skip->compare()!");
    }
  return res;
}

/**
 * Private helper function for skip list implementation.
 *
 * Shim between the \e skipList_t \e duplicateEntry function and \e jsw_skip_t.
 *
 * \param item pointer to an entry defined by the caller
 * \param priv \e void pointer to private data, intended to be an instance of
 * \e skipList_t
 * \return \e void pointer to the duplicated entry data
 */
static void *skipList_dup (void * const item, void * const priv)
{
  const skipList_t *skip = (const skipList_t *)priv;
  return skip->duplicateEntry(item, skip->user);
}

/**
 * Private helper function for skip list implementation.
 *
 * Shim between the \e skipList_t \e deleteEntry function and \e jsw_skip_t.
 *
 * \param item pointer to an entry defined by the caller
 * \param priv \e void pointer to private data, intended to be an instance of
 * \e skipList_t
 */
static void skipList_rel(void * const item, void * const priv)
{
  const skipList_t *skip = (const skipList_t *)priv;
  skip->deleteEntry(item, skip->user);
  return;
}

skipList_t *
skipListCreate(const size_t space,
               const skipListAllocFunc_t alloc, 
               const skipListDeallocFunc_t dealloc,
               const skipListDuplicateEntryFunc_t duplicateEntry,
               const skipListDeleteEntryFunc_t deleteEntry,
               const skipListDebugFunc_t debug,
               const skipListCompFunc_t comp,
               void * const user)
{
  skipList_t *skip; 

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
  if (comp == NULL) 
    {
      debug(__func__, __LINE__, user, "Invalid comp() function!");
      return NULL;
    }

  skip = alloc(sizeof(skipList_t), user);
  if (skip == NULL)
    {
      debug(__func__, __LINE__, user, 
	    "Can't allocate %d for skipList_t",
	    sizeof(skipList_t));
      return NULL;
    }

  skip->skip = jsw_snew(space, skipList_cmp, skipList_dup, skipList_rel, skip);
  if (skip->skip == NULL)
    {
      dealloc(skip, user);
      debug(__func__, __LINE__, user, "Can't create jsw_skip_t instance");
      return NULL;
    }
  skip->compare = comp;
  skip->alloc = alloc;
  skip->dealloc = dealloc;
  skip->deleteEntry = deleteEntry;
  skip->duplicateEntry = duplicateEntry;
  skip->debug = debug;
  skip->space = space;
  skip->user = user;

  return skip;
}

void *
skipListFind(skipList_t * const skip, void * const entry)
{
  if ((skip == NULL) || (entry == NULL))
    {
      return NULL;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!");
      return NULL;
    }

  return jsw_sfind(skip->skip, entry);
}

void *
skipListInsert(skipList_t * const skip, void * const entry)
{
  if ((skip == NULL) || (entry == NULL))
    {
      return NULL;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!");
      return NULL;
    }

  if (jsw_sinsert(skip->skip, entry) == 0)
    {
      return NULL;
    }
  return entry;
}

void *
skipListRemove(skipList_t * const skip, void * const entry) 
{
  if ((skip == NULL) || (entry == NULL))
    {
      return NULL;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!");
      return NULL;
    }

  void * found = jsw_sfind(skip->skip, entry);
  if (found == NULL)
    {
      return NULL;
    }
  
  if(jsw_serase(skip->skip, found) == 0)
    {
      skip->debug(__func__, __LINE__, skip->user, "erase entry failed!");
      return NULL;
    }

  return found;
}

void
skipListClear(skipList_t * const skip)
{
  if (skip == NULL)
    {
      return;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!");
      return;
    }

  /* Reset the jsw_skip_t list traversal markers */
  jsw_sreset(skip->skip);

  /* Find the last non-NULL entry */
  void *item;
  while ((item = jsw_sitem(skip->skip)) != NULL)
    {
      if (jsw_serase(skip->skip,item) == 0)
        {
          skip->debug(__func__, __LINE__, skip->user, 
                      "jsw_serase() failed!");
          return;
        }
    }
}

void
skipListDestroy(skipList_t * const skip)
{
  if ((skip == NULL) || (skip->dealloc == NULL))
    {
      return;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!");
      return;
    }

  /* Destroy jsw_skip_t */
  jsw_sdelete(skip->skip);

  /* Deallocate skip */
  (skip->dealloc)(skip, skip->user);
}

size_t
skipListGetSize(const skipList_t * const skip)
{
  if (skip == NULL)
    {
      return 0;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!"); 
      return 0;
   }
  return jsw_ssize(skip->skip);
}

void *
skipListGetMax(skipList_t * const skip) 
{
  if (skip == NULL)
    {
      return NULL;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!");
      return NULL;
    }

  jsw_sreset(skip->skip);
  void *item = jsw_sitem(skip->skip);
  while(jsw_snext(skip->skip))
    {
      item = jsw_sitem(skip->skip);
    }
  return item;
}

void *
skipListGetMin(skipList_t * const skip) 
{
  if (skip == NULL)
    {
      return NULL;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!");
      return NULL;
    }

  jsw_sreset(skip->skip);
  return jsw_sitem(skip->skip);
}

void *
skipListGetNext(skipList_t * const skip, const void * const entry)
{
  if ((skip == NULL) || (entry == NULL))
    {
      return NULL;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!");
      return NULL;
    }
  if (jsw_ssize(skip->skip) == 1)
    {
      return NULL;
    }

  void *this_item = jsw_sfind(skip->skip, entry);
  if (this_item == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "entry not found!!");
      return NULL;
    }

  jsw_snext(skip->skip);
  return jsw_sitem(skip->skip);
}

void *
skipListGetPrevious(skipList_t * const skip, const void * const entry)
{
  if ((skip == NULL) || (entry == NULL))
    {
      return NULL;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!");
      return NULL;
    }
  if (jsw_ssize(skip->skip) == 1)
    {
      return NULL;
    }

  void *this_item = jsw_sfind(skip->skip, entry);
  if (this_item == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "entry not found!!");
      return NULL;
    }

  jsw_sreset(skip->skip);
  void *prev_item = jsw_sitem(skip->skip);
  while(jsw_snext(skip->skip))
    {
      void *next_item = jsw_sitem(skip->skip);
      if (next_item == this_item)
        {
          return prev_item;
        }
      prev_item = next_item;
    }

  return NULL;
}

bool
skipListWalk(skipList_t * const skip, const skipListWalkFunc_t walk)
{
  if ((skip == NULL) || (walk == NULL))
    {
      return false;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!");
      return false;
    }

  jsw_sreset(skip->skip);
  do 
    {
      void *item = jsw_sitem(skip->skip);
      if (walk(item, skip->user) == false)
        {
          return false;
        }
    }
  while(jsw_snext(skip->skip));

  return true;
}

bool
skipListCheck(skipList_t * const skip)
{
  if (skip == NULL)
    {
      return false;
    }
  if (skip->skip == NULL)
    {
      skip->debug(__func__, __LINE__, skip->user, "skip->skip == NULL!");
      return false;
    }

  jsw_sreset(skip->skip);
  void *b = jsw_sitem(skip->skip);
  while((b != NULL) && jsw_snext(skip->skip))
    {
      void *a = jsw_sitem(skip->skip);
      if (skip->compare(a,b,skip->user) == compareLesser)
        {
          skip->debug(__func__, __LINE__, skip->user, 
                      "skip->compare() failed!");
          return false;
        }
      b=a;
    }

  return true;
}
