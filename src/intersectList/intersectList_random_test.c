/*
 * intersectList_test.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "compare.h"
#include "point.h"
#include "segment.h"
#include "segmentList.h"
#include "intersectList.h"

static
void *
Alloc(const size_t size)
{
  return malloc(size);
}

static
void 
Dealloc(void *ptr)
{
  free(ptr);
}

static
void
DeallocSegment(segment_t *s)
{
  point_t *a, *b;

  segmentGetPoints(s, &a, &b);
  Dealloc(a);
  Dealloc(b);
  Dealloc(s);
}

static
segment_t *
AllocSegment(coord_t ax, coord_t ay, coord_t bx, coord_t by)
{
  point_t *a, *b;
  segment_t *s;

  a = Alloc(sizeof(point_t));
  if (a == NULL)
    {
      return NULL;
    }
  b = Alloc(sizeof(point_t));
  if (b == NULL)
    {
      Dealloc(a);
      return NULL;
    }
  s = Alloc(sizeof(segment_t));
  if (s == NULL)
    {
      Dealloc(a);
      Dealloc(b);
      return NULL;
    }

  pointSetCoords(a, ax, ay);
  pointSetTolerance(a, 0);
  pointSetCoords(b, bx, by);
  pointSetTolerance(b, 0);

  return segmentSetPoints(s, a, b);
}

static
void
Debug(const char *Function, 
      const unsigned int Line, 
      const char *Fmt, 
      ...)
{
  va_list ap;

  if (Function != NULL)
    {
      fprintf(stderr, "\n%s(line %u): ", Function, Line);
    }

  va_start(ap, Fmt);
  vfprintf(stderr, Fmt, ap);
  va_end(ap);
  fprintf(stderr, "\n");
}

static
void
Info(const char *Fmt, ...)
{
  va_list ap;
  va_start(ap, Fmt);
  vfprintf(stdout, Fmt, ap);
  va_end(ap);
}

/* 
 * A simple random number generator from p.279 of 
 * "Numerical Recipes in C" 2nd Edn. OS independent!
 */
static
int ran0(int range)
{
  static int idum = -559038737;
  const int ia = 16807;
  const int im = 2147483647;
  const double am = (1.0/im);
  const int iq = 127773;
  const int ir = 2836;
  const int mask = 123459876;
  int k;
  double ans;
  int ret;

  idum ^= mask;
  k = idum/iq;
  idum = ia*(idum-(k*iq))-(ir*k);
  if(idum < 0) idum += im;
  ans = am*idum;
  idum ^= mask;

  if (range <= 0)
    ret = 0;
  else
    ret = (int)(ans*((double)range-1));

  return ret;
}

static
segment_t *
RandomSegment(void)
{
  segment_t *s;
  coord_t ax, ay,bx,by;

  /* Get some random coordinate values */
  ax = ran0(400);
  ay = ran0(400);
  bx = ran0(400);
  by = ran0(400);

  /* Read a line successfully! Allocate memory. */
  if ((s = AllocSegment(ax, ay, bx, by)) == NULL)
    {
      Debug(__func__, __LINE__, "AllocSegment() failed!");
      exit(-1);
    }

  return s;
}


static
void
writePS(intersectList_t *il, segmentList_t *sl)
{
  intersect_t *i;
  segmentListEntry_t *entry;
  segment_t *s;
  point_t *p, *a, *b;
  coord_t ax, ay, bx, by, px, py;

  Info("%%!PS\n");

  entry = segmentListGetFirst(sl);
  while(entry != NULL)
    {
      s = segmentListGetSegment(sl, entry);
      if (segmentGetPoints(s, &a, &b) == NULL)
	{
	  return;
	}
      pointGetCoords(a, &ax, &ay);
      pointGetCoords(b, &bx, &by);
      Info("%g %g moveto %g %g lineto stroke\n", ax, ay, bx, by);

      entry = segmentListGetNext(sl, entry);
    }

  i = intersectListGetFirst(il);
  while(i != NULL)
    {
      if ((p = intersectGetPoint(i)) == NULL)
      {
	return;
      }
      
      pointGetCoords(p, &px, &py);
      Info("%g %g 3 0 360 arc fill\n", px, py);
      i = intersectListGetNext(il, i);
    }

  Info("showpage\n");
}


int main(void)
{
  const int ListSize = 64;
  int i;
  intersectList_t *il;
  segmentList_t *sl;

  /* segmentListCreate() */
  sl = segmentListCreate(Alloc, Dealloc, DeallocSegment, Debug);
  if (sl == NULL)
    {
      Debug(__func__, __LINE__, "segmentListCreate() failed!");
      return -1;
    }

  /* Make segments */
  for (i=0; i<ListSize; i++)
    {
      segment_t *s;

      s = RandomSegment();
      if (segmentListInsert(sl, s) == NULL)
	{
	  segmentListDestroy(sl);
	  Debug(__func__, __LINE__, "segmentListInsert() failed!");
	  return -1;
	}
    }

  /* intersectListCreate() */
  il = intersectListCreate(Alloc, Dealloc, Debug);
  if (il == NULL)
    {
      segmentListDestroy(sl);
      Debug(__func__, __LINE__, "intersectListCreate() failed!");
      return -1;
    }
  
  /* intersectListScanSegments() */
  if (intersectListScanSegments(il, sl) == false)
    {
      segmentListDestroy(sl);
      intersectListDestroy(il);
      Debug(__func__, __LINE__, "intersectListScanSegments() failed!");
      return -1;	  
    }

  /* Write PS output */
  writePS(il, sl);

  /* intersectDestroy() */
  intersectListDestroy(il);
  
  /* segmentDestroy() */
  segmentListDestroy(sl);

  return 0;
}
