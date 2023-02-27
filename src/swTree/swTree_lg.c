/**
 * \file swTree_lg.c
 */

#include <stdlib.h>

size_t
swTreeFloorLg(const size_t size)
{
  size_t lg=0;
  for(size_t exp2=size+1; exp2>1; exp2=exp2/2)
    {
      lg=lg+1;
    }
  return lg;
}

size_t
swTreeTwoFloorLg(const size_t size)
{
  size_t lg=swTreeFloorLg(size);
  
  size_t two_floor_lg = 1;
  for (size_t k=0; k<lg; k++)
    {
      two_floor_lg = two_floor_lg*2;
    }

  return two_floor_lg;
}

size_t
swTreeTwoCeilLg(const size_t size)
{
  size_t two_floor_lg=swTreeTwoFloorLg(size);

  if ((size+1)>two_floor_lg)
    {
      return 2*two_floor_lg;
    }
  else
    {
      return two_floor_lg;
    }
}
