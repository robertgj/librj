/**
 * \file intersectList.c
 *
 * Calculate a list of segment intersections.
 */
 
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "compare.h"
#include "point.h"
#include "segment.h"
#include "segmentList.h"
#include "intersectList.h"
#include "eventQueue.h"
#include "statusTree.h"

/**
 * \c intersect_t structure. 
 *
 * Internal representation of an intersection.
 */
struct intersect_t
{
  point_t point;
  /**< Intersection point */

  segmentList_t *list;
  /**< List of segments intersecting at \e point */

  struct intersect_t *next; 
  /**< Pointer to the next intersection. */
};

/**
 * \c intersectList_t type. Internal representation of a list of intersections.
 */
struct intersectList_t
{
  intersectListAllocFunc_t alloc;
  /**< Memory allocator callback function. */

  intersectListDeallocFunc_t dealloc;
  /**< Memory de-allocator callback function. */

  intersectListDebugFunc_t debug;
  /**< Debugging message callback function. */

  segmentList_t *lower;
  /**< List of segments whose lower end point lies on the sweep line. */

  segmentList_t *interior;
  /**< List of segments intersected by the sweep line. */

  segmentList_t *upper;
  /**< List of segments whose upper end point lies on the sweep line. */

  eventQueue_t *q;
  /**< Queue of segment endpoint events ordered by sweep line. */

  statusTree_t *t;
  /**< Tree of segments intersecting the current sweep line. */

  intersect_t *head;
  /**< First intersection in the list. */

  intersect_t *tail;
  /**< Last entry in the list. */

  size_t size;
  /**< Number of elements in the list. */

  bool debugging;
  /**< For debugging the source. */
};

/**
 * Private helper function for the \c intersectList_t implementation.
 *
 * Print some information for debugging.
 *
 * \param list \c intersectList_t pointer
 * \param upperList \c segmentList_t pointer to segment list containing
 * segments having point as their upper endpoint.
 * \param sweep \c point_t pointer to current sweep point
 * \param str descriptive string
 */
static
void
intersectListShowState(intersectList_t * const list, 
                       segmentList_t * const upperList,
                       point_t * const sweep, 
                       const char *str)
{
  coord_t x, y;
  real_t tol;
  
  if (list == NULL)
    {
      return;
    }

  if (upperList == NULL)
    {
      list->debug(__func__, __LINE__, "invalid upperList==NULL!");
      return;
    }

  if (sweep == NULL)
    {
      list->debug(__func__, __LINE__, "invalid sweep==NULL!");
      return;
    }

  pointGetCoords(sweep, &x, &y);
  pointGetTolerance(sweep, &tol);
  (list->debug)(NULL, 0, "");
  if (str != NULL)
    {
      (list->debug)(NULL, 0, "%s", str);
    }
  (list->debug)(NULL, 0, "Sweep : (%g,%g) tol %g", x, y, tol);

  (list->debug)(NULL, 0, "Point upper list:");
  segmentListShow(upperList);

  (list->debug)(NULL, 0, "Intersect lower list:");
  segmentListShow(list->lower);

  (list->debug)(NULL, 0, "Intersect interior list:");
  segmentListShow(list->interior);

  (list->debug)(NULL, 0, "Intersect upper list:");
  segmentListShow(list->upper);

  (list->debug)(NULL, 0, "Event queue:");
  eventQueueShow(list->q);

  (list->debug)(NULL, 0, "Status tree:");
  statusTreeShow(list->t, sweep);

  (list->debug)(NULL, 0, "Intersection list:");
  intersectListShow(list);

  (list->debug)(NULL, 0, "");

  return;
}

/**
 * Private helper function for intersection list implementation.
 *
 * Insert a list of segments into the status tree. If the segment is
 * is inserted between two segments intersecting below the sweep line
 * the corresponding event point is removed. This saves space in the event
 * queue. The event will be re-activated later.
 *
 * \param list \c intersectList_t pointer
 * \param segments \c segmentList_t pointer to list of segments to insert
 * \param sweep \c point_t pointer to current sweep point
 * \return \c intersectList_t pointer. \c NULL indicates failure.
 */
