/**
 * \file swTree_lg_test.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "swTree_lg.h"

int main(void)
{

  printf("n    floor(lg(n+1)) 2^floor(lg(n+1)) 2^ceil(lg(n+1))\n");
  for (size_t n=0; n<100; n++)
    {
      printf("%4lu %4lu           %4lu             %4lu\n",
             n, swTreeFloorLg(n), swTreeTwoFloorLg(n), swTreeTwoCeilLg(n));
    }
  
  return 0;
}
