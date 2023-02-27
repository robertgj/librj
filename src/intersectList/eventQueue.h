/**
 * \file eventQueue.h
 *
 * Public interface for a segment intersection event queue type
 */

#if !defined(EVENT_QUEUE_H)
#define EVENT_QUEUE_H

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
#include "segmentList.h"


/**
 * \c event_t structure. An opaque type for an event.
 */
typedef struct event_t event_t;

/**
 * \c eventQueue_t structure. An opaque type for an event queue.
 */
typedef struct eventQueue_t eventQueue_t;

/**
 * \c eventQueue_t memory allocator.
 *
 * Memory allocation call-back.
 *
 * \param size amount of memory requested
 * \return pointer to the memory allocated. \c NULL indicates failure.
 */
typedef void *(*eventQueueAllocFunc_t)(const size_t size);

/**
 * \c eventQueue_t memory de-allocator.
 *
 * Memory deallocation call-back.
 *
 * \param ptr pointer to memory to be deallocated
 */
typedef void (*eventQueueDeallocFunc_t)(void *ptr);

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
typedef void (*eventQueueDebugFunc_t)(const char *function, 
                                      const unsigned int line, 
                                      const char *format, 
                                      ...);

/**
 * Return the point from the \c event_t
 *
 * Access function for the event point
 *
 * \param event pointer to \c event_t 
 * \return pointer to \c point_t
 */
point_t * eventGetPoint(event_t * const event);

/**
 * Return the upper endpoint segment list from the \c event_t
 *
 * Access function for the event upper endpoint segment list
 *
 * \param event pointer to \c event_t 
 * \return pointer to \c segmentList_t
 */
segmentList_t *eventGetUpperList(const event_t * const event);

/**
 * Set the point in the \c event_t
 *
 * Access function for the event point
 *
 * \param event pointer to \c event_t 
 * \param p pointer to \c point_t to be installed

 */
event_t *eventSetPoint(event_t * const event, point_t * const p);

/**
 * Set the upper endpoint list in the \c event_t
 *
 * Access function for the event upper endpoint list
 *
 * \param event pointer to \c event_t 
 * \param l pointer to \c segmentList_t to be installed
 * \return pointer to \c event_t. \c NULL indicates failure
 */
event_t *eventSetUpperList(event_t * const event, segmentList_t * const l);

/**
 * Create an empty event queue of segments.
 * 
 * Creates and initialises an empty \c eventQueue_t instance
 *
 * \param alloc memory allocator callback
 * \param dealloc memory deallocator callback
 * \param debug message function callback
 * \return pointer to a \c eventQueue_t. \c NULL indicates failure.
 */
eventQueue_t *eventQueueCreate(const eventQueueAllocFunc_t alloc, 
                               const eventQueueDeallocFunc_t dealloc, 
                               const eventQueueDebugFunc_t debug);

/**
 * Insert an event point into the event queue.
 *
 * Allocates an event, copies the input point and inserts the copy into
 * the event queue. The address of the \c event_t is returned. 
 *
 * \param queue \c eventQueue_t pointer to the event queue
 * \param point pointer to \c point_t to be inserted
 * \return pointer to the \c event_t. \c NULL indicates failure.
 */
event_t *eventQueueInsertPoint(eventQueue_t * const queue, 
                               point_t * const point);

/**
 * Insert a segment list into the event queue.
 *
 * Allocates an event for each endpoint in the segment list, copies the 
 * point and inserts the copy into the queue. The address of the 
 * \c eventQueue_t is returned. 
 *
 * \param queue \c eventQueue_t pointer to the event queue
 * \param list pointer to \c segmentList_t of points to be inserted
 * \return pointer to the \c eventQueue_t. \c NULL indicates failure.
 */
eventQueue_t *eventQueueInsertList(eventQueue_t * const queue, 
                                   segmentList_t * const list);

/**
 * Remove an event point from the queue.
 *
 * Remove an event point from the queue.
 *
 * \param queue \c eventQueue_t pointer to the event queue
 * \param point \c point_t pointer to the event point to be removed
 */
void eventQueueRemovePoint(eventQueue_t * const queue, point_t * const point);

/**
 * Delete an event
 *
 * Free memory of an event. Assume it is no longer in the queue.
 *
 * \param queue \c eventQueue_t pointer to the event queue
 * \param event \c event_t pointer the event to be removed
 */
void eventQueueDeleteEvent(eventQueue_t * const queue, event_t * const event);

/** 
 * Clear the eventQueue.
 *
 * Clear the eventQueue.
 *
 * \param queue \c eventQueue_t pointer to the eventQueue
 */
void eventQueueClear(eventQueue_t * const queue);

/** 
 * Destroy the event queue.
 *
 * Clears the event queue and deallocates the eventQueue_t.
 *
 * \param queue \c eventQueue_t pointer to the event queue
 */
void eventQueueDestroy(eventQueue_t * const queue);

/** 
 * Check if the queue is empty.
 *
 * Check if the event queue is empty.
 *
 * \param queue \c eventQueue_t pointer to the event queue
 * \return \c bool value
 */
bool eventQueueIsEmpty(eventQueue_t * const queue);

/** 
 * Get the next event in the queue.
 *
 * Given an event return the next event in the queue. The event is
 * removed from the queue but not deallocated.
 *
 * \param queue \c eventQueue_t pointer to the event queue
 * \return \c event_t pointer
 */
event_t *eventQueueGetMax(eventQueue_t * const queue);

/** 
 * Print each event in the queue.
 *
 * Traverses the event queue printing each event.
 *
 * \param queue \c eventQueue_t pointer to the event queue
 */
void eventQueueShow(eventQueue_t * const queue);

#ifdef __cplusplus
}
#endif

#endif
