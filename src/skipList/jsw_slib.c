/**
 * \file jsw_slib.c
 *
 * Implementation of a skip list written by Julienne Walker.
 *
 * Original comments follow:
 *
 * Classic skip list library
 *
 * Created (Julienne Walker): April 11, 2004
 *
 * Updated (Julienne Walker): August 19, 2005
 */

#include "jsw_rand.h"
#include "jsw_slib.h"

#ifdef __cplusplus

#include <climits>
#include <cstdlib>

using std::malloc;
using std::free;
using std::size_t;

#else

#include <limits.h>
#include <stdlib.h>

#endif

/** 
 * Private \e jsw_node_t structure used by the \e jsw_skip_t skip list. 
 */
typedef struct jsw_node {
  void             *item;
  /**< Data item with combined key/value */

  size_t            height; 
  /**< Column height of this node */

  struct jsw_node **next;   
  /**< Dynamic array of next links */
} jsw_node_t;

/** 
 * \e Opaque definition of a \e jsw_skip_t structure. 
 */
struct jsw_skip {
  jsw_node_t *head; 
  /**< Full height header node */

  jsw_node_t **fix;
  /**< Update array */

  jsw_node_t *curl; 
  /**< Current link for traversal */

  size_t maxh; 
  /**< Tallest possible column */

  size_t curh; 
  /**< Tallest available column */

  size_t size; 
  /**< Number of items at level 0 */

  cmp_f cmp;  
  /**< User defined item compare function */

  dup_f dup;  
  /**< User defined item copy function */

  rel_f rel;  
  /**< User defined delete function */

  void *priv; 
  /**< User private data passed to the user defined functions */
};

/** 
 * Private function to generate a weighted random 
 * level with probability 1/2.
 *
 * Weighted random level with probability 1/2.
 * (For better distribution, modify with 1/3)
 * Implements a tuned bit stream algorithm.
 * \param max maximum height of the skip list
 * \return a random height less than \e max
 */
static size_t rlevel ( size_t max )
{
  static size_t bits = 0;
  static size_t reset = 0;
  size_t h, found = 0;

  for ( h = 0; !found; h++ ) {
    if ( reset == 0 ) {
      bits = jsw_rand();
      reset = sizeof ( size_t ) * CHAR_BIT - 1;
    }

    /*
      For 1/3 change to:

      found = bits % 3;
      bits = bits / 3;
    */
    found = bits & 1;
    bits = bits >> 1;
    --reset;
  }

  if ( h >= max )
    h = max - 1;

  return h;
}

/** 
 * Private function to create a new node in the \e jsw_skip_t skip list.
 *
 * Create a new node in the \e jsw_skip_t skip list at the given
 * height. This function does not make a copy of the item.
 * \param item \e void pointer to memory to be duplicated
 * \param height level in the skip list
 * \return pointer to an instance of \e jsw_node_t
 */
static jsw_node_t *new_node ( void *item, size_t height )
{
  jsw_node_t *node = (jsw_node_t *)malloc ( sizeof *node );
  size_t i;

  if ( node == NULL )
    return NULL;

  node->next = (jsw_node_t **)malloc ( height * sizeof *node->next );

  if ( node->next == NULL ) {
    free ( node );
    return NULL;
  }

  node->item = item;
  node->height = height;

  for ( i = 0; i < height; i++ )
    node->next[i] = NULL;

  return node;
}

/** 
 * Delete a node in the \e jsw_skip_t skip list
 *
 * Delete a node in the \e jsw_skip_t skip list. This function
 * does not release the items memory.
 * \param node pointer to the node memory to be deleted
 */
static void delete_node ( jsw_node_t *node )
{
  free ( node->next );
  free ( node );
}

/** 
 * Private function to locate an existing item in the \e jsw_skip_t skip list
 *
 * Locate an existing item in the \e jsw_skip_t skip list or the 
 * position before which it would be inserted.
 * \param skip pointer to a \e jsw_skip_t instance
 * \param item \e void pointer to the key/value to be found
 * \return pointer to the node containing the item or the node before
 * which it would be inserted.
 */
static jsw_node_t *locate ( jsw_skip_t * const skip, const void * const item )
{
  jsw_node_t *p = skip->head;
  size_t i;

  for ( i = skip->curh; i < (size_t)-1; i-- ) {
    while ( p->next[i] != NULL ) {
      if ( skip->cmp ( item, p->next[i]->item, skip->priv ) <= 0 )
        break;

      p = p->next[i];
    }

    skip->fix[i] = p;
  }

  return p;
}

