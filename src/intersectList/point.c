/**
 * \file point.c
 *
 * Implementation of a point type.
 */

#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>

#include "compare.h"
#include "point.h"

/**
 * pointEps sets a tolerance threshold for line segment intersections
 * calculated in \e pointSegmentIntersection() and 
 * \e pointSegmentSweepIntersection(). The former calculates the
 * tolerance for the point by calculating the two possible
 * intersections of two segments with floating-point (approximate)
 * arithmentic. It is possible that this tolerance is zero. The latter
 * function orders a segment and point by calculating the intersection
 * of a segment and sweep line. That intersection may differ slightly
 * from the first. So, pointEps is the minimum tolerance for the
 * interior intersection of two segments or a sweep line intersecting
 * with the interior of a segment. The recommended approach is to tune
 * pointEps to your situation. Given the device pixel resolution
 * choose a sub-pixel resolution and scale positions to that sub-pixel
 * resolution then set the pointEps value to somewhat more than the
 * floating point precision of the worst case intersections on your
 * system.
 */
const real_t pointEps = 1e-8;

/**
 * Private helper function for point_t implementation.
 *
 * Find the hypotenuse. Modify for different coord_t types and for C
 * libraries having a \e hypot() function.
 *
 * \param x
 * \param y 
 * \return hypotenuse of trangle formed by \e x and \e y
 */
static real_t
pointGetHypot(const real_t x, const real_t y)
{
  return hypot(x, y);
}

point_t *
pointSetCoords(point_t * const p, const coord_t x, const coord_t y)
{
  if (p == NULL)
    {
      return NULL;
    }

  p->x = x;
  p->y = y;
  p->tol = 0;

  return p;
}

point_t *
pointSetTolerance(point_t * const p, const real_t tol)
{
  if (p == NULL)
    {
      return NULL;
    }

  p->tol = tol;

  return p;
}

point_t *
pointGetCoords(point_t * const p, coord_t * const x, coord_t * const y)
{
  if ((p == NULL) || (x == NULL) || (y == NULL))
    {
      return NULL;
    }

  *x = p->x;
  *y = p->y;

  return p;
}

point_t *
pointGetTolerance(point_t * const p, real_t * const tol)
{
  if (p == NULL)
    {
      return NULL;
    }

  *tol = p->tol;

  return p;
}

void
pointCopy(point_t * const dst, const point_t * const src)
{
  if ((dst == NULL) || (src == NULL))
    {
      return;
    }

  dst->x = src->x;
  dst->y = src->y;
  dst->tol = src->tol;
}

real_t
pointGetDifference(point_t * const a, point_t * const b)
{
  coord_t dx, dy;
  
  if ((a == NULL) || (b == NULL))
    {
      return 0;
    }

  dx = a->x-b->x;
  dy = a->y-b->y;

  return pointGetHypot(dx, dy);
}

compare_e
pointCompare(point_t * const a, point_t * const b)
{
  if ((a == NULL) || (b == NULL))
    {
      return compareError;
    }

  /* Approximate comparison */
  if ((a->tol > 0) || (b->tol > 0))
    {
      real_t diff;

      diff = pointGetDifference(a, b);
      if ((diff <= a->tol) || (diff <= b->tol))
        {
          return compareEqual;
        }
    }

  /* Exact comparison */
  if (a->y == b->y)
    {
      if (a->x == b->x)
        {
          return compareEqual;
        }
      else if (a->x > b->x)
        {
          return compareGreater;
        }
      else
        {
          return compareLesser;
        }
    }
  else if (a->y > b->y)
    {
      return compareGreater;
    }
  else
    {
      return compareLesser;
    }
}

bool
pointIsColinear(point_t * const a, point_t * const b, point_t * const c)
{
  real_t Area;

  Area = ((b->x-a->x)*(c->y-a->y)) - ((c->x-a->x)*(b->y-a->y));

  return Area == 0 ? true : false;
}

bool
pointIsBetween(point_t * const a, point_t * const b, point_t * const c)
{
  int v;
  v = 
    (((a->x <= c->x) && (c->x <= b->x)) || 
     ((a->x >= c->x) && (c->x >= b->x)))
    &&
    (((a->y <= c->y) && (c->y <= b->y)) || 
     ((a->y >= c->y) && (c->y >= b->y)));

  return ((v == 0) ? false : true);
}

bool
pointIsOutsideSweep(point_t * const a, point_t * const b, point_t * const c)
{
  if (((a->y < c->y) && (b->y < c->y)) 
      ||
      ((a->y > c->y) && (b->y > c->y)))
    {
      return true;
    }

  return false;
}

bool
pointSegmentIsLower(point_t * const a, 
                    point_t * const b, 
                    point_t * const c, 
                    point_t * const d)
{
  if (((a->y < c->y) && (a->y < d->y))
      ||
      ((b->y < c->y) && (b->y < d->y))
      ||
      ((c->y > a->y) && (c->y > b->y))
      ||
      ((d->y > a->y) && (d->y > b->y)))
    {
      return true;
    }
  else
    {
      return false;
    }
}