static
intersectList_t *
intersectListInsertSegmentsInStatus(intersectList_t * const list, 
                                    segmentList_t * const segments, 
                                    point_t * const sweep)
{
  segmentListEntry_t *entry;

  if (list == NULL)
    {
      return NULL;
    }

  if (segments == NULL)
    {
      list->debug(__func__, __LINE__, "invalid segments==NULL!");
      return NULL;
    }

  if (sweep == NULL)
    {
      list->debug(__func__, __LINE__, "invalid sweep==NULL!");
      return NULL;
    }

  /* Insert segments */
  entry = segmentListGetFirst(segments);
  while (entry != NULL)
    {
      status_t *S, *nextS, *previousS;
      segment_t *s, *nexts, *previouss;
      point_t intersect;
      segmentIntersect_e intersectType;

      s = segmentListGetSegment(segments, entry);
      if ((S = statusTreeInsert(list->t, s, sweep)) == NULL)
        {
          (list->debug)(__func__, __LINE__,
                        "statusTreeInsert() failed!");
          return NULL;
        }
      
      /* 
       *  If the new status tree segment has separated two
       *  intersecting segments remove the corresponding event point.
       */
      if (((nextS = statusTreeGetNext(list->t, S)) != NULL)
          &&
          ((previousS = statusTreeGetPrevious(list->t, S)) != NULL))
        {
          nexts = statusTreeGetSegment(list->t, nextS);
          previouss = statusTreeGetSegment(list->t, previousS);
          intersectType = segmentIntersection(nexts, previouss, &intersect);
          if (intersectType == segmentError)
            {
              (list->debug)(__func__, __LINE__,
                            "segmentIntersection() failed!");
              return NULL;      
            }

          if (((intersectType == segmentInteriorS1) || 
               (intersectType == segmentInteriorS2) || 
               (intersectType == segmentInterior)))
            {
              /* Remove event point */
              eventQueueRemovePoint(list->q, &intersect);
            }
        }

      /* Loop */
      entry = segmentListGetNext(segments, entry);
    }

  return list;
}

/**
 * Private helper function for intersection list implementation.
 *
 * Create a \c intersect_t intersection list entry.
 *
 * \param list \c intersectList_t pointer
 * \param point \c point_t pointer to intersection point
 * \return \c intersectList_t pointer. \c NULL indicates failure.
 */
static
intersect_t *
intersectListCreateIntersect(intersectList_t * const list, point_t * const point)
{
  segmentList_t *segments;
  intersect_t *intersect;

  if (list == NULL)
    {
      return NULL;
    }

  if (point == NULL)
    {
      list->debug(__func__, __LINE__, "invalid point==NULL!");
      return NULL;
    }

  /* Create a new intersection */
  if ((intersect = (intersect_t *)(list->alloc)(sizeof(intersect_t))) == NULL)
    {
      (list->debug)(__func__, __LINE__,
                    "Couldn't allocate %d bytes for new intersect!", 
                    sizeof(intersect_t));
      return NULL;
    }

  /* Create a new segment list for this intersection */
  segments = segmentListCreate(list->alloc, list->dealloc, NULL, list->debug); 
  if (segments == NULL)
    {
      (list->debug)(__func__, __LINE__,
                    "segmentListCreate() failed!");
      return NULL;
    }

  /* Copy intersecting segments */
  if ((list->lower != NULL) && 
      (segmentListCopy(segments, list->lower) == NULL))
    {
      (list->debug)(__func__, __LINE__,
                    "segmentListCopy() failed!");
      return NULL;
    }
  if ((list->interior != NULL) && 
      (segmentListCopy(segments, list->interior) == NULL))
    {
      (list->debug)(__func__, __LINE__,
                    "segmentListCopy() failed!");
      return NULL;
    }
  if ((list->upper != NULL) && 
      (segmentListCopy(segments, list->upper) == NULL))
    {
      (list->debug)(__func__, __LINE__,
                    "segmentListCopy() failed!");
      return NULL;
    }

  /* Install */
  pointCopy( &(intersect->point), point);
  intersect->list = segments;
  intersect->next = NULL;

  /* Add the new intersection to the intersection list */
  if (list->head == NULL)
    {
      list->head = list->tail = intersect;
    }
  else
    {
      list->tail->next = intersect;
      list->tail = intersect;
    }
  list->size = list->size + 1;

  return intersect; 
}

/**
 * Private helper function for intersection list implementation.
 *
 * Search the status tree for segments containing the current event point.
 * First find the left-most segment containing the point and then search
 * rightwards in the status tree adding segments to the list of lower
 * endpoint or interior intersections as appropriate.
 *
 * \param list \c intersectList_t pointer
 * \param point \c point_t pointer to event point
 * \return \c intersectList_t pointer. \c NULL indicates failure.
 */
