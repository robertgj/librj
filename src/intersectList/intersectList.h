/**
 * \file intersectList.h
 *
 * Public interface for a list of intersections of segments 
 */

#if !defined(INTERSECT_LIST_H)
#define INTERSECT_LIST_H

#ifdef __cplusplus
#include <cstdarg>
#include <cstdbool>
#include <cstddef>
using std::size_t;
extern "C" {
#else
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#endif

#include "segment.h"
#include "segmentList.h"

  /**
   * \c intersect_t structure. An opaque type for an intersection point
   */
  typedef struct intersect_t intersect_t;

  /**
   * \c intersectList_t structure. An opaque type for a list of intersection 
   * points
   */
  typedef struct intersectList_t intersectList_t;

  /**
   * \c intersectList_t memory allocator.
   *
   * Memory allocation call-back.
   *
   * \param size amount of memory requested
   * \return pointer to the memory allocated. \c NULL indicates failure.
   */
  typedef void *(*intersectListAllocFunc_t)(const size_t size);

  /**
   * \c intersectList_t memory de-allocator.
   *
   * Memory deallocation call-back.
   *
   * \param ptr pointer to memory to be deallocated
   */
  typedef void (*intersectListDeallocFunc_t)(void *ptr);

  /**
   * Debugging message.
   *
   * Callback function to output a debugging message. Accepts a 
   * variable argument list like \e printf(). 
   *
   * \param function name
   * \param line number
   * \param format string
   * \param ... variable argument list
   */
  typedef void (*intersectListDebugFunc_t)(const char *function, 
                                           const unsigned int line, 
                                           const char *format, 
                                           ...);

  /**
   * Access function for the intersection segment list
   *
   * Get the intersection point from the \c intersect_t
   *
   * \param intersect pointer to \c intersect_t 
   * \return pointer to \c point_t. \c NULL indicates failure
   */
  point_t *intersectGetPoint(intersect_t * const intersect);

  /**
   * Access function for the intersection segment list
   *
   * Get the list of intersecting segments from the \c intersect_t
   *
   * \param intersect pointer to \c intersect_t 
   * \return pointer to \c segmentList_t. \c NULL indicates failure
   */
  segmentList_t *intersectGetList(intersect_t * const intersect);

  /**
   * Create an empty intersection list.
   * 
   * Creates and initialises an empty \c intersectList_t 
   *
   * \param alloc memory allocator callback
   * \param dealloc memory deallocator callback
   * \param debug message function callback
   * \return pointer to a \c intersectList_t. \c NULL indicates failure.
   */
  intersectList_t *intersectListCreate(const intersectListAllocFunc_t alloc, 
                                       const intersectListDeallocFunc_t dealloc, 
                                       const intersectListDebugFunc_t debug);

  /** 
   * Scan a list of segments to find intersections.
   *
   * Scans the list of segments to find the intersection points.
   *
   * \param list \c intersectList_t pointer to the intersection list
   * \param segments \c segmentList_t pointer to list of segments
   * \return \c intersect_t pointer to the first intersection found.
   */
  bool intersectListScanSegments(intersectList_t * const list, 
                                 segmentList_t * const segments);

  /** 
   * Clear the intersection list.
   *
   * Clears the intersection list.
   *
   * \param list \c intersectList_t pointer to the intersection list
   */
  void intersectListClear(intersectList_t * const list);

  /** 
   * Destroy the intersection list.
   *
   * Clears the intersection list and deallocates the intersectList_t.
   *
   * \param list \c intersectList_t pointer to the intersection list
   */
  void intersectListDestroy(intersectList_t * const list);

  /** 
   * Size of the intersection list.
   *
   * Clears the intersection list and deallocates the intersectList_t.
   *
   * \param list \c intersectList_t pointer to the intersection list
   * \return number if intersections in the list
   */
  size_t intersectListGetSize(intersectList_t * const list);

  /** 
   * Get the first intersection in the list.
   *
   * Get the first intersection in the list. 
   *
   * \param list \c intersectList_t pointer to the intersection list
   * \return \c intersect_t pointer
   */
  intersect_t *intersectListGetFirst(intersectList_t * const list);

  /** 
   * Get the next intersection in the list.
   *
   * Given an intersection return the next intersection in the list. 
   *
   * \param list \c intersectList_t pointer to the intersection list
   * \param intersect \c intersect_t pointer. 
   * \return \c intersect_t pointer
   */
  intersect_t *intersectListGetNext(intersectList_t * const list, 
                                    intersect_t * const intersect);

  /** 
   * Check the consistency of the intersection list.
   *
   * Check the consistency of the intersection list.
   *
   * \param list \c intersectList_t pointer to the intersection list
   * \return \c bool indicating success
   */
  bool intersectListCheck(intersectList_t * const list);

  /** 
   * Print each intersection in the list.
   *
   * Traverses the intersection list printing each intersection.
   *
   * \param list \c intersectList_t pointer to the intersection list
   */
  void intersectListShow(intersectList_t * const list);

#ifdef __cplusplus
}
#endif

#endif
