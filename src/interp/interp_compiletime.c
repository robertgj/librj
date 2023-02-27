#include <stdlib.h>
#include <limits.h>

#include "interp_ex.h"
#include "interp_wrapper.h"

#define COMPILE_TIME_ASSERT(pred)            \
  switch(0){case 0: case pred:;}

#define ASSERT_MIN_BITSIZE(type, size)       \
  COMPILE_TIME_ASSERT(sizeof(type) * CHAR_BIT >= size)

#define ASSERT_EXACT_BITSIZE(type, size)     \
  COMPILE_TIME_ASSERT(sizeof(type) * CHAR_BIT == size)

#define ASSERT_EQUAL_SIZE(type1, type2)      \
  COMPILE_TIME_ASSERT(sizeof(type1) == sizeof(type2))

#define ASSERT_EQUAL(val1, val2)      \
  COMPILE_TIME_ASSERT(val1 == val2)

#define ASSERT_GREATER_EQUAL(val1, val2)     \
  COMPILE_TIME_ASSERT(val1 >= val2)

void
compile_time_checks(void)
{
  ASSERT_MIN_BITSIZE(char, 8);
  
  ASSERT_EXACT_BITSIZE(unsigned int, 32); 
  ASSERT_EXACT_BITSIZE(unsigned long, 64); 
  ASSERT_EXACT_BITSIZE(size_t, 64);
  ASSERT_EXACT_BITSIZE(data_t, 64); 
  ASSERT_EQUAL_SIZE(data_t, void*); 
  ASSERT_EQUAL_SIZE(data_t, interpWalkFunc_t);
  ASSERT_EQUAL_SIZE(unsigned int, nodeEnum);
}