static
intersectList_t *
intersectListSearchStatusForPoint(intersectList_t * const list, 
                                  point_t * const point)
{
  status_t *leftS, *nextLeftS;
  segment_t *lefts;

  /* Sanity checks */
  if (list == NULL)
    {
      return NULL;
    }

  if (point == NULL)
    {
      list->debug(__func__, __LINE__, "invalid point==NULL!");
      return NULL;
    }

  /* Search for the lowest segment containing the point */
  if ((leftS = statusTreeGetLeftMost(list->t, point)) == NULL)
    {
      return list;
    }

  /* Update lists */
  lefts = statusTreeGetSegment(list->t , leftS);
  while(segmentOrderSegmentAndPoint(lefts, point) == compareEqual)
    {
      point_t *lowerP, *upperP;

      /* 
       *  Upper endpoints don't appear in the intersection list yet so
       *  if P is an endpoint it must be the lower one. Otherwise, it
       *  must be interior to current segment.
       */
      if (segmentOrderSegmentEndPoints(lefts, &lowerP, &upperP) == NULL)
        {
          (list->debug)(__func__, __LINE__,
                        "segmentOrderSegmentEndPoints() failed!");
          return NULL;
        }
      if (pointCompare(point, lowerP) == compareEqual)
        {
          if (segmentListInsert(list->lower, lefts) == NULL)
            {
              (list->debug)(__func__, __LINE__,
                            "segmentListInsert() failed!");
              return NULL;
            }
        }
      else
        {
          if (segmentListInsert(list->interior, lefts) == NULL)
            {
              (list->debug)(__func__, __LINE__,
                            "segmentListInsert() failed!");
              return NULL;
            }
        }


      /* Remove the segment from the status tree */
      nextLeftS = statusTreeGetNext(list->t, leftS);
      statusTreeRemove(list->t, leftS);
      leftS = nextLeftS;
      lefts = statusTreeGetSegment(list->t, leftS);
    }

  return list;
}

/**
 * Private helper function for intersection list implementation.
 *
 * Decide if the segments left and right of the current event point
 * will intersect in a new event point. 
 *
 * \param list \c intersectList_t pointer
 * \param point \c point_t pointer to event point
 * \param sl \c segment_t pointer to left-hand segment
 * \param sr \c segment_t pointer to right-hand segment
 * \return \c intersectList_t pointer. \c NULL indicates failure.
 */
static
intersectList_t *
intersectListFindNewEvent(intersectList_t * const list,
                          point_t * const point,
                          segment_t * const sl, 
                          segment_t * const sr)
{
  segmentIntersect_e intersectType;
  point_t i;
  compare_e comp;

  /* Sanity checks */
  if (list == NULL)
    {
      return NULL;
    }

  if (point == NULL)
    {
      list->debug(__func__, __LINE__, "invalid point==NULL!");
      return NULL;
    }

  if (sl == NULL)
    {
      list->debug(__func__, __LINE__, "invalid sl==NULL!");
      return NULL;
    }

  if (sr == NULL)
    {
      list->debug(__func__, __LINE__, "invalid sr==NULL!");
      return NULL;
    }

  /*  
   *  If sl and sr intersect below the current sweep line, or on it
   *  and to the right of the current event, and the intersection is
   *  not yet present as an event in the queue then insert the
   *  intersection point as an event into the queue.
   */

  /* Find any intersection */
  intersectType = segmentIntersection(sl, sr, &i);
  if (intersectType == segmentError)
    {
      (list->debug)(__func__, __LINE__, "segmentIntersection() failed!");
      return NULL;      
    }
  if ((intersectType != segmentInteriorS1) &&
      (intersectType != segmentInteriorS2) && 
      (intersectType != segmentInterior))
    {
      return list;
    }

  /* There is an intersection. Check for a new event */
  comp = pointCompare(&i, point);
  if ((comp == compareLesser) || (comp == compareEqual))
    {
      /* Create a new event point */
      if (eventQueueInsertPoint(list->q, &i) == NULL)
        {
          (list->debug)(__func__, __LINE__,
                        "eventQueueInsert() failed!");
          return NULL;
        }
    }

  return list;
}

/**
 * Private helper function for intersection list implementation.
 *
 * Given an event, implements the line segment intersection algorithm.
 *
 * \param list \c intersectList_t pointer
 * \param event \c event_t pointer to event 
 * \return \c intersectList_t pointer. \c NULL indicates failure.
 */
