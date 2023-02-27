/**
 * \file statusTree.h
 *
 * Public interface for a segment intersection status tree type
 */

#if !defined(STATUS_TREE_H)
#define STATUS_TREE_H

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
   * \c status_t structure. An opaque type for a node of the status tree.
   */
  typedef struct status_t status_t;

  /**
   * \c statusTree_t structure. An opaque type for a status tree.
   */
  typedef struct statusTree_t statusTree_t;

  /**
   * \c statusTree_t memory allocator.
   *
   * Memory allocation call-back.
   *
   * \param size amount of memory requested
   * \return pointer to the memory allocated. \c NULL indicates failure.
   */
  typedef void *(*statusTreeAllocFunc_t)(const size_t size);

  /**
   * \c statusTree_t memory de-allocator.
   *
   * Memory deallocation call-back.
   *
   * \param ptr pointer to memory to be deallocated
   */
  typedef void (*statusTreeDeallocFunc_t)(void *ptr);

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
  typedef void (*statusTreeDebugFunc_t)(const char *function,
                                        const unsigned int line,
                                        const char *format, 
                                        ...);
 

  /**
   * Create an empty status tree of segments.
   * 
   * Creates and initialises an empty \c statusTree_t instance
   *
   * \param alloc memory allocator callback
   * \param dealloc memory deallocator callback
   * \param debug message function callback
   * \return pointer to a \c statusTree_t. \c NULL indicates failure.
   */
  statusTree_t *statusTreeCreate(const statusTreeAllocFunc_t alloc, 
                                 const statusTreeDeallocFunc_t dealloc, 
                                 const statusTreeDebugFunc_t debug);

  /**
   * Insert a segment into the status tree.
   *
   * Inserts a segment into the status tree at the sweep point. The address 
   * of the \c status_t is returned. 
   *
   * \param tree \c statusTree_t pointer to the status tree
   * \param segment pointer to \c segment_t to be inserted
   * \param sweep pointer to \c point_t to be inserted
   * \return pointer to the \c status_t. \c NULL indicates failure.
   */
  status_t *statusTreeInsert(statusTree_t * const tree, 
                             segment_t * const segment, 
                             point_t * const sweep);

  /**
   * Remove a segment from the status tree.
   *
   * Remove a segment from the status tree.
   *
   * \param tree \c statusTree_t pointer to the status tree
   * \param status \c status_t pointer to status_t of the segment removed
   */
  void statusTreeRemove(statusTree_t * const tree, status_t * const status);

  /** 
   * Clear the statusTree.
   *
   * Clear the statusTree.
   *
   * \param tree \c statusTree_t pointer to the statusTree
   */
  void statusTreeClear(statusTree_t * const tree);

  /** 
   * Destroy the status tree.
   *
   * Clears the status tree and deallocates the statusTree_t.
   *
   * \param tree \c statusTree_t pointer to the status tree
   */
  void statusTreeDestroy(statusTree_t * const tree);

  /** 
   * Check if the status tree is empty.
   *
   * Check if the status tree is empty.
   *
   * \param tree \c statusTree_t pointer to the status tree
   * \return \c bool value
   */
  bool statusTreeIsEmpty(const statusTree_t * const tree);

  /** 
   * Get the segment from this \c status_t status tree node.
   *
   * Given a \c status_t status tree node return the segment in the node. 
   *
   * \param tree \c statusTree_t pointer to the status tree
   * \param status current \c status_t pointer.
   * \return \c segment_t pointer
   */
  segment_t *statusTreeGetSegment(statusTree_t * const tree, 
                                  status_t * const status);

  /** 
   * Get the next segment in the status tree.
   *
   * Given a \c status_t return the next status in the status tree. 
   *
   * \param tree \c statusTree_t pointer to the status tree
   * \param status current \c status_t pointer.
   * \return \c status_t pointer
   */
  status_t *statusTreeGetNext(statusTree_t * const tree, 
                              status_t * const status);

  /** 
   * Get the previous segment in the status tree.
   *
   * Given a \c status_t containing the segment return the previous
   * status in the status tree.
   *
   * \param tree \c statusTree_t pointer to the status tree
   * \param status \c status_t pointer.
   * \return \c status_t pointer
   */
  status_t *statusTreeGetPrevious(statusTree_t * const tree,
                                  status_t * const status);
  /** 
   * Get the status above or including sweep in the status tree.
   *
   * Return the status in the status tree above or containing sweep.
   *
   * \param tree \c statusTree_t pointer to the status tree
   * \param sweep pointer to \c point_t sweep point
   * \return \c status_t pointer
   */
  status_t *statusTreeGetUpper(statusTree_t * const tree,
                               point_t * const sweep);

  /** 
   * Get the status below or including sweep in the status tree.
   *
   * Return the status in the status tree below or containing sweep.
   *
   * \param tree \c statusTree_t pointer to the status tree
   * \param sweep pointer to \c point_t sweep point
   * \return \c status_t pointer
   */
  status_t *statusTreeGetLower(statusTree_t * const tree,
                               point_t * const sweep);

  /** 
   * Get the left-most status containing sweep in the status tree.
   *
   * Return the left-most \c status_t containing the segment in the
   * status tree that contains the given sweep point.
   *
   * \param tree \c statusTree_t pointer to the status tree
   * \param sweep pointer to \c point_t sweep point
   * \return \c status_t pointer
   */
  status_t * statusTreeGetLeftMost(statusTree_t * const tree, 
                                   point_t * const sweep);

  /** 
   * Get the right-most status containing sweep in the status tree.
   *
   * Return the right-most \c status_t containing the segment in the
   * status tree that contains the given sweep point.
   *
   * \param tree \c statusTree_t pointer to the status tree
   * \param sweep pointer to \c point_t sweep point
   * \return \c status_t pointer
   */
  status_t *statusTreeGetRightMost(statusTree_t * const tree, 
                                   point_t * const sweep);

  /** 
   * Print each segment in the status tree.
   *
   * Traverse the status tree printing each segment.
   *
   * \param tree \c statusTree_t pointer to the status tree
   * \param sweep pointer to \c point_t sweep point
   */
  void statusTreeShow(statusTree_t * const tree, point_t * const sweep);

  /** 
   * Check the consistency of the status tree at the current sweep point.
   *
   * Traverse the status tree checking consistency.
   *
   * \param tree \c statusTree_t pointer to the status tree
   */
  bool statusTreeCheck(statusTree_t * const tree);

#ifdef __cplusplus
}
#endif

#endif
