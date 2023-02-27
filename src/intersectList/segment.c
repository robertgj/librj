/**
 * \file segment.c
 *
 * Implementation of segment_t type
 */
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "point.h"
#include "segment.h"

/**
 * Private helper function for segment_t implementation.
 *
 * Test for segment equality. Do endpoints coincide?
 *
 * \param s1 pointer to first \c segment_t
 * \param s2 pointer to second \c segment_t
 * \return \c bool true if equal
 */
static
bool 
segmentAreEqual(segment_t * const s1, segment_t * const s2)
{
  point_t *a, *b, *c, *d;

  if ((s1 == NULL) || (s2 == NULL))
    {
      return false;
    }

  a = s1->a;
  b = s1->b;
  c = s2->a;
  d = s2->b;

  if (((a == c) && (b == d)) 
      || 
      ((a == d) && (b == c))
      ||
      ((pointCompare(a, c) == compareEqual) && 
       (pointCompare(b, d) == compareEqual))
      ||
      ((pointCompare(b, c) == compareEqual) && 
       (pointCompare(a, d) == compareEqual)))
    {
      return true;
    }

  return false;
}

/**
 * Private helper function for segment_t implementation.
 *
 * Order two segments by their slope. If both segments have finite 
 * slope the situation is:
 \verbatim
       P   or   P           etc.
      /\        \  `
   +ve  -ve     +ve  +ve
   1st  2nd     1st  2nd
 \endverbatim
 *
 *  We use the inverse slope so that small positive slopes are very
 *  positive and small negative slopes are very negative and large
 *  slopes are small. Horizontal segments rank lowest.
 *
 * \param segment1 pointer to first \c segment_t
 * \param segment2 pointer to second \c segment_t
 * \return \c compare_e value
 */
static
compare_e
segmentOrderSegmentsBySlope(segment_t * const segment1, 
                            segment_t * const segment2)
{
  if (segmentAreEqual(segment1, segment2))
    {
      /* Equal segments! */
      return compareEqual;
    }

  /*
   *  Choose an order for the segments by intersection
   *  with a sweep line lower than the current sweep point.
   */
  point_t *point1a, *point1b, *point2a, *point2b;
  point_t *lower1, *upper1, *lower2, *upper2;
  real_t invSlope1, invSlope2;
  bool invSlope1isFinite, invSlope2isFinite;

  point1a = point1b = point2a = point2b = NULL;
  lower1 = upper1 = lower2 = upper2 = NULL;

  segmentGetPoints(segment1, &point1a, &point1b);
  segmentGetPoints(segment2, &point2a, &point2b);

  segmentOrderSegmentEndPoints(segment1, &lower1, &upper1);
  segmentOrderSegmentEndPoints(segment2, &lower2, &upper2);
      
  invSlope1isFinite = pointSegmentInvSlope(lower1, upper1, &invSlope1);
  invSlope2isFinite = pointSegmentInvSlope(lower2, upper2, &invSlope2);
      
  if (invSlope1isFinite && invSlope2isFinite)
    {
      if ((invSlope1) < (invSlope2))
        {
          return compareGreater;
        }
      else if ((invSlope1) > (invSlope2))
        {
          return compareLesser;
        }
      else if (pointSegmentIsLefter(point1a, point1b, point2a, point2b))
        {
          return compareLesser;
        }
      else if (pointSegmentIsLower(point1a, point1b, point2a, point2b))
        {
          return compareLesser;
        }
      else
        {
          return compareGreater;
        }
    }
  else if (invSlope1isFinite && (!invSlope2isFinite))
    {
      return compareGreater;
    }
  else if ((!invSlope1isFinite) && invSlope2isFinite)
    {
      return compareLesser;
    }
  else
    {
      if (pointSegmentIsLefter(point1a, point1b, point2a, point2b))
        {
          return compareLesser;
        }
      else
        {
          return compareGreater;
        }
    }
}

/**
 * Private helper function for segment_t implementation.
 *
 * Find the intersection point of two parallel segments.
 *
 * \param s1 pointer to first \c segment_t
 * \param s2 pointer to second \c segment_t
 * \param p pointer to \c point_t for intersection point
 * \return \c segmentIntersect_e value
 */
