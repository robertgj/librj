/**
 * \file segment.h
 *
 * Public interface for a 2-D segment type
 */

#if !defined(SEGMENT_H)
#define SEGMENT_H

#include "point.h"
#include "compare.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \c segment_t structure. 
 */
typedef struct segment_t 
{
  point_t *a;
  /**< A point. */

  point_t *b;
  /**< Another point */
}
segment_t;

/**
 * \c segmentIntersect_e enumerated type.
 *
 * An enumeration of the possible types of segment intersection
 */
typedef enum segmentIntersect_e
{
  segmentError = 0,
  /**< Error */
 
  segmentPointsDisjoint,
  /**< Segments are distinct points */
 
  segmentPointsCoincide,
  /**< Segments are coincident points */
 
  segmentDisjoint,
  /**< Segments do not intersect */
 
  segmentInteriorS1,
  /**< Intersection interior to first segment but at endpoint of second */
 
  segmentInteriorS2,
  /**< Intersection interior to second segment but at endpoint of first */
 
  segmentInterior,
  /**< Intersection interior to both segments */
 
  segmentVertex,
  /**< Intersection at endpoints of both segments */
 
  segmentCoincide
  /**< Enpoints of each segment are distinct but coincide with the other */
 
}
segmentIntersect_e;

/**
 * Get the end-points of the segment
 *
 * Get the end-points of the segment
 *
 * \param s pointer to \c segment_t
 * \param a pointer to \c point_t pointer, first endpoint
 * \param b pointer to \c point_t pointer, second endpoint
 * \return \c segment_t pointer. \c NULL represents failure.
 */
segment_t *segmentGetPoints(segment_t * const s, 
                            point_t ** const a, 
                            point_t ** const b);

/**
 * Set the end-points of the segment
 *
 * Set the end-points of the segment
 *
 * \param s pointer to \c segment_t
 * \param a pointer to \c point_t, first endpoint
 * \param b pointer to \c point_t, second endpoint
 * \return \c segment_t pointer. \c NULL represents failure.
 */
segment_t *segmentSetPoints(segment_t * const s, 
                            point_t * const a, 
                            point_t * const b);

/**
 * Return a string representing the \c segmentIntersect_e
 *
 * Return a string representing the \c segmentIntersect_e
 *
 * \param i \c segmentIntersect_e value
 * \return string representing \e i 
 */
const char *segmentIntersectEnum2Str(segmentIntersect_e i);

/**
 * Order the end-points of the segment
 *
 * Order the end-points of the segment so that \e a is lower than \e b
 *
 * \param s pointer to \c segment_t
 * \param lower pointer to \c point_t pointer, lower endpoint
 * \param upper pointer to \c point_t pointer, upper endpoint
 * \return \c segment_t pointer. \c NULL represents failure.
 */
segment_t *segmentOrderSegmentEndPoints(segment_t * const s,
                                        point_t ** const lower, 
                                        point_t ** const upper);

/**
 * Order two segments at a point
 *
 * Order two segments at a point. If the segments intersect at the point, 
 * order by segment slope. Otherwise, order by intersection with the sweep
 * line at the point.
 *
 * \param s1 pointer to first \c segment_t
 * \param s2 pointer to second \c segment_t
 * \param p pointer to \c point_t 
 * \return \c compare_e value
 */
compare_e segmentOrderSegments(segment_t * const s1,
                               segment_t * const s2,
                               point_t * const p);

/**
 * Order a segment and a point
 *
 * Order a segment and a point. If the point does not lie on the segment then
 * order by the position of the point on a sweep line through the point 
 * intersecting the segment.
 *
 * \param s pointer to \c segment_t
 * \param p pointer to \c point_t 
 * \return \c compare_e value
 */
compare_e segmentOrderSegmentAndPoint(segment_t * const s, point_t * const p);

/**
 * Find the intersection of two segments
 *
 * Find the intersection of two segments
 *
 * \param s1 pointer to \c first segment_t
 * \param s2 pointer to second \c segment_t 
 * \param i pointer to \c point_t to store the intersection point
 * \return \c segmentIntersect_e value
 */
segmentIntersect_e segmentIntersection(segment_t * const s1, 
                                       segment_t * const s2, 
                                       point_t * const i);

#ifdef __cplusplus
}
#endif

#endif