bool
pointSegmentIsLefter(point_t * const a, 
                     point_t * const b, 
                     point_t * const c,
                     point_t * const d)
{
  if (((a->x < c->x) && (a->x < d->x))
      ||
      ((b->x < c->x) && (b->x < d->x))
      ||
      ((c->x > a->x) && (c->x > b->x))
      ||
      ((d->x > a->x) && (d->x > b->x)))
    {
      return true;
    }
  else
    {
      return false;
    }
}


bool
pointSegmentInvSlope(point_t * const a, 
                     point_t * const b, 
                     real_t * const invSlope)
{
  /* Sanity check */
  if ((a == NULL) || (b == NULL) || (invSlope == NULL))
    {
      return false;
    }


  /*
   *  Return 0 if inverse of slope isn't finite
   */
  if (a->y == b->y)
    {
      return false;
    }

  /*
   * Inverse of slope
   */
  *invSlope = ((real_t)(b->x - a->x))/((real_t)(b->y - a->y));

  return true;
}

compare_e
pointSegmentSweepIntersection(point_t * const a, point_t * const b, point_t * const p, point_t * const i)
{
  if ((a == NULL) || (b == NULL) || (p == NULL) || (i == NULL))
    {
      return compareError;
    }

  i->x = p->x;
  i->y = p->y;
  i->tol = 0;

  /*
   *  Intersection possible?
   */
  if (pointIsOutsideSweep(a, b, p))
    {
      return compareError;
    }

  /*
   * Intersection with sweep line at p->y
   */
      
  /* Vertical line */
  if (a->x == b->x)
    {
      i->x = a->x;
    }
  /* Horizontal line */
  else if (a->y == b->y)
    {
      if (((a->x <= p->x) && (p->x <= b->x)) || 
          ((b->x <= p->x) && (p->x <= a->x)))
        {
          i->x = p->x;
        }
      else
        {
          /* Use left most end-point for horizontal intersection */
          i->x = (pointCompare(a, b) == compareLesser) ? a->x : b->x;
        }
    }

  /* sweep level with endpoint */
  else if (p->y == a->y)
    {
      i->x = a->x;
    }
  else if (p->y == b->y)
    {
      i->x = b->x;
    } 
  /* Brute force slope/intercept */
  else
    {
      real_t invSlope;

      if (pointSegmentInvSlope(a, b, &invSlope) == true)
        {
          i->x = a->x + ((p->y - a->y)*invSlope);
          i->tol = pointEps;
        }
      else
        {
          i->x = a->x;
        }
    }

  return compareEqual;
}

void
pointSegmentIntersection(point_t * const a, 
                         point_t * const b, 
                         point_t * const c, 
                         point_t * const d, 
                         bool    * const areParallel,
                         real_t  * const s, 
                         real_t  * const t, 
                         point_t * const p)
{
  real_t num, denom;
  point_t ps, pt;

  *areParallel = false;
  *s = 0;
  *t = 0;
  p->x = 0;
  p->y = 0;
  p->tol = 0;

  /* 
   *  Follow O'Rourke's SegSegInt routine. Express the segments
   *  as parametric equations:
   *    u(s) = a + s*(b-a)
   *    v(t) = c + t*(d-c)
   *  Find s and t at the intersection.
   */
  denom = 
    a->x * (d->y - c->y) +
    b->x * (c->y - d->y) +
    d->x * (b->y - a->y) +
    c->x * (a->y - b->y);
  
  if (denom == 0)
    {
      *areParallel = true;
      return;
    }
  
  num =     
    a->x * (d->y - c->y) +
    c->x * (a->y - d->y) +
    d->x * (c->y - a->y);

  *s = num/denom;
  
  num = - (a->x * (c->y - b->y) +
           b->x * (a->y - c->y) +
           c->x * (b->y - a->y));
  *t = num/denom;

  if ((0 >= (*t)) || ((*t) >= 1) || (0 >= (*s)) || ((*s) >= 1))
    {
      return;
    }

  /* Intersection within both segments */
  ps.x = a->x + (*s)*(b->x - a->x);
  ps.y = a->y + (*s)*(b->y - a->y);
  ps.tol = 0;
  pt.x = c->x + (*t)*(d->x - c->x);
  pt.y = c->y + (*t)*(d->y - c->y);
  pt.tol = 0;
  
  /* Default p */
  p->x = (ps.x+pt.x)/2.0;
  p->y = (ps.y+pt.y)/2.0;
  
  /* Find tolerance on intersection point */
  p->tol = pointGetDifference(&ps, &pt);
  if (p->tol < pointEps)
    {
      p->tol = pointEps;
    }

  /* Correct P for horizontal/vertical line intersections */
  if (a->x == b->x)
    {
      p->x = p->x;
    }
  else if (c->x == d->x)
    {
      p->x = c->x;
    }
  if (a->y == b->y)
    {
      p->y = a->y;
    }
  else if (c->y == d->y)
    {
      p->y = c->y;
    }
}