static
intersectList_t *
intersectListHandleEventPoint(intersectList_t * const list, 
                              event_t * const event)
{
  segmentList_t *upperList;
  point_t *point;

  /* Sanity check */
  if (list == NULL)
    {
      return NULL;
    }

  if (event == NULL)
    {
      list->debug(__func__, __LINE__, "invalid event==NULL!");
      return NULL;
    }

  if((point = eventGetPoint(event)) == NULL)
    {
      (list->debug)(__func__, __LINE__, 
                    "eventGetPoint() failed!");
      return NULL;
    }

  upperList = eventGetUpperList(event);

  /* Echo */
  if (list->debugging)
    {
      coord_t x, y;
      real_t tol;

      pointGetCoords(point, &x, &y);
      pointGetTolerance(point, &tol);
      (list->debug)(NULL, 0, 
                    "\n!!!\nHandling event point (%g,%g) tol. %g\n!!!\n", 
                    x, y, tol);

      intersectListShowState(list, upperList, point,
                             "At entry to intersectHandleEventPoint():");
    }

  /*
   *  Find all segments stored in t that contain the event; they are
   *  adjacent in t. Let lower(event) denote the subset of segments
   *  found whose lower endpoint is event, and let interior(event)
   *  denote the subset of segments found that contain event in their
   *  interior and upper(event) denote the subset of segments whose
   *  upper endpoint is event. Note that, by design, segments in
   *  upper(event) cannot appear in t yet.
   */


  /* Search status for segments containing event */
  if (intersectListSearchStatusForPoint(list, point) == NULL)
    {
      (list->debug)(__func__, __LINE__,
                    "intersectListSearchStatusForEvent() failed!");
      return NULL;
    }


  /* Remember that points in upper(event) won't be found yet! Copy these. */
  if ((upperList != NULL) && 
      (segmentListCopy(list->upper, upperList) == NULL))
    {
      (list->debug)(__func__, __LINE__,
                    "segmentListCopy() failed!");
      return NULL;
    }

  /*
   *  Create an intersection point for event and the segments 
   *  (more than one!) in upper(event)+lower(event)+interior(event) 
   */
  if ((segmentListGetSize(list->lower) +
       segmentListGetSize(list->interior) +
       segmentListGetSize(list->upper)) > 1)
    {
      if (intersectListCreateIntersect(list, point) == NULL)
        {
          (list->debug)(__func__, __LINE__,
                        "intersectListCreateIntersect() failed!");
          return NULL;
        }
    }

  /*
   *  Insert the segments in upper(event)+interior(event) into t, The
   *  order of the segments in t should correspond to the order in
   *  which they are intersected by a sweep line just below event. If
   *  there is a horizontal segment, it comes last among all segments
   *  containing event. (Deleting and re-inserting the segments of
   *  interior(event) reverses their order.)
   */
  if (intersectListInsertSegmentsInStatus(list, list->upper, point) == NULL)
    {
      (list->debug)(__func__, __LINE__,
                    "intersectListInsertSegmentsInStatus() failed!");
      return NULL;
    }
  if (intersectListInsertSegmentsInStatus(list, list->interior, point) == NULL)
    {
      (list->debug)(__func__, __LINE__,
                    "intersectListInsertSegmentsInStatus() failed!");
      return NULL;
    }


  /* Do a sanity check after insertion */
  if (list->debugging)
    {
      intersectListShowState(list, upperList, point,
                             "After insertion in status tree:");

      if(statusTreeCheck(list->t) == false)
        {
          (list->debug)(__func__, __LINE__,
                        "statusSanityCheck() failed!");
          return NULL;
        }
    }

  /*
   *  If upper(event)+interior(event) is empty then:
   *     - let sl and sr be the left and right neighbours of event in t
   *     - check for a new event point at the intersection of sl and sr
   *  otherwise:
   *     - let s' be the leftmost segment of upper(event)+interior(event) 
   *       in t and sl be the left neighbour of s' in t
   *     - check for a new event point at the intersection of s' and sl
   *     - let s'' be the rightmost segment of upper(event)+interior(event) 
   *       in t and sr be the right neighbour of s'' in t
   *     - check for a new event point at the intersection of s'' and sr
   */

  /* If upper(event)+interior(event) is empty */
  if ((segmentListGetSize(list->upper) == 0) && 
      (segmentListGetSize(list->interior) == 0))
    {
      status_t *Sl, *Sr;
      segment_t *sl, *sr;

      Sl = statusTreeGetLower(list->t, point);
      Sr = statusTreeGetUpper(list->t, point);
      if ((Sl != NULL) && (Sr != NULL))
        {
          sl = statusTreeGetSegment(list->t, Sl);
          sr = statusTreeGetSegment(list->t, Sr);
          if (intersectListFindNewEvent(list, point, sl, sr) == NULL)
            {
              (list->debug)(__func__, __LINE__,
                            "intersectFindNewEvent() failed!");
              return NULL;
            }
        }
    }

  /* Otherwise check for new events: */
  else
    {
      status_t *Sp, *Spp, *Sl, *Sr;
      segment_t *sl, *sr, *sp, *spp;

      /* Find the left-most segment in t containing event */
      Sp = statusTreeGetLeftMost(list->t, point);
      Sl = statusTreeGetPrevious(list->t, Sp);
      if ((Sp != NULL) && (Sl != NULL))
        {
          sp = statusTreeGetSegment(list->t, Sp);
          sl = statusTreeGetSegment(list->t, Sl);
          intersectListFindNewEvent(list, point, sl, sp);
        }

      /* Find the right-most segment in t containing event */
      Spp = statusTreeGetRightMost(list->t, point);
      Sr = statusTreeGetNext(list->t, Spp);
      if ((Spp != NULL) && (Sr != NULL))
        {
          spp = statusTreeGetSegment(list->t, Spp);
          sr = statusTreeGetSegment(list->t, Sr);
          intersectListFindNewEvent(list, point, sr, spp);
        }
    }

  /* Do a sanity check after event checking */
  if (list->debugging)
    {
      intersectListShowState(list, upperList, point,
                             "After checking for event:");

      if (statusTreeCheck(list->t) == false)
        {
          (list->debug)(__func__, __LINE__,
                        "statusSanityCheck() failed!");
          return NULL;
        }
    }

  /* Clear upper, interior and lower lists */
  segmentListClear(list->upper);
  segmentListClear(list->interior);
  segmentListClear(list->lower);

  /* Done */
  return list;
}

