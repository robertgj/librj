/**
 * \file point.h
 *
 * Public interface for a 2-D point type
 */

#if !defined(POINT_H)
#define POINT_H

#ifdef __cplusplus
#include <cstdbool>
extern "C" {
#else
#include <stdbool.h>
#endif

#include "compare.h"

  /**
   * \c real_t
   * Placeholder for a floating point type.
   */
  typedef double real_t;

  /**
   * \c coord_t
   * Placeholder for a coordinate type.
   */
  typedef real_t coord_t;

  /**
   * \c point_t structure. 
   */
  typedef struct point_t
  {
    coord_t x;
    /**< x coordinate. */

    coord_t y;
    /**< y coordinate. */

    real_t tol;
    /**< tolerance on point position. */
  }
    point_t;

  /**
   * Assign coordinates to a point
   *
   * Access function for coordinates of a point
   *
   * \param p pointer to \c point_t being initialised
   * \param x pointer to \e x coordinate
   * \param y pointer to \e y coordinate
   * \return pointer to \c point_t. \c NULL if failure
   */
  point_t *pointSetCoords(point_t * const p, const coord_t x, const coord_t y);

  /**
   * Get coordinates of a point
   *
   * Access function for coordinates of a point
   *
   * \param p pointer to \c point_t being initialised
   * \param x pointer to \e x coordinate
   * \param y pointer to \e y coordinate
   * \return pointer to \c point_t. \c NULL if failure
   */
  point_t *pointGetCoords(point_t *p, coord_t *x, coord_t *y);

  /**
   * Get point tolerance
   *
   * Access function for tolerance of a point
   *
   * \param p pointer to \c point_t being initialised
   * \param tol pointer to \e tol
   * \return pointer to \c point_t. \c NULL if failure
   */
  point_t *pointGetTolerance(point_t * const p, real_t * const tol);

  /**
   * Set point tolerance
   *
   * Access function for tolerance of a point
   *
   * \param p pointer to \c point_t being initialised
   * \param tol \c real_t \e tol
   * \return pointer to \c point_t. \c NULL if failure
   */
  point_t *pointSetTolerance(point_t * const p, const real_t tol);

  /**
   * Copy coordinates of a point
   *
   * Access function for coordinates of a point
   *
   * \param dst pointer to destination point
   * \param src pointer to source point
   */
  void pointCopy(point_t * const dst, const point_t * const src);

  /**
   * Euclidean difference
   *
   * Get the hypotenuse of the triangle formed by points \e a and \e b.
   *
   * \param a pointer to \c point_t
   * \param b pointer to \c point_t
   * \return \c real_t difference value
   */
  real_t pointGetDifference(point_t * const a, point_t * const b);

  /**
   * Compare two points
   *
   * Check if the two points a and b coincide
   *
   * \param a pointer to \c point_t of first segment 
   * \param b pointer to \c point_t of first segment 
   * \return \c compare_e value
   */
  compare_e pointCompare(point_t * const a, point_t * const b);

  /**
   * Test colinearity
   *
   * Check if points a, b and c are colinear.
   *
   * \param a pointer to \c point_t
   * \param b pointer to \c point_t
   * \param c pointer to \c point_t
   * \return \c bool 
   */
  bool pointIsColinear(point_t * const a, point_t * const b, point_t * const c);

  /**
   * Test "between-ness"
   *
   * Check if point c is within the rectangle (or segment) formed by 
   * points a and b inclusive of the boundary.
   *
   * \param a pointer to \c point_t
   * \param b pointer to \c point_t
   * \param c pointer to \c point_t
   * \return \c bool
   */
  bool pointIsBetween(point_t * const a, point_t * const b, point_t * const c);

  /**
   * Segment helper function for "outside-ness" of a sweep line
   *
   * Check if the sweep line defined by point c is "outside" the y-range of 
   * the segment formed by points a and b.
   *
   * \param a pointer to \c point_t of segment
   * \param b pointer to \c point_t of segment
   * \param c pointer to \c point_t defining sweep line
   * \return \c bool
   */
  bool pointIsOutsideSweep(point_t * const a, point_t * const b, point_t * const c);


  /**
   * Segment helper function for "lower-ness" test of two segments
   *
   * Check if the segment defined by points a and b is "lower" than that
   * formed buy c and d in the sense that either a or b is "below" c or d.
   *
   * \param a pointer to \c point_t of first segment
   * \param b pointer to \c point_t of first segment
   * \param c pointer to \c point_t of second segment
   * \param d pointer to \c point_t of second segment
   * \return \c bool
   */
  bool pointSegmentIsLower(point_t * const a, 
                           point_t * const b,
                           point_t * const c,
                           point_t * const d);

  /**
   * Segment helper function for "left-ness" test of two segments
   *
   * Check if the segment defined by points a and b is "lefter" than that
   * formed buy c and d in the sense that either a or b is "left of" c or d.
   *
   * \param a pointer to \c point_t of first segment
   * \param b pointer to \c point_t of first segment
   * \param c pointer to \c point_t of second segment
   * \param d pointer to \c point_t of second segment
   * \return \c bool
   */
  bool pointSegmentIsLefter(point_t * const a, 
                            point_t * const b, 
                            point_t * const c, 
                            point_t * const d);

  /**
   * Segment helper function for inverse slope of segment
   *
   * Calculate the inverse slope of the segment defined by points a and b
   *
   * \param a pointer to \c point_t of segment
   * \param b pointer to \c point_t of segment
   * \param invSlope value of inverse of slope of segment
   * \return \c bool indicating success
   */
  bool pointSegmentInvSlope(point_t * const a,
                            point_t * const b, 
                            real_t  * const invSlope);

  /**
   * Segment helper function for sweep line intersection
   *
   * Find the point of intersection of a segment and a sweep line
   *
   *
   * \param a pointer to \c point_t of segment
   * \param b pointer to \c point_t of segment
   * \param p pointer to \c point_t defining sweep line
   * \param i pointer to \c point_t to store intersection point
   * \return \c compare_e value
   */
  compare_e pointSegmentSweepIntersection(point_t * const a, 
                                          point_t * const b, 
                                          point_t * const p, 
                                          point_t * const i);

  /**
   * Segment helper function for segment intersection
   *
   * Find the point of intersection of two segments. Several values are 
   * returned:\n
   *  - lines are parallel \n
   *  - parametric extension of segment at the point of intersection 
   *    (ie: in vectors \c a+s*(b-a) ) \n
   *  - point of intersection \n
   *
   * \param a pointer to \c point_t of first segment
   * \param b pointer to \c point_t of first segment
   * \param c pointer to \c point_t of second segment
   * \param d pointer to \c point_t of second segment
   * \param areParallel pointer to \c bool true if segments are parallel
   * \param s pointer to real value of parametric extension of first segment
   * \param t pointer to real value of parametric extension of second segment
   * \param p pointer to \c point_t of intersection
   */
  void pointSegmentIntersection(point_t * const a, 
                                point_t * const b, 
                                point_t * const c, 
                                point_t * const d, 
                                bool    * const areParallel,
                                real_t  * const s, 
                                real_t  * const t, 
                                point_t * const p);

#ifdef __cplusplus
}
#endif

#endif