jsw_skip_t *jsw_snew ( size_t max, 
                       const cmp_f cmp, 
                       const dup_f dup, 
                       const rel_f rel, 
                       void * const priv )
{
  jsw_skip_t *skip = (jsw_skip_t *)malloc ( sizeof *skip );

  if ( skip == NULL )
    return NULL;

  skip->head = new_node ( NULL, ++max );

  if ( skip->head == NULL ) {
    free ( skip );
    return NULL;
  }

  skip->fix = (jsw_node_t **)malloc ( max * sizeof *skip->fix );

  if ( skip->fix == NULL ) {
    delete_node ( skip->head );
    free ( skip );
    return NULL;
  }

  skip->curl = NULL;
  skip->maxh = max;
  skip->curh = 0;
  skip->size = 0;
  skip->cmp = cmp;
  skip->dup = dup;
  skip->rel = rel;
  skip->priv = priv;

  jsw_seed ( jsw_time_seed() );

  return skip;
}

void jsw_sdelete ( jsw_skip_t * const skip )
{
  jsw_node_t *it = skip->head->next[0];
  jsw_node_t *save;

  while ( it != NULL ) {
    save = it->next[0];
    skip->rel ( it->item, skip->priv );
    delete_node ( it );
    it = save;
  }

  delete_node ( skip->head );
  free ( skip->fix );
  free ( skip );
}

void *jsw_sfind ( jsw_skip_t * const skip, const void * const item )
{
  jsw_node_t *p = locate ( skip, item )->next[0];

  if ( p != NULL && skip->cmp ( item, p->item, skip->priv ) == 0 )
    return p->item;

  return NULL;
}

int jsw_sinsert ( jsw_skip_t * const skip, void * const item )
{
  void *p = locate ( skip, item )->item;

  if ( p != NULL &&
       skip->cmp ( item, p, skip->priv ) == 0 ) {
    return 0;
  }

  /* Try to allocate before making changes */
  size_t h = rlevel ( skip->maxh );
  void *dup = skip->dup ( item, skip->priv );
  jsw_node_t *it;

  if ( dup == NULL )
    return 0;

  it = new_node ( dup, h );

  if ( it == NULL ) {
    skip->rel ( dup, skip->priv );
    return 0;
  }

  /* Raise height if necessary */
  if ( h > skip->curh ) {
    h = ++skip->curh;
    skip->fix[h] = skip->head;
  }

  /* Build skip links */
  {
    int it_assigned = 0;

    while ( --h < (size_t)-1 ) {
      it->next[h] = skip->fix[h]->next[h];
      skip->fix[h]->next[h] = it;
      it_assigned = 1;
    }

    /* Node not used ?? (Memory leak noted by scan-build.) */
    if ( it_assigned == 1 ) {
      ++skip->size;
    } else {
      delete_node( it );
    }
  }

  return 1;
}

int jsw_serase ( jsw_skip_t * const skip, void *item )
{
  jsw_node_t *p = locate ( skip, item )->next[0];

  if ( p == NULL || skip->cmp ( item, p->item, skip->priv ) != 0 )
    return 0;
  else {
    size_t i;

    /* Erase column */
    for ( i = 0; i < skip->curh; i++ ) {
      if ( skip->fix[i]->next[i] != p )
        break;

      skip->fix[i]->next[i] = p->next[i];
    }

    skip->rel ( p->item, skip->priv );
    delete_node ( p );

    /* Lower height if necessary */
    while ( skip->curh > 0 ) {
      if ( skip->head->next[skip->curh - 1] != NULL )
        break;

      --skip->curh;
    }
  }

  /* Erasure invalidates traversal markers */
  jsw_sreset ( skip );

  --skip->size;

  return 1;
}

size_t jsw_ssize ( const jsw_skip_t * const skip )
{
  return skip->size;
}

void jsw_sreset ( jsw_skip_t * const skip )
{
  skip->curl = skip->head->next[0];
}

void *jsw_sitem ( const jsw_skip_t * const skip )
{
  return skip->curl == NULL ? NULL : skip->curl->item;
}

int jsw_snext ( jsw_skip_t * const skip )
{
  return ( skip->curl = skip->curl->next[0] ) != NULL;
}
