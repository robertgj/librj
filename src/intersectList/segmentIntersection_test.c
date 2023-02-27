/*
 * segmentIntersection_test.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "compare.h"
#include "point.h"
#include "segment.h"
#include "segmentList.h"

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
segment_t *
readSegment(void)
{
  segment_t *s;
  const int StrSize = 50;
  char Str[StrSize];
  char movetoToken[StrSize];
  char linetoToken[StrSize];
  char strokeToken[StrSize];
  float ax, ay, bx, by;

  /*
   *  Input format is:
   *
   *  %!PS
   *  100 100 lineto 300 300 moveto stroke
   *  .
   *  .
   *  showpage
   */

  while (fgets(&(Str[0]), StrSize, stdin) != NULL)
    {
      if ((*Str == '%') || (*Str == '\n') || (*Str == '\r'))
        {
          continue;
        }

      if (!strncmp(&(Str[0]), "showpage", strlen("showpage")))
        {
          return NULL;
        }

      if((sscanf(Str, 
                 "%g %g %s %g %g %s %s", 
                 &ax, &ay, 
                 movetoToken, 
                 &bx, &by, 
                 linetoToken, strokeToken) != 7) ||
         strcmp(movetoToken, "moveto") ||
         strcmp(linetoToken, "lineto") ||
         strcmp(strokeToken, "stroke"))
        {
          Debug(__func__, __LINE__, "sscanf() failed for %s!", Str);
          exit(-1);
        }

      /* Read a line successfully! Allocate memory. */
      if ((s = AllocSegment(ax, ay, bx, by)) == NULL)
        {
          Debug(__func__, __LINE__, "AllocSegment() failed!");
          exit(-1);
        }

      return s;
    }

  return NULL;
}

int
main(void)
{
  segmentList_t *sl;
  segmentListEntry_t *sle;
  segment_t *s, *s1, *s2;
  segmentIntersect_e segIntType;
  point_t intersect, *a, *b;
  coord_t ax, ay, bx, by, ix, iy;
  bool InputOK;

  /* segmentListCreate() */
  sl = segmentListCreate(Alloc, Dealloc, DeallocSegment, Debug);
  if (sl == NULL)
    {
      Debug(__func__, __LINE__, "segmentListCreate() failed!");
      return -1;
    }

  /* Read segments */
  InputOK = false;
  while ((s = readSegment()) != NULL)
    {
      /* Got something! */
      InputOK = true;

      /* segmentListInsert() */
      if (segmentListInsert(sl, s) == NULL)
        {
          segmentListDestroy(sl);
          Debug(__func__, __LINE__, "segmentListInsert() failed!");
          return -1;
        }
    }

  /* Check got something */
  if (InputOK == false)
    {
      segmentListDestroy(sl);
      Debug(__func__, __LINE__,  "readSegment() failed!");
      return -1;      
    }

  /* segmentIntersection() */
  sle = segmentListGetFirst(sl);
  while (sle != NULL)
    {
      s1 = segmentListGetSegment(sl, sle);
      sle = segmentListGetNext(sl, sle);
      if (sle == NULL)
        {
          break;
        }
      s2 = segmentListGetSegment(sl, sle);

      segIntType = segmentIntersection(s1, s2, &intersect);
      Info("Segment intersect test:");
      segmentGetPoints(s1, &a, &b);
      pointGetCoords(a, &ax, &ay);
      pointGetCoords(b, &bx, &by);
      Info("s1: (%g,%g) (%g,%g)", ax, ay, bx, by);
      segmentGetPoints(s2, &a, &b);
      pointGetCoords(a, &ax, &ay);
      pointGetCoords(b, &bx, &by);
      Info("s2: (%g,%g) (%g,%g)", ax, ay, bx, by);
      pointGetCoords(&intersect, &ix, &iy);
      Info("type %s at (%g, %g)", 
           segmentIntersectEnum2Str(segIntType), ix, iy);

      sle = segmentListGetNext(sl, sle);
    }
  
  /* segmentDestroy() */
  segmentListDestroy(sl);

  return 0;
}
