/**
 * \file segmentList.h
 *
 * Public interface for a segment list type.
 */

#if !defined(SEGMENT_LIST_H)
#define SEGMENT_LIST_H

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

#include "point.h"
#include "segment.h"

  /**
   * \c segmentListEntry_t structure. An opaque type for a entry in a
   * list of segments.
   */
  typedef struct segmentListEntry_t segmentListEntry_t;

  /**
   * \c segmentList_t structure. An opaque type for a list of segments.
   */
  typedef struct segmentList_t segmentList_t;

  /**
   * \c segmentList_t memory allocator.
   *
   * Memory allocation call-back.
   *
   * \param amount of memory requested
   * \return pointer to the memory allocated. \c NULL indicates failure.
   */
  typedef void *(*segmentListAllocFunc_t)(const size_t amount);

  /**
   * \c segmentList_t memory de-allocator.
   *
   * Memory deallocation call-back.
   *
   * \param pointer to memory to be deallocated
   */
  typedef void (*segmentListDeallocFunc_t)(void *pointer);

  /**
   * \c segmentList_t segment memory de-allocator.
   *
   * Memory deallocation call-back for caller's segment memory
   *
   * \param segment pointer to caller's memory to be deallocated
   */
  typedef void (*segmentListDeallocSegmentFunc_t)(segment_t *segment);

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
  typedef void (*segmentListDebugFunc_t)(const char *function,  
                                         const unsigned int line,
                                         const char *format, 
                                         ...);

  /**
   * Create an empty list of segments.
   * 
   * Creates and initialises an empty \c segmentList_t instance
   *
   * \param alloc memory allocator callback
   * \param dealloc memory deallocator callback
   * \param deallocSegment segment memory deallocator callback
   * \param debug message function callback
   * \return pointer to a \c segmentList_t. \c NULL indicates failure.
   */
  segmentList_t *
  segmentListCreate(const segmentListAllocFunc_t alloc, 
                    const segmentListDeallocFunc_t dealloc, 
                    const segmentListDeallocSegmentFunc_t deallocSegment, 
                    const segmentListDebugFunc_t debug);

  /**
   * Insert an entry into the segment list.
   *
   * Allocates a segment, copies the input segment and inserts the copy at 
   * the end of the list. Sets it as the current entry. The address of the 
   * segment inserted is returned. 
   *
   * \param list \c segmentList_t pointer to list 
   * \param s \c segment_t pointer to segment to be inserted 
   * \return \c segmentListEntry_t pointer to the segment list entry 
   * installed. \c NULL indicates failure.
   */
  segmentListEntry_t *segmentListInsert(segmentList_t * const list,
                                        segment_t * const s);

  /**
   * Copy segment pointers from one list to another.
   *
   * Copies the segment pointer s from list \e src to an existing list \e dst. 
   *
   * \param dst \c segmentList_t pointer to destination list
   * \param src \c segmentList_t pointer to src list
   * \return \c segmentList_t pointer to \e dst. \c NULL indicates failure.
   */
  segmentList_t *segmentListCopy(segmentList_t * const dst,
                                 segmentList_t * const src);

  /**
   * Clear the segment list.
   *
   * Remove all segments from the list.
   *
   * \param list \c segmentList_t pointer
   */
  void segmentListClear(segmentList_t * const list);

  /** 
   * Destroy the segment list.
   *
   * Clears the segment list and deallocates the segmentList_t.
   *
   * \param list \c segmentList_t pointer
   */
  void segmentListDestroy(segmentList_t * const list);

  /** 
   * Get the number of segments in the list.
   *
   * Return the number of segments in the list.
   *
   * \param list \c segmentList_t pointer
   * \return number of segments
   */
  size_t segmentListGetSize(segmentList_t * const list);

  /** 
   * Get the segment from the segment list entry.
   *
   * Get the segment from the segment list entry.
   *
   * \param list \c segmentList_t pointer
   * \param entry \c segmentListEntry_t pointer
   * \return \c segment_t pointer
   */
  segment_t *
  segmentListGetSegment(segmentList_t * const list,
                        segmentListEntry_t * const entry);

  /** 
   * Get the first segment in the list.
   *
   * Get the first segment in the list.
   *
   * \param list \c segmentList_t pointer
   * \return \c segmentListEntry_t pointer
   */
  segmentListEntry_t *segmentListGetFirst(segmentList_t * const list);

  /** 
   * Get the next segment in the list.
   *
   * Given segment \e s return the next segment in the list.
   *
   * \param list \c segmentList_t pointer
   * \param entry \c segmentListEntry_t pointer
   * \return \c segmentListEntry_t pointer
   */
  segmentListEntry_t *
  segmentListGetNext(segmentList_t * const list,
                     segmentListEntry_t * const entry);

  /** 
   * Print each segment in the list.
   *
   * Traverses the segment list printing each segment.
   *
   * \param list pointer to \c segmentList_t
   */
  void segmentListShow(segmentList_t * const list);

#ifdef __cplusplus
}
#endif

#endif