static
segmentIntersect_e
segmentParallelIntersection(segment_t * const s1,
                            segment_t * const s2,
                            point_t * const p)
{
  point_t *upper1, *lower1, *upper2, *lower2;

  if ((s1 == NULL) || (s2 == NULL) || (p == NULL))
    {
      return segmentDisjoint;
    }
      
  segmentOrderSegmentEndPoints(s1, &lower1, &upper1);
  segmentOrderSegmentEndPoints(s2, &lower2, &upper2);

  /* Test for colinear */
  if (!pointIsColinear(lower1, upper1, lower2))
    {
      return segmentDisjoint;
    }

  /* Find in-between points */
  if (pointIsBetween(lower1, upper1, lower2))
    {
      pointCopy(p, lower2);
      return segmentInteriorS1;
    }
  
  if (pointIsBetween(lower1, upper1, upper2))
    {
      pointCopy(p, upper2);
      return segmentInteriorS1;
    }

  if (pointIsBetween(lower2, upper2, lower1))
    {
      pointCopy(p, lower1);
      return segmentInteriorS2;
    }
  
  if (pointIsBetween(lower2, upper2, upper1))
    {
      pointCopy(p, upper1);
      return segmentInteriorS2;
    }

  return segmentDisjoint;
}

segment_t * 
segmentGetPoints(segment_t * const s, 
                 point_t ** const a, 
                 point_t ** const b)
{
  if ((s == NULL) || (a == NULL) || (b == NULL))
    {
      return NULL;
    }

  *a = s->a;
  *b = s->b;

  return s;
}

segment_t * 
segmentSetPoints(segment_t * const s, point_t * const a, point_t * const b)
{
  if ((s == NULL) || (a == NULL) || (b == NULL))
    {
      return NULL;
    }

  s->a = a;
  s->b = b;

  return  s;
}

const
char *
segmentIntersectEnum2Str(segmentIntersect_e i)
{
  const char *str;

  switch(i)
    {
    case segmentPointsDisjoint:
      str = "segmentPointsDisjoint";
      break;
    case segmentPointsCoincide:
      str = "segmentPointsCoincide";
      break;
    case segmentDisjoint:
      str = "segmentDisjoint";
      break;
    case segmentInteriorS1:
      str = "segmentInteriorS1";
      break;
    case segmentInteriorS2:
      str = "segmentInteriorS2";
      break;
    case segmentInterior:
      str = "segmentInterior";
      break;
    case segmentVertex:
      str = "segmentVertex";
      break;
    case segmentCoincide:
      str = "segment_Coincide";
      break;
    default:
      str = "unknown";
      break;
    }

  return str;
}

segment_t *
segmentOrderSegmentEndPoints(segment_t * const s,
                             point_t ** const lower,
                             point_t ** const upper)
{
  if ((s == NULL) || (lower == NULL) || (upper == NULL))
    {
      return NULL;
    }
  segmentGetPoints(s, upper, lower);
  if ((*lower == NULL) || (*upper == NULL))
    {
      return NULL;
    }
  if (pointCompare(*upper, *lower) == compareLesser)
    {
      segmentGetPoints(s, lower, upper);
    }

  return s;
}

compare_e
segmentOrderSegments(segment_t * const s1, 
                     segment_t * const s2, 
                     point_t * const p)
{
  point_t i1, i2, *a, *b, *c, *d;
  compare_e result;

  /* Check for same segment by address */
  if (s1 == s2)
    {
      return compareEqual;
    }
  
  a = b = c = d = NULL;

  segmentGetPoints(s1, &a, &b);
  segmentGetPoints(s2, &c, &d);

  /* Find sweep line intersection for each segment */
  result = pointSegmentSweepIntersection(a, b, p, &i1);
  if(result != compareEqual)
    {
      return compareError;
    }
  result = pointSegmentSweepIntersection(c, d, p, &i2);
  if(result != compareEqual)
    {
      return compareError;
    }

  /* Order by sweep line intersection in x */
  result = pointCompare(&i1, &i2);
  if (result == compareEqual)
    {
      /* Segments intersect at the sweep point! */
      return segmentOrderSegmentsBySlope(s1, s2);
    }
  else
    {
      return result;
    }
}