point_t *
intersectGetPoint(intersect_t * const intersect)
{
  if (intersect == NULL)
    {
      return NULL;
    }

  return &(intersect->point);
}

segmentList_t *
intersectGetList(intersect_t * const intersect)
{
  if (intersect == NULL)
    {
      return NULL;
    }

  return intersect->list;
}

intersectList_t *
intersectListCreate(const intersectListAllocFunc_t alloc, 
                    const intersectListDeallocFunc_t dealloc, 
                    const intersectListDebugFunc_t debug)
{
  intersectList_t *list = NULL;

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

  /* Allocate intersect list */
  list = (intersectList_t *)alloc(sizeof(intersectList_t));
  if (list == NULL)
    {
      debug(__func__, __LINE__,
            "Couldn't allocate %d bytes for intersect list!", 
            sizeof(intersectList_t));
      return NULL;
    }

  /* Initalise */
  list->debugging = false;
  list->alloc = alloc;
  list->dealloc = dealloc;
  list->debug = debug;
  list->t = NULL;
  list->q = NULL;
  list->lower = NULL;
  list->interior = NULL;
  list->upper = NULL;
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;

  /* 
   *  Create Upper, Lower and Interior lists 
   *  (used in intersectListHandleEventPoint())
   */
  list->lower = segmentListCreate(alloc, dealloc, NULL, debug);
  if (list->lower == NULL)
    {
      debug(__func__, __LINE__, "segmentListCreate() falisted!");
      goto onError;
    }

  list->interior = segmentListCreate(alloc, dealloc, NULL, debug);
  if (list->interior == NULL)
    {
      debug(__func__, __LINE__, "segmentListCreate() failed!");
      goto onError;
    }

  list->upper = segmentListCreate(alloc, dealloc, NULL, debug);
  if (list->upper == NULL)
    {
      debug(__func__, __LINE__, "segmentListCreate() failed!");
      goto onError;
    }

  /* Initialise an empty event queue */
  list->q = eventQueueCreate(alloc, dealloc, debug);
  if (list->q == NULL)
    {
      debug(__func__, __LINE__, "eventQueueCreate() failed!");
      goto onError;
    }

  /* Initialise an empty status structure */
  list->t = statusTreeCreate(alloc, dealloc, debug);
  if (list->t == NULL)
    {
      debug(__func__, __LINE__, "statusTreeCreate() failed!");
      goto onError;
    }

  return list;

 onError:
  intersectListDestroy(list);
  return NULL;
}

