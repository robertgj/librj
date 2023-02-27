/**
 * \file segmentList.c
 *
 * Implementation of segmentList_t
 */

#include <stdlib.h>
#include <stdarg.h>

#include "point.h"
#include "segment.h"
#include "segmentList.h"

/**
 * \c segmentListEntry_t structure.
 *
 *  An opaque segment list entry type.
 */
struct segmentListEntry_t
{
  segment_t *segment;
  segmentListEntry_t *next;
};

/**
 * \c segmentList_t structure.
 *
 *  An opaque segment list type.
 */
struct segmentList_t
{
  segmentListAllocFunc_t alloc; 
  /**< Memory allocator callback function. */

  segmentListDeallocFunc_t dealloc;
  /**< Memory de-allocator callback function. */
 
  segmentListDeallocSegmentFunc_t deallocSegment;
  /**< Segment memory de-allocator callback function. */

  segmentListDebugFunc_t debug;
  /**< Debugging message callback function. */

  segmentListEntry_t *head;
  /**< First segment in the list. */

  segmentListEntry_t *tail;
  /**< Last segment in the list. */

  size_t size;
  /**< Number of segments in the list. */
};

/**
 * Private helper function for segment list implementation.
 *
 * Show the contents of a point
 *
 * \param list \c segmentList_t pointer
 * \param s pointer to \c segment_t
 */
static
void
segmentListShowSegment(segmentList_t * const list, segment_t * const s)
{
  point_t *a, *b;
  coord_t ax, ay, bx, by;

  if (list == NULL)
    {
      return;
    }
  
  if (s == NULL)
    {
      list->debug(__func__, __LINE__, "invalid s==NULL!");
      return;
    }

  segmentGetPoints(s, &a, &b);
  pointGetCoords(a, &ax, &ay);
  pointGetCoords(b, &bx, &by);
  (list->debug)(NULL, 0, "Segment a=(%g,%g) b=(%g,%g)", ax, ay, bx, by);

  return;
}

segmentList_t *
segmentListCreate(const segmentListAllocFunc_t alloc, 
                  const segmentListDeallocFunc_t dealloc, 
                  const segmentListDeallocSegmentFunc_t deallocSegment, 
                  const segmentListDebugFunc_t debug)
{
  segmentList_t *list = NULL;

  /* Sanity checks */
  if (debug == NULL)
    {
      return NULL;
    }
  if (alloc == NULL)
    {
      debug(__func__, __LINE__, "Invalid alloc() function!");
      return NULL;
    }
  if (dealloc == NULL)
    {
      debug(__func__, __LINE__, "Invalid dealloc() function!");
      return NULL;
    }
  if (deallocSegment == NULL)
    {
      /* This is permitted! */
    }

  /* Allocate segment list */
  list = (segmentList_t *)alloc(sizeof(segmentList_t));
  if (list == NULL)
    {
      debug(__func__, __LINE__,
            "Couldn't allocate %d bytes for segment list!", 
            sizeof(segmentList_t));

      return NULL;
    }

  /* Initalise */
  list->alloc = alloc;
  list->dealloc = dealloc;
  list->deallocSegment = deallocSegment;
  list->debug = debug;
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
  
  return list;
}

segmentListEntry_t *
segmentListInsert(segmentList_t * const list, segment_t * const s)
{
  segmentListEntry_t *entry;

  /* Sanity check */
  if (list == NULL)
    {
      return NULL;
    }

  if (s == NULL)
    {
      list->debug(__func__, __LINE__, "invalid s==NULL!");
      return NULL;
    }

  entry = (segmentListEntry_t *)(list->alloc)(sizeof(segmentListEntry_t));
  if (entry == NULL)
    {
      (list->debug)(__func__, __LINE__,
                    "Couldn't allocate %d bytes for segment list entry!", 
                    sizeof(segmentListEntry_t));

      return NULL;
    }
  entry->segment = s;
  entry->next = NULL;

  /* Install segment in list */
  if (list->head == NULL)
    {
      list->head = list->tail = entry;
    }
  else
    {
      list->tail->next = entry;
      list->tail = entry;
    }
  list->size = list->size + 1;

  return entry;
}

segmentList_t *
segmentListCopy(segmentList_t * const dst, segmentList_t * const src)
{
  segmentListEntry_t *entry;

  /* Sanity check */
  if ((dst == NULL) || (src == NULL))
    {
      return NULL;
    }

  /* Copy segments from src */
  entry = src->head;
  while(entry != NULL)
    {
      if (segmentListInsert(dst, entry->segment) == NULL)
        {
          (dst->debug)(__func__, __LINE__,
                       "segmentListInsert() failed!");
          return NULL;
        }
      entry = entry->next;
    }

  return dst;
}

void
segmentListClear(segmentList_t * const list)
{
  segmentListEntry_t *entry;

  /* Sanity checks */
  if (list == NULL)
    {
      return;
    }

  entry = list->head;
  while(entry != NULL)
    {
      segmentListEntry_t *nextEntry;
      
      nextEntry = entry->next;
      if (list->deallocSegment != NULL)
        {
          (list->deallocSegment)(entry->segment);
        }
      (list->dealloc)(entry);
      entry = nextEntry;
    }
  list->head = list->tail = NULL;
  list->size = 0;

  return;
}

void
segmentListDestroy(segmentList_t * const list)
{
  if (list == NULL)
    {
      return;
    }

  segmentListClear(list);
  (list->dealloc)(list);

  return;
}

segment_t *
segmentListGetSegment(segmentList_t * const list, 
                      segmentListEntry_t * const entry)
{
  if ((list == NULL) || (entry == NULL))
    {
      return NULL;
    }

  return entry->segment;
}

segmentListEntry_t *
segmentListGetFirst(segmentList_t * const list)
{
  if (list == NULL)
    {
      return NULL;
    }

  return list->head;
}

segmentListEntry_t *
segmentListGetNext(segmentList_t * const list, segmentListEntry_t * const entry)
{
  if (list == NULL)
    {
      return NULL;
    }

  return entry->next;
}

size_t
segmentListGetSize(segmentList_t * const list)
{
  if (list == NULL)
    {
      return 0;
    }

  return list->size;
}

void
segmentListShow(segmentList_t * const list)
{
  segmentListEntry_t *entry;

  if (list == NULL)
    {
      return;
    }

  entry = list->head;
  while(entry != NULL)
    {
      segmentListShowSegment(list, entry->segment);
      entry = entry->next;
    }

  (list->debug)(NULL, 0, ""); 

  return;
}
