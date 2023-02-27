/**
 * \file interp_show.c 
 */

#include "interp_utility.h"
#include "interp_data.h"

bool
interpShow(void * const entry, void * const user)
{
  (void)user;
  interpMessage("%ld", *(data_t *)entry);
  return true;
}

