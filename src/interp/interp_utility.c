/**
 * \file interp_utility.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


void
interpMessage(const char *format, ...)
{
  va_list ap;

  va_start(ap, format);
  vfprintf(stdout, format, ap);
  va_end(ap);
  fprintf(stdout, "\n");
  fflush(stdout);
}

void
interpError(const char *function, 
            const unsigned int line, 
            const char *format, 
            ...)
{
  va_list ap;

  if (function != NULL)
    {
      fprintf(stderr, "\n%s(line %u): ", function, line);
    }

  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);

  fprintf(stderr, "\n");
  fflush(stderr);
  exit(-1);
}


long interpRand(const long range)
{
  static int idum = (int)0xdeadbeef;  /* -559038737 */
  const int ia = 16807; 
  const int im = 2147483647;
  const double am = (1.0/im);
  const int iq = 127773;
  const int ir = 2836;
  const int mask = 123459876;
  int k;
  double ans;
  long ret;
  
  idum ^= mask;
  k = idum/iq;
  idum = ia*(idum-(k*iq))-(ir*k);
  if(idum < 0) idum += im;
  ans = am*idum;
  idum ^= mask;

  if (range <= 0)
    ret = 0;
  else
    ret = (long)(ans*((double)range-1));

  return ret;
}

