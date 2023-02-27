/** 
 * \file  list.c 
 * A \c list_t implementation.
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "list.h"
#include "list_private.h"

/**
 * Private helper function for list implementation.
 *
 * Detach and deallocate the current list entry. Deallocates the list
 * entry data if the \e deleteEntry() function has been defined.
 *
 * \param list pointer to \e list_t
 */
static
void
listRemoveCurrent(list_t * const list)
{
  listEntry_t *current;

  if (list == NULL) 
    {
      return;
    }
  if (list->size == 0)
    {
      return;
    }

  /* Fix list */
  current = list->current;
  (current->next)->prev = current->prev;
  (current->prev)->next = current->next;
  list->current = current->next;

  /* Delete entry */
  if ((list->deleteEntry != NULL) && (current->entry != NULL))
    {
      (list->deleteEntry)(current->entry, list->user);
    }

  /* Deallocate current */
  list->dealloc(current, list->user);
  list->size = list->size - 1;

  return;
}

list_t *
listCreate(const listAllocFunc_t alloc, 
           const listDeallocFunc_t dealloc, 
           const listDuplicateEntryFunc_t duplicateEntry, 
           const listDeleteEntryFunc_t deleteEntry, 
           const listDebugFunc_t debug,
           const listCompFunc_t compare,
           void * const user)
{
  list_t *list = NULL;

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

  /* Allocate list state */
  list = alloc(sizeof(list_t), user);
  if (list == NULL)
    {
      debug(__func__, __LINE__, user, 
            "Couldn't allocate %d bytes for list_t!", 
            sizeof(list_t));

      return NULL;
    }

  /* Initalise new list_t */
  list->alloc = alloc;
  list->dealloc = dealloc;
  list->duplicateEntry = duplicateEntry;
  list->deleteEntry = deleteEntry;
  list->debug = debug;
  list->compare = compare;
  list->head.prev = NULL;
  list->head.next = &(list->tail);
  list->head.entry = NULL;
  list->tail.prev = &(list->head);
  list->tail.next = NULL;
  list->tail.entry = NULL;
  list->current = NULL;
  list->size = 0;
  list->user = user;
  
  return list;
}

void *
listFind(list_t * const list, const void * const entry)
{
  listEntry_t *listEntry;

  if ((list == NULL) || (entry == NULL))
    {
      return NULL;
    }
  if (list->compare == NULL)
    {
      list->debug(__func__, __LINE__, list->user, 
                  "Invalid compare() function!");
      return NULL;
    }

  /* Check current */
  if ((list->current != NULL) &&
      (list->current->entry != NULL) &&
      ((list->compare)(entry, list->current->entry, list->user) 
       == compareEqual))
    {
      return list->current->entry;
    }

  /* Find the entry */
  listEntry = (list->head).next;
  while (listEntry != &(list->tail))
    {
      if ((list->compare)(entry, listEntry->entry, list->user) == compareEqual)
        {
          list->current = listEntry;
          return listEntry->entry;
        }
      listEntry = listEntry->next;
    }

  return NULL;
}

void *
listInsert(list_t * const list, void * const entry)
{
  listEntry_t *new_listEntry;

  if (list == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      list->debug(__func__, __LINE__, list->user, "Invalid entry==NULL!");
      return NULL;
    }

  /* Don't replace an existing entry */
  if (listFind(list, entry) != NULL)
    {
      return list->current->entry;
    }

  /* Allocate a new listEntry_t */
  new_listEntry = list->alloc(sizeof(listEntry_t), list->user);
  if (new_listEntry == NULL)
    {
      list->debug(__func__, __LINE__, list->user, 
                  "Couldn't allocate %d bytes for listEntry_t!", 
                  sizeof(listEntry_t));
      return NULL;
    }

  /* Duplicate entry */
  if (list->duplicateEntry != NULL)
    {
      void *new_entry = list->duplicateEntry(entry, list->user);
      if (new_entry == NULL)
        {
          list->dealloc(new_listEntry, list->user);
          list->debug(__func__, __LINE__, list->user, 
                      "Couldn't duplicate entry!");
          return NULL;
        }
      new_listEntry->entry = new_entry;
    }
  else
    {
      new_listEntry->entry = entry;
    }

  /* Update list */
  new_listEntry->next = &(list->tail);
  new_listEntry->prev = (list->tail).prev;
  (new_listEntry->prev)->next = new_listEntry;
  (list->tail).prev = new_listEntry;
  list->current = new_listEntry;
  list->size = list->size + 1;

  return new_listEntry->entry;
}

void *
listRemove(list_t * const list, void * const entry)
{
  if ((list == NULL) || (entry == NULL) || (list->size == 0))
    {
      return NULL;
    }

  /* Find the entry */
  void *found = listFind(list, entry);
  if(found != NULL)
    {
      listRemoveCurrent(list);
    }

  return found;
}

void
listClear(list_t * const list)
{
  if (list == NULL)
    {
      return;
    }
  if (list->size == 0)
    {
      return;
    }

  /* Get rid of all entries */
  list_t *tmp = listGetFirst(list);
  if (tmp == NULL)
    {
      return;
    }
  while (list->size != 0)
    {
      listRemoveCurrent(list);
    }

  return;
}

void 
listDestroy(list_t * const list)
{
  if (list == NULL)
    {
      return;
    }

  /* Get rid of all entries */
  listClear(list);

  /* Free the list_t */
  (list->dealloc)(list, list->user);
}

size_t 
listGetSize(const list_t * const list)
{
  if (list == NULL)
    {
      return 0;
    }
  return list->size;
}

void *
listGetFirst(list_t *list)
{
  if (list == NULL)
    {
      return NULL;
    }
  if (list->size == 0)
    {
      return NULL;
    }

  list->current = (list->head).next;

  return list->current->entry;
}

void *
listGetLast(list_t * list)
{
  if (list == NULL)
    {
      return NULL;
    }

  list->current = (list->tail).prev;

  return list->current->entry;
}

void *
listGetNext(list_t * list, const void * const entry)
{
  if (list == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      return NULL;
    }

  listFind(list, entry);

  if ((list->current == NULL) || (list->current->next == NULL))
    {
      list->current = NULL;
      return NULL;
    }

  list->current = list->current->next;

  return list->current->entry;
}

void *
listGetPrevious(list_t * const list, const void * const entry)
{
  if (list == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      return NULL;
    }

  listFind(list, entry);

  if ((list->current == NULL) || (list->current->prev == NULL))
    {
      list->current = NULL;
      return NULL;
    }

  list->current = list->current->prev;

  return list->current->entry;
}

bool
listWalk(list_t * const list, const listWalkFunc_t walk)
{
  bool res;
  listEntry_t *listEntry, *nextListEntry;

  if (list == NULL)
    {
      return false;
    }
  if (walk == NULL)
    {
      list->debug(__func__, __LINE__, list->user,
                  "Invalid walk() function!");
      return false;
    }


  listEntry = list->head.next;
  while (listEntry != &(list->tail))
    {
      nextListEntry = listEntry->next; 
      if ((res = walk(listEntry->entry, list->user)) == false)
        {
          return false;
        }
      listEntry = nextListEntry;
    }

  return true;
}

list_t *
listCopy(list_t * const dst, list_t * const src)
{
  if ((dst == NULL) || (src == NULL))
    {
      return NULL;
    }

  listClear(dst);
  if (listGetSize(src) == 0)
    {
      return dst;
    }

  void *entry = listGetFirst(src);
  do
    {
      listInsert(dst, entry);
      entry = listGetNext(src, entry);
    }
  while (entry != NULL);

  return dst;
}