bool 
intersectListScanSegments(intersectList_t * const list, 
                          segmentList_t * const segments)
{
  event_t *event;

  if (list == NULL)  
    {
      return false;
    }

  if (segments == NULL)
    {
      list->debug(__func__, __LINE__, "invalid segments==NULL!");
      return false;
    }

  if (intersectListCheck(list) == false)
    {
      list->debug(__func__, __LINE__, "intersectListCheck() failed!");
      return false;
    }

  /*
   *  Initialise an empty event queue. Add the segment 
   *  endpoints to the queue. When an upper endpoint is 
   *  inserted, the corresponding segment should be 
   *  stored with it.
   */
  if (eventQueueInsertList(list->q, segments) == NULL)
    {
      return false;
    }

  /*
   *  Handle event points in the queue
   *
   *  eventQueueGetMax() removes event from the queue but doesn't
   *  dealloc. it
   */
  while ((event = eventQueueGetMax(list->q)) != NULL)
    {
      /* 
       *  Handle the event point
       */
      if (intersectListHandleEventPoint(list, event) == NULL)
        {
          (list->debug)(__func__, __LINE__, 
                        "intersectListHandleEventPoint() failed!");
          return false;
        }
 
      /* 
       *  Deallocate the current event point now
       */
      eventQueueDeleteEvent(list->q, event);
    }


  /*
   *  Sanity check
   */
  return intersectListCheck(list);
}

void intersectListClear(intersectList_t * const list)
{
  intersect_t *intersect;

  if (list == NULL)
    {
      return;
    }
  
  if (list->size == 0)
    {
      return;
    }

  /* Clear the segment lists. */
  segmentListClear(list->lower);
  segmentListClear(list->interior);
  segmentListClear(list->upper);

  /* Clear the status tree and event queue. */
  eventQueueClear(list->q);
  statusTreeClear(list->t);

  /* Get rid of all intersections */
  intersect = list->head;
  while (intersect != NULL)
    {
      intersect_t *nextIntersect;

      nextIntersect = intersect->next;
      segmentListDestroy(intersect->list);
      (list->dealloc)(intersect);
      intersect = nextIntersect;
    }
  list->head = list->tail = NULL;
  list->size = 0;

  return;
}

void 
intersectListDestroy(intersectList_t * const list)
{
  if (list == NULL)
    {
      return;
    }

  intersectListClear(list);
  segmentListDestroy(list->lower);
  segmentListDestroy(list->interior);
  segmentListDestroy(list->upper);
  eventQueueDestroy(list->q);
  statusTreeDestroy(list->t);
  (list->dealloc)(list);

  return;
}

size_t 
intersectListGetSize(intersectList_t * const list)
{
  if (list == NULL)
    {
      return 0;
    }

  return list->size;
}

intersect_t *
intersectListGetFirst(intersectList_t * const list)
{
  if (list == NULL)
    {
      return NULL;
    }

  return list->head;
}

intersect_t *
intersectListGetNext(intersectList_t * const list, 
                     intersect_t * const intersect)
{
  if (list == NULL)
    {
      return NULL;
    }

  if (intersect == NULL)
    {
      list->debug(__func__, __LINE__, "intersect==NULL!");
      return NULL;
    }

  return intersect->next;
}

bool
intersectListCheck(intersectList_t * const list)
{
  if (list == NULL)
    {
      return false;
    }

  if ((list->q == NULL) || 
      (eventQueueIsEmpty(list->q) != true) ||
      (list->t == NULL) || 
      (statusTreeIsEmpty(list->t) != true) || 
      (segmentListGetSize(list->upper) != 0) ||
      (segmentListGetSize(list->interior) != 0) ||
      (segmentListGetSize(list->lower) != 0))
    {
      return false;
    }

  return true;
}

void
intersectListShow(intersectList_t * const list)
{
  intersect_t *i;
  point_t *p;
  coord_t x, y;

  if (list == NULL)
    {
      return;
    }

  (list->debug)(NULL, 0, "%d intersections", list->size);

  i = list->head;
  while(i != NULL)
    {
      p = intersectGetPoint(i);
      if (pointGetCoords(p, &x, &y) == NULL)
        {
          (list->debug)(__func__, __LINE__,
                        "pointGetCoords() failed!"); 
          return;
        }
      (list->debug)(NULL, 0, "intersection: (%g,%g)", x, y);
      segmentListShow(i->list);

      i = i->next;
    }

  (list->debug)(NULL, 0, ""); 

  return;
}
