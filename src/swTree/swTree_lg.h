/**
 * \file swTree_lg.h
 *
 * See: "Tree Rebalancing in Optimal Time and Space", Q. F. Stout and
 * B. L. Warren, Communications of the ACM, September 1986, Volume 29,
 * Number 9, pp. 902-908
 */

#if !defined(STOUT_WARREN_TREE_LG_H)
#define STOUT_WARREN_TREE_LG_H

#ifdef __cplusplus
extern "C" {
  using std::size_t;
#else
#include <stdlib.h>
#endif

  /**
   * Helper function for Stout/Warren tree implementation.
   *
   * Find floor(lg(size+1)) where lg means log-base-2
   *
   * \param size number of elements of the tree
   * \return floor(lg(size+1))
   */
  size_t swTreeFloorLg(const size_t size);

  /**
   * Helper function for Stout/Warren tree implementation.
   *
   * Find 2^floor(lg(size+1)) where lg means log-base-2
   *
   * \param size number of elements of the tree
   * \return power-of-2
   */
  size_t swTreeTwoFloorLg(const size_t size);

  /**
   * Helper function for Stout/Warren tree implementation.
   *
   * Find 2^[ceil(lg(size+1))-1] where lg means log-base-2
   *
   * \param size number of elements of the tree
   * \return power-of-2
   */
  size_t swTreeTwoCeilLg(const size_t size);

#ifdef __cplusplus
}
#endif

#endif