compare_e
segmentOrderSegmentAndPoint(segment_t * const s, point_t * const p)
{
  point_t *a, *b, i;
  compare_e result;

  /* Sanity check */
  if ((s == NULL) || (p == NULL))
    {
      return compareError;
    }

  /* Shared endpoints? */
  segmentGetPoints(s, &a, &b);
  if ((pointCompare(p, a) == compareEqual) || 
      (pointCompare(p, b) == compareEqual))
    {
      return compareEqual;
    }

  /* Find the sweep intersection from point P */
  result = pointSegmentSweepIntersection(a, b, p, &i);
  if (result != compareEqual)
    {
      return compareError;
    }

  return pointCompare(p, &i);
}

segmentIntersect_e
segmentIntersection(segment_t * const s1, 
                    segment_t * const s2, 
                    point_t * const p)
{
  point_t *a, *b, *c, *d;

  a = b = c = d = NULL;

  segmentGetPoints(s1, &a, &b);
  segmentGetPoints(s2, &c, &d);

  /* Default assignment of p */
  pointCopy(p, a);

  /* Equal addresses */
  if (s1 == s2)
    {
      return segmentCoincide;
    }

  /* Just points */
  else if ((pointCompare(a, b) == compareEqual) && 
           (pointCompare(c, d) == compareEqual))
    {
      if (pointCompare(a, c) == compareGreater)
        {
          pointCopy(p, c);
        }
      return (pointCompare(a, c) == compareEqual) ? 
        segmentPointsCoincide : segmentPointsDisjoint;
    }

  /* Two equal endpoints */
  else if (((pointCompare(a, c) == compareEqual) && 
            (pointCompare(b, d) == compareEqual)) 
           ||
           ((pointCompare(a, d) == compareEqual) && 
            (pointCompare(b, c) == compareEqual)))
    {
      if (pointCompare(a, b) > 0)
        {
          pointCopy(p, b);
        }
      return segmentCoincide;
    }

  /* One equal endpoint */
  else if (pointCompare(a, c) == compareEqual)
    {
      return segmentVertex;
    }
  else if (pointCompare(a, d) == compareEqual)
    {
      return segmentVertex;
    }
  else if (pointCompare(b, c) == compareEqual)
    {
      pointCopy(p, b);
      return segmentVertex;
    }
  else if (pointCompare(b, d) == compareEqual)
    {
      pointCopy(p, b);
      return segmentVertex;
    }

  /* General case */
  else 
    {
      bool areParallel;
      compare_e result;
      real_t s, t;
      point_t i;

      pointSegmentIntersection(a, b, c, d, &areParallel, &s, &t, p);

      /* Parallel */
      if (areParallel)
        {
          return segmentParallelIntersection(s1, s2, p);
        }
      
      /* Disjoint */
      else if ((0 > t) || (t > 1) || (0 > s) || (s > 1))
        {
          return segmentDisjoint;
        }

      /* Intersect at endpoints of both */
      else if (((s == 0) || (s == 1)) && (((t == 0) || (t == 1))))
        {
          (s == 0) ? pointCopy(p, a) : pointCopy(p, b);
          return segmentVertex;
        }

      /* Intersect at endpoint of s1, interior of s2 */
      else if ((s == 0) || (s == 1))
        {
          (s == 0) ? pointCopy(p, a) : pointCopy(p, b);
          result = pointSegmentSweepIntersection(c, d, p, &i);
          if (result != compareEqual)
            {
              return segmentError;
            }
          p->tol = 2*pointGetDifference(p, &i);
          return segmentInteriorS2;
        }

      /* Intersect at endpoint of s2, interior of s1 */
      else if ((t == 0) || (t == 1))
        {
          (t == 0) ? pointCopy(p, c) : pointCopy(p, d);
          result = pointSegmentSweepIntersection(a, b, p, &i);
          if (result != compareEqual)
            {
              return segmentError;
            }
          p->tol = 2*pointGetDifference(p, &i);
          return segmentInteriorS1;
        }

      /* Intersect in interior of both. (p->tol already set) */
      else
        {
          return segmentInterior;
        }
    }
}

