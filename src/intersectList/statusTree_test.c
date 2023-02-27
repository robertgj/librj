/*
 * statusTree_test.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "point.h"
#include "segment.h"
#include "segmentList.h"
#include "statusTree.h"

typedef struct segmentVector_t
{
  point_t a;
  point_t b;
}
segmentVector_t;

static segmentVector_t segmentVector[] = 
  {
    {{0,0,0}, {1,1,0}}, 
    {{1,0,0}, {0,1,0}}, 
    {{1,0,0}, {2,1,0}}, 
    {{2,0,0}, {1,1,0}}, 
    {{0,1,0}, {0,0,0}}, 
    {{0,0,0}, {1,0,0}}, 
    {{-1,0,0}, {0,0,0}}, 
    {{0.5,0,0}, {0.5,1,0}}, 
    {{0,0,0}, {0.5,1,0}}, 
    {{0.75,0,0}, {0.25,1,0}}, 
    {{0.25,0,0}, {0.75,1,0}}, 
    {{0.4,0,0}, {0.6,1,0}}, 
    {{0.6,0,0}, {0.4,1,0}}, 
    {{0.55,0,0}, {0.45,1,0}}, 
    {{0.45,0,0}, {0.55,1,0}}, 
  };

static point_t sweepVector[] = 
  {
    {0,0,0}, 
    {0,1,0},
    {0.5,0.5,0}, 
    {0.25, 0.25, 0},
    {0.2, 0.25, 0},
  };

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
DeallocSegment(segment_t *segment)
{
  Dealloc(segment);
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
      fprintf(stdout, "\n%s(line %u): ", Function, Line);
    }

  va_start(ap, Fmt);
  vfprintf(stdout, Fmt, ap);
  va_end(ap);
  fprintf(stdout, "\n");
}

static
void
Info(const char *Fmt, ...)
{
  va_list ap;
  va_start(ap, Fmt);
  vfprintf(stdout, Fmt, ap);
  va_end(ap);
  fprintf(stdout, "\n");
}

static
void
showSegment(segment_t *s)
{
  point_t *a, *b;
  coord_t ax, ay, bx, by;

  if (s != NULL)
    {
      segmentGetPoints(s, &a, &b);
      pointGetCoords(a, &ax, &ay);
      pointGetCoords(b, &bx, &by);
      Info("Segment a=(%g,%g) b=(%g,%g)", ax, ay, bx, by);
    }
  else
    {
      Info("No segment!");
    }
}

static
void
showVector(segmentVector_t *v)
{
  Info("Segment a=(%g,%g) b=(%g,%g)", v->a.x, v->a.y, v->b.x, v->b.y);
}

bool
statusTreeTest(segmentList_t *sl, point_t *sweep)
{
  segmentListEntry_t *E;
  segment_t *s, *nexts, *prevs;
  statusTree_t *t;
  status_t *S, *nextS, *prevS;

  /* statusInit() */
  t = statusTreeCreate(Alloc, Dealloc, Debug);
  if (t == NULL)
    {
      Debug(__func__, __LINE__, "statusTreeCreate() failed!");
      return false;
    }
  
  /* statusTreeInsert() */
  E = segmentListGetFirst(sl);
  s = segmentListGetSegment(sl, E);
  while(s != NULL)
    {
      if (statusTreeInsert(t, s, sweep) == NULL)
	{
	  Debug(__func__, __LINE__, "statusTreeInsert() failed!");
	  return false;
	}
      E = segmentListGetNext(sl, E);
      s = segmentListGetSegment(sl, E);
    }
  if (statusTreeCheck(t) == false)
    {
      Debug(__func__, __LINE__, "statusTreeCheck() failed!");
      return false;
    }

  /* statusShowTree() */
  statusTreeShow(t, sweep);
  
  S = statusTreeGetUpper(t, sweep);
  s = statusTreeGetSegment(t, S);
  Info("statusTreeGetUpper():");
  showSegment(s);

  S = statusTreeGetLower(t, sweep);
  s = statusTreeGetSegment(t, S);
  Info("statusTreeGetLower():");
  showSegment(s);

  S = statusTreeGetLeftMost(t, sweep);
  s = statusTreeGetSegment(t, S);
  Info("statusTreeGetLeftMost():");
  showSegment(s);

  S = statusTreeGetRightMost(t, sweep);
  s = statusTreeGetSegment(t, S);
  Info("statusTreeGetRightMost():");
  showSegment(s);

  nextS = statusTreeGetNext(t, S);
  nexts = statusTreeGetSegment(t, nextS);
  Info("statusTreeGetNext():");
  showSegment(nexts);

  prevS = statusTreeGetPrevious(t, S);
  prevs = statusTreeGetSegment(t, prevS);
  Info("statusTreeGetPrev():");
  showSegment(prevs);

  /* statusTreeDestroy() */
  statusTreeDestroy(t);

  return true;
}

int
main(void)
{
  unsigned int m, n, numSweeps, numSegments;
  segmentList_t *sl;

  /* For each sweep point ... */
  numSweeps = sizeof(sweepVector)/sizeof(point_t);
  for (m = 0; m < numSweeps; m++)
    {
      Info("\nstatus tree order test %d:", m);

      /* segmentListCreate() */
      sl = segmentListCreate(Alloc, Dealloc, DeallocSegment, Debug);
      if (sl == NULL)
	{
	  Debug(__func__, __LINE__, "segmentListCreate() failed!");
	  return -1;
	}
      
      /* Make segment list */
      numSegments = sizeof(segmentVector)/sizeof(segmentVector_t);
      for (n = 0; n < numSegments; n++)
	{
	  point_t *a, *b;
	  segment_t *s;
	  
	  /* Check valid */
	  a = &(segmentVector[n].a);
	  b = &(segmentVector[n].b);
	  if (pointIsOutsideSweep(a, b, &(sweepVector[m])))
	  {
	    Info("Not using:");
 	    showVector(&(segmentVector[n]));
	    continue;
	  }

	  /* Allocate segment */
	  if ((s = Alloc(sizeof(segment_t))) == NULL)
	    {
	      Debug(__func__, __LINE__, "Alloc() failed!");
	      return -1;
	    }

	  /* segmentListInsert() */
	  segmentSetPoints(s, a, b);
	  if (segmentListInsert(sl, s) == NULL)
	    {
	      Debug(__func__, __LINE__, "segmentListInsert() failed!");
	      return -1;
	    }
	}
      segmentListShow(sl);

      /* statusTreeTest() */
      if (statusTreeTest(sl, &(sweepVector[m])) == false)
	{
	  Debug(__func__, __LINE__, "statusTreeTest() failed!");
	  exit(-1);
	}

      /* segmentListDestroy() */
      segmentListDestroy(sl);
    }

  return 0;
}
