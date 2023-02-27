/**
 * \file compare.h
 */

#if !defined(COMPARE_H)
#define COMPARE_H

#ifdef __cplusplus
extern "C" {
#endif

/** An enumerated type for the results of comparisons. */
typedef enum compare_e 
{ 
  compareError=10,
  /**< Error */

  compareLesser=20,
  /**< Lower */

  compareEqual=30, 
  /**< Equal */

  compareGreater=40, 
  /**< Greater */
}
compare_e;

#ifdef __cplusplus
}
#endif

#endif
