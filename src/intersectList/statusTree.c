/**
 * \file statusTree.c
 *
 * An implementation of a status tree type.
 */

/*
 *  This code is a very heavily modified version of the macros in 
 *  the NetBSD source file tree.h. Here are the opening comments from 
 *  that file:
 *
 *
 *  $NetBSD: tree.h,v 1.9 2005/02/26 22:25:34 perry Exp $ 
 *  $OpenBSD: tree.h,v 1.7 2002/10/17 21:51:54 art Exp $
 *
 * Copyright 2002 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* From the <a href="http://en.wikipedia.org/wiki/Red-black_tree">
 * Wikipedia</a> entry:
 *
 * A red-black tree is a binary search tree where each node has 
 * a color attribute, the value of which is either red or black. 
 * In addition to the ordinary requirements imposed on binary search 
 * trees, we make the following additional requirements of any valid 
 * red-black tree:
 *
 *  1. A node is either red or black.\n
 *  2. The root is black.\n
 *  3. All leaves are black.\n
 *  4. Both children of every red node are black.\n
 *  5. All paths from any given node to its leaf nodes contain the 
 *     same number of black nodes.
 *
 * These constraints enforce a critical property of red-black trees: 
 * that the longest possible path from the root to a leaf is no more 
 * than twice as long as the shortest possible path. The result is that 
 * the tree is roughly balanced. Since operations such as inserting,
 * deleting, and finding values requires worst-case time proportional 
 * to the height of the tree, this theoretical upper bound on the 
 * height allows red-black trees to be efficient in the worst-case, 
 * unlike ordinary binary search trees. 
 *
 * To see why these properties guarantee this, it suffices to note
 * that no path can have two red nodes in a row, due to property 4. 
 * The shortest possible path has all black nodes, and the longest
 * possible path alternates between red and black nodes. Since all
 * maximal paths have the same number of black nodes, by property 5,
 * this shows that no path is more than twice as long as any other
 * path.
 *
 * In many presentations of tree data structures, it is possible for
 * a node to have only one child, and leaf nodes contain data. It is
 * possible to present red-black trees in this paradigm, but it
 * changes several of the properties and complicates the algorithms.
 * For this reason, we use "null leaves", which contain no data and
 * merely serve to indicate where the tree ends.  A consequence of
 * this is that all internal (non-leaf) nodes have two children,
 * although one or more of those children may be a null leaf. A 
 * threaded red-black tree uses the pointer space in the leaf nodes
 * to build an ordered linked list of internal nodes. An extra tag
 * is required for each node to distinguish between internal and 
 * thread nodes.
 *
 * <a href="http://en.wikipedia.org/wiki/Wikipedia:Text_of_the_GNU_Free_Documentation_License">GNU Free Documentation License</a>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "compare.h"
#include "point.h"
#include "segment.h"
#include "statusTree.h"

/**
 * Threaded red-black tree node colour. 
 *
 * Enumeration of threaded red-black tree node colour.
 */
typedef enum statusTreeColour_e 
  {
    statusTreeBlack = 0, statusTreeRed
  }
statusTreeColour_e;

/**
 * Threaded red-black tree node tag.
 *
 * Enumeration of threaded red-black tree node tag for a node left or right
 * pointer to a child or a leaf. A "leaf" is a pointer to the next or previous
 * node in the thread. A "child" has a "real" entry.
 */
typedef enum statusTreeTag_e 
  {
    statusTreeLeaf = 0, statusTreeChild
  }
statusTreeTag_e;

/**
 * \c statusTree_t internal node. 
 *
 * Internal representation of a statusTree_t node.
 */
struct status_t
{
  segment_t *segment;
  /**< Pointer to segment. */

  struct status_t *left;
  /**< Pointer to left-hand child of node. */

  struct status_t *right;
  /**< Pointer to right-hand child of node. */

  struct status_t *parent;
  /**< Pointer to parent of node. */

  statusTreeColour_e colour;
  /**< Colour of node. */

  statusTreeTag_e leftTag;
  /**< Tag of left-hand child of node. */

  statusTreeTag_e rightTag;
  /**< Tag of right-hand child of node. */
};

/**
 * \c statusTree_t type. Internal representation of a status tree.
 */
struct statusTree_t
{
  statusTreeAllocFunc_t alloc;
  /**< Memory allocator callback function. */

  statusTreeDeallocFunc_t dealloc;
  /**< Memory de-allocator callback function. */

  statusTreeDebugFunc_t debug;
  /**< Debugging message callback function. */

  point_t sweep;
  /**< Current sweep point defining segment order in the status tree. */

  status_t *root;
  /**< Root entry in the status tree. */

  status_t *current;
  /**< Last entry accessed in the tree. */

  size_t size;
  /**< Number of entries in the red-black tree. */
};

/**
 * Private helper function for status tree implementation.
 *
 * Compare two status points
 *
 * \param tree pointer to \c statusTree_t
 * \param s1 \c void pointer to \c segment_t
 * \param s2 \c void pointer to \c segment_t
 * \return \c compare_e value
 */
static
compare_e
statusTreeCompare(statusTree_t * const tree, 
                  segment_t * const s1, 
                  segment_t * const s2)
{
  point_t *sweep;

  /* Sanity check */
  if(s2 == NULL)
    {
      return compareError;
    }

  /* 
   * Update intersection. Assume tree contains the current sweep
   * point and s2 is an existing entry in the tree.
   *
   * If we are ordering by the sweep point then s1 need not exist. For
   * example, if we want to find the segments on either side of the
   * event point.
   */

  /* Locals */
  sweep = &(tree->sweep);
      
  /* Check for same segment by address */
  if (s1 == s2)
    {
      return compareEqual;
    }

  /* Find the order of s2 and sweep on this sweep line */
  else if (s1 == NULL)
    {
      return segmentOrderSegmentAndPoint(s2, sweep);
    }
  
  /* General segment to segment ordering at sweep */
  else  
    {
      return segmentOrderSegments(s1, s2, sweep);
    }
}

/**
 * Private helper function for status tree implementation.
 *
 * Colour nodes.
 *
 * \param black pointer to \c statusTreeNode_t to be coloured black
 * \param red pointer to \c status_t to be coloured red
 */
static
void
statusTreeSetBlackRed(status_t * const black, status_t * const red)
{
  black->colour = statusTreeBlack;
  red->colour = statusTreeRed;
}

/**
 * Private helper function for status tree implementation.
 *
 * Colour node black.
 *
 * \param node pointer to \c status_t to be coloured black
 */
static
void
statusTreeSetBlack(status_t * const node)
{
  node->colour = statusTreeBlack;
}

/**
 * Private helper function for status tree implementation.
 *
 * Colour node red.
 *
 * \param node pointer to \c status_t to be coloured red
 */
static
void
statusTreeSetRed(status_t * const node)
{
  node->colour = statusTreeRed;
}

/**
 * Private helper function for status tree implementation.
 *
 * Copy node colour.
 *
 * \param dest pointer to destination \c status_t for colour
 * \param src pointer to source \c status_t of colour
 */
static
void
statusTreeCopyColour(status_t * const dest, const status_t * const src)
{
  dest->colour = src->colour;
}

/**
 * Private helper function for status tree implementation.
 *
 * Is node black?
 *
 * \param node pointer to \c status_t to be tested
 * \return \e true if node is black
 */
static
bool
statusTreeIsBlack(const status_t * const node)
{
  if ((node != NULL) && (node->colour == statusTreeBlack))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for status tree implementation.
 *
 * Is node red?
 *
 * \param node pointer to \c status_t to be tested
 * \return \e true if node is red
 */
static
bool
statusTreeIsRed(const status_t * const node)
{
  if ((node != NULL) && (node->colour == statusTreeRed))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for status tree implementation.
 *
 * Does node have a left leaf?
 *
 * \param node pointer to \c status_t to be tested
 * \return \e true if node has a left leaf
 */
static
bool
statusTreeLeftIsLeaf(const status_t * const node)
{
  if ((node != NULL) && (node->leftTag == statusTreeLeaf))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for status tree implementation.
 *
 * Does node have a left child?
 *
 * \param node pointer to \c status_t to be tested
 * \return \e true if node has a left child
 */
static
bool
statusTreeLeftIsChild(const status_t * const node)
{
  if ((node != NULL) && (node->leftTag == statusTreeChild))
  {
    if (node->left == NULL)
      {
        /* Should not get here! A leaf of a child is not NULL. */
        fprintf(stderr, "%s:%d : node->left == NULL\n", __func__, __LINE__);
        exit(-1);
      }
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for status tree implementation.
 *
 * Does node have a right leaf?
 *
 * \param node pointer to \c status_t to be tested
 * \return \e true if node has a right leaf
 */
static
bool
statusTreeRightIsLeaf(const status_t * const node)
{
  if ((node != NULL) && (node->rightTag == statusTreeLeaf))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for status tree implementation.
 *
 * Does node have a right child?
 *
 * \param node pointer to \c status_t to be tested
 * \return \e true if node has a right child
 */
static
bool
statusTreeRightIsChild(const status_t * const node)
{
  if ((node != NULL) && (node->rightTag == statusTreeChild))
  {
    if (node->right == NULL)
      {
        /* Should not get here! A leaf of a child is not NULL. */
        fprintf(stderr, "%s:%d : node->right == NULL\n", __func__, __LINE__);
        exit(-1);
      }
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for status tree implementation.
 *
 * Does node have a parent?
 *
 * \param node pointer to \c status_t to be tested
 * \return \e true if node has a parent
 */
static
bool
statusTreeHasParent(const status_t * const node)
{
  if ((node != NULL) && (node->parent != NULL))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for status tree implementation.
 *
 * Is it a valid node?
 *
 * \param tree pointer to \c statusTree_t
 * \param node pointer to \c status_t to be tested
 * \return \e true if node is not \c NULL, \e max or \e min.
 */
static
bool
statusTreeIsValid(const statusTree_t * const tree, const status_t * const node)
{
  (void)tree;

  if (node != NULL)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for status tree implementation.
 *
 * Rotate node right maintaining order. Adjust the thread for the right-hand 
 * child of x.
 *
 \verbatim
        y            x  
       / \          / \ 
      x   c  -->   a   y
     / \              / \   
    a   b            b   c  
 \endverbatim
 *
 * \param tree pointer to \c statusTree_t
 * \param y pointer to \c status_t to be rotated right
 */
static
void
statusTreeRotateRight(statusTree_t * const tree, status_t * const y)
{
  status_t *x = y->left;

  /* x's right subtree, b, becomes y's left subtree */
  y->left = x->right;
  y->leftTag = x->rightTag;
  if (statusTreeLeftIsChild(y)) 
    {
      (x->right)->parent = y;
    }
  else
    {
      x->rightTag = statusTreeChild;
      y->leftTag = statusTreeLeaf;
      y->left = x;
    }

  /* x replaces y */
  x->parent = y->parent;
  if (statusTreeHasParent(x)) 
    {
      if (y == (y->parent)->left) 
        {
          (y->parent)->left = x;
        }
      else 
        {
          (y->parent)->right = x;
        }
    }
  else 
    {
      tree->root = x;
    }

  /* x becomes y's parent */
  x->right = y;
  y->parent = x;
}

/**
 * Private helper function for status tree implementation.
 *
 * Rotate node left maintaining order. Adjust the thread for the left-hand 
 * child of y.
 *
 \verbatim
      x               y   
     / \             / \  
    a   y   -->     x   c 
       / \         / \    
      b   c       a   b   
 \endverbatim
 * 
 * \param tree pointer to \c statusTree_t
 * \param x pointer to \c status_t to be rotated left
 */
static
void
statusTreeRotateLeft(statusTree_t * const tree, status_t * const x)
{
  status_t *y = x->right;

  /* y's left subtree, b, becomes x's right subtree */
  x->right = y->left;
  x->rightTag = y->leftTag;  
  if (statusTreeRightIsChild(x)) 
    {
      (y->left)->parent = x;
    }
  else
    {
      y->leftTag = statusTreeChild;
      x->rightTag = statusTreeLeaf;
      x->right = y;
    }

  /* y replaces x */
  y->parent = x->parent;
  if (statusTreeHasParent(y))
    {
      if (x == (x->parent)->left) 
        {
          (x->parent)->left = y;
        }
      else 
        {
          (x->parent)->right = y;
        }
    }
  else
    {
      tree->root = y;
    }

  /* y becomes x's parent */
  y->left = x;
  x->parent = y;
}

/**
 * Private helper function for status tree implementation.
 *
 * After insertion of a new node:\n
 *    - if parent node is black tree is valid\n
 *    - if parent node is red tree then fix the tree
 * 
 * \param tree pointer to \c statusTree_t
 * \param node pointer to \c status_t that was inserted
 */
static
void 
statusTreeInsertColour(statusTree_t * const tree, status_t *node) 
{
  status_t *parent, *gparent, *uncle;

  while (statusTreeHasParent(node) && statusTreeIsRed(node->parent))
    {
      /* parent is red so gparent isn't NULL since root is black */
      parent = node->parent;
      gparent = parent->parent;
      if (parent == gparent->left) 
        {
          uncle = gparent->right;
          if (statusTreeRightIsChild(gparent) && statusTreeIsRed(uncle)) 
            {
              /*
               *  If both the parent node and the uncle node are red, 
               *  then we can repaint them both black and repaint the 
               *  gparent node red (to maintain property 5). Now our 
               *  new red node has a black parent. Since any path through 
               *  the parent or uncle must pass through the gparent, 
               *  the number of black nodes on these paths has not 
               *  changed. However, the gparent may now violate 
               *  properties 2 or 4. To fix this, we repeat this 
               *  procedure on gparent.
               */
              statusTreeSetBlack(uncle);
              statusTreeSetBlackRed(parent, gparent);
              node = gparent;
              continue;
            }
          if (statusTreeRightIsChild(parent) && (parent->right == node))
            {
              /*
               *  The parent is red but the uncle is black; also, the 
               *  new node is the right child of its parent, and the 
               *  parent in turn is the left child of its parent. In 
               *  this case, we can perform a left rotation that 
               *  switches the roles of the new node and its parent; 
               *  then, we deal with the former parent node
               */
              statusTreeRotateLeft(tree, parent);
              uncle = parent;
              parent = node;
              node = uncle;
            }
          /*
           *  The parent is red but the uncle is black, the new node
           *  is the left child of its parent, and the parent is the
           *  left child of its parent. In this case, we perform a right
           *  rotation about the grandparent; the result is a tree where
           *  the former parent is now the parent of both the new node
           *  and the former grandparent. We know that the former
           *  grandparent is black, since the parent could not have been
           *  red otherwise. We then switch the colors of the former
           *  parent and grandparent nodes, and the resulting tree
           *  satisfies property 4. Property 5 also remains satisfied,
           *  since all paths that went through any of these three nodes
           *  went through the grandparent before, and now they all go
           *  through the former parent. In each case, this is the only
           *  black node of the three.
           */
          statusTreeSetBlackRed(parent, gparent); 
          statusTreeRotateRight(tree, gparent); 
        } 
      else 
        { 
          /* See comments for right-hand case */
          uncle = gparent->left; 
          if (statusTreeLeftIsChild(gparent) && statusTreeIsRed(uncle)) 
            { 
              statusTreeSetBlack(uncle);
              statusTreeSetBlackRed(parent, gparent); 
              node = gparent;
              continue; 
            } 
          if (statusTreeLeftIsChild(parent) && (parent->left == node))
            {
              statusTreeRotateRight(tree, parent); 
              uncle = parent; 
              parent = node; 
              node = uncle; 
            } 
          statusTreeSetBlackRed(parent, gparent);
          statusTreeRotateLeft(tree, gparent); 
        } 
    }


  /* 
   *  Root node is always painted black 
   */
  statusTreeSetBlack(tree->root);
}


/**
 * Private helper function for status tree implementation.
 *
 *  Fix the threaded red-black property after a normal tree deletion.
 *
 * \param tree pointer to \c statusTree_t
 * \param parent pointer to parent of \c status_t being deleted
 * \param node pointer to \c status_t replacing that being deleted
 */
static
void 
statusTreeRemoveColour(statusTree_t * const tree, 
                       status_t *parent, 
                       status_t *node) 
{
  status_t *sibling;

  while ((((node == NULL) || statusTreeIsBlack(node))) && 
         (node != tree->root))
    {
      if ((statusTreeLeftIsChild(parent) && (parent->left == node))
          ||
          (statusTreeLeftIsLeaf(parent) && (node == NULL))
          )
        {
          sibling = parent->right;
          if (statusTreeRightIsChild(parent) && statusTreeIsRed(sibling)) 
            {
              /*
               *  Right-hand sibling is red. Reverse the colors of
               *  node's parent and sibling, and then rotate left at
               *  node's parent, turning node's sibling into its
               *  grandparent. Although all paths still have the
               *  same number of black nodes, node now has a black
               *  sibling and a red parent.  (Its new sibling is
               *  black because it was once the child of the old red
               *  sibling. See the diagrams for left/right rotation.)
               */
              statusTreeSetBlackRed(sibling, parent);
              statusTreeRotateLeft(tree, parent);
              sibling = parent->right;
              if (statusTreeRightIsLeaf(parent))
                {
                  continue;
                }
            }

          if ((statusTreeLeftIsLeaf(sibling) || 
               statusTreeIsBlack(sibling->left)) 
              && 
              (statusTreeRightIsLeaf(sibling) || 
               statusTreeIsBlack(sibling->right)))
            {
              /*
               *  sibling and siblings children are black. In this
               *  case set sibling to red.
               */
              statusTreeSetRed(sibling);
              node = parent; 
              parent = node->parent;
              /*
               *  If parent was red then we'll break out of the loop
               *  and set it black. This doesn't affect the number
               *  of black nodes on paths not going through node,
               *  but it does add one to the number of black nodes
               *  on paths going through node, making up for the
               *  deleted black node on those paths. (We only get to
               *  this function if the deleted node was black.)
               */
            }
          else 
            {
              if (statusTreeRightIsLeaf(sibling) || 
                  statusTreeIsBlack(sibling->right))
                {
                  if (statusTreeLeftIsChild(sibling)) 
                    {
                      statusTreeSetBlack(sibling->left);
                    }

                  statusTreeSetRed(sibling);
                  statusTreeRotateRight(tree, sibling);
                  sibling = parent->right;
                }

              statusTreeCopyColour(sibling, parent);
              statusTreeSetBlack(parent);

              if (statusTreeRightIsChild(sibling))
                {
                  statusTreeSetBlack(sibling->right);
                }

              statusTreeRotateLeft(tree, parent);
              node = tree->root;
              break;
            }
        }
      else
        {
          /* See comments for right-hand sibling */
          sibling = parent->left;
          if (statusTreeLeftIsChild(parent) && statusTreeIsRed(sibling)) 
            {
              statusTreeSetBlackRed(sibling, parent);
              statusTreeRotateRight(tree, parent);
              sibling = parent->left;
              if (statusTreeLeftIsLeaf(parent))
                {
                  continue;
                }
            }

          if ((statusTreeLeftIsLeaf(sibling) || 
               statusTreeIsBlack(sibling->left))
              && 
              (statusTreeRightIsLeaf(sibling) || 
               statusTreeIsBlack(sibling->right)))
            {
              statusTreeSetRed(sibling);
              node = parent;
              parent = node->parent;
            }
          else 
            {
              if (statusTreeLeftIsLeaf(sibling) || 
                  statusTreeIsBlack(sibling->left))
                {
                  if (statusTreeRightIsChild(sibling))
                    {
                      statusTreeSetBlack(sibling->right);
                    }

                  statusTreeSetRed(sibling);
                  statusTreeRotateLeft(tree, sibling);
                  sibling = parent->left;
                }

              statusTreeCopyColour(sibling, parent);
              statusTreeSetBlack(parent);

              if (statusTreeLeftIsChild(sibling))
                {
                  statusTreeSetBlack(sibling->left);
                }

              statusTreeRotateRight(tree, parent);
              node = tree->root;
              break;
            }
        }
    }

  if (statusTreeIsValid(tree, node))
    {
      statusTreeSetBlack(node);
    }
}

/**
 * Private helper function for status tree implementation.
 *
 * Remove a node from the tree.
 *
 * \param tree pointer to \c statusTree_t
 * \param node pointer to \c status_t to be removed
 * \return pointer to \c status_t found. \c NULL if not found.
 */
static
status_t *
statusTreeRemoveNode(statusTree_t * const tree, status_t * const node) 
{
  status_t *child, *parent, *old, *succ, *pred;
  statusTreeColour_e colour;

  if ((old = node) == NULL)
    {
      return NULL;
    }

  /*
   *  In a normal binary search tree, when deleting a node with two
   *  non-leaf children, we find either the maximum element in its
   *  left subtree or the minimum element in its right subtree, and
   *  move its value into the node being deleted. We then delete the
   *  node we copied the value from, which must have less than two
   *  non-leaf children. Because merely copying a value does not
   *  violate any threaded red-black properties, this reduces the problem of
   *  deleting to the problem of deleting a node with at most one
   *  child. It doesn't matter whether this node is the node we
   *  originally wanted to delete or the node we copied the value
   *  from.
   *
   *  Delete the node from the tree then fix the colour property.
   *
   *  There are three cases for node deletion:
   *    - node has no children. Just delete the node
   *    - node to be deleted has only one child. Replace the node 
   *      with its child and make the parent of the deleted node
   *      the parent of the node = tree->root; child of the deleted node.
   *    - node to be deleted has two children. First, find the minimum 
   *      value of right subtree and delete it but keep its value. 
   *      Next, replace the value of the node to be deleted by the 
   *      minimum value whose node was deleted earlier. For example:
   *     
   *              5                      7
   *             / \                    / \
   *            /   \                  /   \
   *           /     \      ->        /     \
   *          3       9              3       9
   *         / \     / \            / \     / \
   *        2   4   7   10         2   4   8   10
   *                 \
   *                  8
   *
   *  Usually, we try to use the predecessor and successor nodes 
   *  equally to avoid unbalancing the tree. Here the threaded red-black 
   *  property maintains balance.
   */
  if (statusTreeLeftIsLeaf(node) && statusTreeRightIsLeaf(node))
    {
      parent = node->parent;
      colour = node->colour;
      child = NULL;
      if (statusTreeHasParent(node))
        {
          if (node == parent->left)
            {
              parent->left = node->left;
              parent->leftTag = statusTreeLeaf;
            }
          else
            {
              parent->right = node->right;
              parent->rightTag = statusTreeLeaf;
            }
        }
      else
        {
          tree->root = NULL;
        }
    }
  else if (statusTreeLeftIsLeaf(node) && statusTreeRightIsChild(node))
    {
      parent = node->parent;
      colour = node->colour;

      /* Child is successor of node in left subtree of node->right */
      child = node->right;
      if (statusTreeLeftIsChild(node->right))
        {
          while (statusTreeLeftIsChild(child))
            {
              child = child->left;
            }
          if (statusTreeRightIsChild(child))
            {
              child->parent->left = child->right;
              child->right->parent = child->parent;
            }
          else
            {
              child->parent->leftTag = statusTreeLeaf;
            }
          child->right = node->right;
          child->rightTag = statusTreeChild;
        }

      /* Install child. Recall left is a leaf! */
      child->left = node->left;
      child->leftTag = node->leftTag;
      child->parent = node->parent;
      if (statusTreeHasParent(node)) 
        {
          if (node == parent->left) 
            {
              parent->left = child;
            }
          else 
            {
              parent->right = child;
            }
        }
      else 
        {
          tree->root = child;
        }
    }
  else if (statusTreeLeftIsChild(node) && statusTreeRightIsLeaf(node))
    {
      parent = node->parent;
      colour = node->colour;

      /* Child is successor of node in right subtree of node->left */
      child = node->left;
      if (statusTreeRightIsChild(node->left))
        {
          while (statusTreeRightIsChild(child))
            {
              child = child->right;
            }
          if (statusTreeLeftIsChild(child))
            {
              child->parent->right = child->left;
              child->left->parent = child->parent;
            }
          else
            {
              child->parent->rightTag = statusTreeLeaf;
            }
          child->left = node->left;
          child->leftTag = statusTreeChild;
        }

      /* Install child. Recall right is a leaf! */
      child->right = node->right;
      child->rightTag = node->rightTag;
      child->parent = node->parent;
      if (statusTreeHasParent(node)) 
        {
          if (node == parent->left) 
            {
              parent->left = child;
            }
          else 
            {
              parent->right = child;
            }
        }
      else 
        {
          tree->root = child;
        }
    }
  else 
    {
      /* 
       *  Node has two children. Arbitrarily replace with right child
       */

      /* First the case for node->right being node's successor */
      if (statusTreeLeftIsLeaf(node->right))
        {
          /* Find child (whose colour may need to be fixed) */
          succ = node->right;
          colour = succ->colour;
          parent = succ;
          if (statusTreeRightIsChild(succ))
            {
              child = succ->right;
            }
          else
            {
              child = NULL;
            }
          /* succ keeps right pointer */
        }
      else
        {
          /* Find successor of node in left subtree of node->right */
          succ = node->right;
          while (statusTreeLeftIsChild(succ))
            {
              succ = succ->left;
            }
          if (succ == NULL)
            {
              /* Should not get here! A leaf of a child is not NULL. */
              fprintf(stderr, "%s:%d : succ == NULL\n", __func__, __LINE__);
              exit(-1);
            }

          /* child is the node of the tree whose colour may be altered. */
          parent = succ->parent;
          colour = succ->colour;
          if (statusTreeRightIsChild(succ))
            {
              succ->parent->left = succ->right;
              succ->right->parent = succ->parent;
              child = succ->right;
            }
          else
            {
              succ->parent->leftTag = statusTreeLeaf;
              child = NULL;
            }

          /* Replace succ right pointer */
          succ->right = node->right;
          succ->rightTag = statusTreeChild;

          /* Fix node right parent */
          node->right->parent = succ;
        }

      /* 
       *  Install succ. Recall left is a child! 
       */
      succ->left = node->left;
      succ->leftTag = node->leftTag;
      statusTreeCopyColour(succ, node);
      succ->parent = node->parent;

      /* 
       *  Fix parent link 
       */
      if (statusTreeHasParent(node)) 
        {
          if (node == node->parent->left) 
            {
              node->parent->left = succ;
            }
          else 
            {
              node->parent->right = succ;
            }
        }
      else 
        {
          tree->root = succ;
        }
  
      /* 
       *  Fix the left thread 
       */
      pred = node->left;
      pred->parent = succ;
      while (statusTreeRightIsChild(pred))
        {
          pred = pred->right;
        }
      pred->right = succ;
    }
  
  /*
   *  Now fix the colour. If we are deleting a red node, we 
   *  can simply replace it with its child, which must be black.
   *  If deleting a black node we need to fix the tree.
   */
  if (colour == statusTreeBlack)
    {
      statusTreeRemoveColour(tree, parent, child);
    }

  /* Done */
  tree->size = tree->size-1;
  return node;
}

/**
 * Private helper function for status tree implementation.
 *
 *  Find left-most entry.
 *
 * \param tree pointer to \c statusTree_t
 * \return pointer to minimum \c status_t found. 
 * \c NULL if not found.
 */
static
status_t *
statusTreeFindMin(statusTree_t * const tree) 
{
  status_t *node;

  if (tree == NULL)
    {
      return NULL;
    }

  node = tree->root;
  while (statusTreeLeftIsChild(node))
    {
      node = node->left;
    }

  return node;
}

/**
 * Private helper function for status tree implementation.
 *
 *  Compare a segment in a status_t with a sweep point
 *
 * \param status pointer to \c status_t
 * \param sweep pointer to sweep point
 * \return \c compare_e value 
 */
static
compare_e 
statusTreeOrderStatusAndPoint(status_t * const status, point_t * const sweep)
{
  if ((status == NULL) || (sweep == NULL))
    {
      return compareError;
    }

  return segmentOrderSegmentAndPoint(status->segment, sweep);
}

statusTree_t *
statusTreeCreate(const statusTreeAllocFunc_t alloc, 
                 const statusTreeDeallocFunc_t dealloc, 
                 const statusTreeDebugFunc_t debug)
{
  statusTree_t *t;

  if (debug == NULL)
    {
      return NULL;
    }
  if (alloc == NULL)
    {
      debug(__func__, __LINE__, " Invalid alloc() function!");
      return NULL;
    }
  if (dealloc == NULL)
    {
      debug(__func__, __LINE__, " Invalid dealloc() function!");
      return NULL;
    }

  /* Allocate space for status tree */
  t = (statusTree_t *)alloc(sizeof(statusTree_t));
  if (t == NULL)
    {
      debug(__func__, __LINE__,
            "Can't allocate %d for statusTree_t",
            sizeof(statusTree_t));
      return NULL;
    }

  /* Initialise t*/
  t->alloc = alloc;
  t->dealloc = dealloc;
  t->debug = debug;
  pointSetCoords(&(t->sweep), 0, 0);
  pointSetTolerance(&(t->sweep), 0);
  t->root = NULL;
  t->current = NULL;
  t->size = 0;

  return t;
}

status_t *
statusTreeInsert(statusTree_t * const tree,
                 segment_t * const segment, 
                 point_t * const sweep)
{
  status_t *node, *parent;
  compare_e comp;

  if (tree == NULL)
    {
      return NULL;
    }
  if (segment == NULL)
    {
      tree->debug(__func__, __LINE__, " Invalid segment==NULL!");
      return NULL;
    }
  if (sweep == NULL)
    {
      tree->debug(__func__, __LINE__, " Invalid sweep==NULL!");
      return NULL;
    }

  /* Search for an existing segment */
  pointCopy(&(tree->sweep), sweep);
  node = tree->root;
  parent = NULL;
  comp = compareEqual;
  while (statusTreeIsValid(tree, node))
    {
      parent = node;
      comp = statusTreeCompare(tree, segment, parent->segment);
      if (comp == compareLesser) 
        {
          if (statusTreeLeftIsChild(node))
            {
              node = node->left;
            }
          else
            {
              break;
            }
        }
      else if (comp == compareGreater) 
        {
          if (statusTreeRightIsChild(node))
            {
              node = node->right;
            }
          else
            {
              break;
            }
        }
      else if (comp == compareEqual)
        {
          node->segment = segment;
          tree->current = node;
          return node;
        }
      else
        {
          (tree->debug)(__func__, __LINE__, "Illegal compare result!");
          return NULL;
        }
    }

  /* Create a node for the new entry */
  node = (status_t *)(tree->alloc)(sizeof(status_t));
  if (node == NULL)
    {
      (tree->debug)(__func__, __LINE__, "alloc failed!");
      return NULL;
    }

  /* 
   *  Inserting a node is much easier than removing 
   *  one because left and right are leaves!
   */
  node->segment = segment;
  node->parent = parent;
  node->leftTag = statusTreeLeaf;
  node->rightTag = statusTreeLeaf;
  statusTreeSetRed(node);
  if (parent != NULL) 
    {
      if (comp == compareLesser) 
        {
          /* parent->left must have been a leaf! */
          node->right = parent; 
          node->left = parent->left;
          parent->left = node;
          parent->leftTag = statusTreeChild;
        }
      else 
        {
          /* parent->right must have been a leaf! */
          node->left = parent;
          node->right = parent->right;
          parent->right = node;
          parent->rightTag = statusTreeChild;
        }
    }
  else
    {
      tree->root = node;
      node->left = NULL;
      node->leftTag = statusTreeLeaf;
      node->right = NULL;
      node->rightTag = statusTreeLeaf;
    }

  /* Fix the red-black property */
  statusTreeInsertColour(tree, node);

  /* Done */
  tree->current = node;
  tree->size = tree->size+1;

  return node;
}

void 
statusTreeRemove(statusTree_t * const tree, status_t * const status)
{
  if (statusTreeRemoveNode(tree, status) == NULL)
    {
      return;
    }

  /* Deallocate the deleted node and entry */
  (tree->dealloc)(status);
}


void 
statusTreeClear(statusTree_t * const tree)
{
  status_t *node;
  status_t *next;

  /* Sanity check */
  if (tree == NULL)
    {
      return;
    }

  /* Delete all nodes */
  node = statusTreeFindMin(tree);
  while (statusTreeIsValid(tree, node))
    {
      next = statusTreeGetNext(tree, node); 
      statusTreeRemoveNode(tree, node);
      (tree->dealloc)(node);
      node = next;
    }

  return;
}

void 
statusTreeDestroy(statusTree_t *tree)
{
  statusTreeClear(tree);
  (tree->dealloc)(tree);

  return;
}

bool
statusTreeIsEmpty(const statusTree_t * const tree)
{
  if (tree == NULL)
    {
      return false;
    }

  if (tree->size == 0)
    {
      return true;
    }

  return false;
}

segment_t *
statusTreeGetSegment(statusTree_t * const tree, status_t * const S)
{
  if ((tree == NULL) || (S == NULL))
    {
      return NULL;
    }

  return S->segment;
}

status_t *
statusTreeGetNext(statusTree_t * const tree, status_t * const status)
{
  status_t *next;

  if ((tree == NULL) || (status == NULL))
    {
      return NULL;
    }

  if (statusTreeRightIsChild(status)) 
    {
      /* Find left-most node on right sub-tree */
      next = status->right;
      while (statusTreeLeftIsChild(next))
        {
          next = next->left;
        }
    }
  else 
    {
      /* Follow thread */
      next = status->right;
    }

  return next;
}

status_t *
statusTreeGetPrevious(statusTree_t * const tree, status_t * const status)
{
  status_t *prev;

  if ((tree == NULL) || (status == NULL))
    {
      return NULL;
    }

  if (statusTreeLeftIsChild(status))
    {
      /* Find right-most status on left sub-tree */
      prev = status->left;
      while (statusTreeRightIsChild(prev))
        {
          prev = prev->right;
        }
    }
  else 
    {
      /* Follow thread */
      prev = status->left;
    }

  return prev;
}

status_t *
statusTreeGetUpper(statusTree_t * const tree, point_t * const sweep)
{
  compare_e comp;
  status_t *node;
  status_t *upper;

  if ((tree == NULL) || (sweep == NULL))
    {
      return NULL;
    }

  pointCopy(&(tree->sweep), sweep);
  node  = tree->root;
  upper = node;
  while (statusTreeIsValid(tree, node))

    {
      comp = statusTreeCompare(tree, NULL, node->segment);
      if (comp == compareLesser) 
        {
          upper = node->left;
          if (statusTreeLeftIsLeaf(node))
            {
              upper = node;
              break;
            }
        }
      else if (comp == compareGreater)
        {
          upper = node->right;
          if (statusTreeRightIsLeaf(node))
            {
              upper = statusTreeGetNext(tree, node);
              break;
            }
        }
      else if (comp == compareEqual)
        {
          upper = node;
          break;
        }
      else
        {
          (tree->debug)(__func__, __LINE__, "Illegal compare result!");
          return NULL;
        }

      node = upper;
    }

  if (upper == NULL)
    {
      return NULL;
    }
  else
    {
      return upper;
    }
}

status_t *
statusTreeGetLower(statusTree_t * const tree, point_t * const sweep)
{
  status_t *node;
  status_t *lower;
  compare_e comp;

  if ((tree == NULL) || (sweep == NULL))
    {
      return NULL;
    }

  pointCopy(&(tree->sweep), sweep);
  node = tree->root;
  lower = node;
  while (statusTreeIsValid(tree, node))
    {
      comp = statusTreeCompare(tree, NULL, node->segment);
      if (comp == compareLesser) 
        {
          lower = node->left;
          if (statusTreeLeftIsLeaf(node))
            {
              lower =  statusTreeGetPrevious(tree, node);
              break;
            }
        }
      else if (comp == compareGreater)
        {
          lower = node->right;
          if (statusTreeRightIsLeaf(node))
            {
              lower = node;
              break;
            }
        }
      else if (comp == compareEqual)
        {
          lower = node;
          break;
        }
      else
        {
          (tree->debug)(__func__, __LINE__, "Illegal compare result!");
          return NULL;
        }

      node = lower;
    }

  if (lower == NULL)
    {
      return NULL;
    }
  else
    {
      return lower;
    }
}

status_t *
statusTreeGetLeftMost(statusTree_t * const tree, point_t * const sweep)
{
  status_t *S, *Sp;
  
  Sp = NULL;
  if (((S = statusTreeGetLower(tree, sweep)) != NULL) &&
      (statusTreeOrderStatusAndPoint(S, sweep) == compareEqual))
    {
      Sp = S;
      while (((S = statusTreeGetPrevious(tree, S)) != NULL) &&
             (statusTreeOrderStatusAndPoint(S, sweep) == compareEqual))
        {
          Sp = S;
        }
    }

  return Sp;
}

status_t *
statusTreeGetRightMost(statusTree_t * const tree, point_t * const sweep)
{
  status_t *S, *Spp;
  
  Spp = NULL;
  if (((S = statusTreeGetUpper(tree, sweep)) != NULL) &&
      (statusTreeOrderStatusAndPoint(S, sweep) == compareEqual))
    {
      Spp = S;
      while (((S = statusTreeGetNext(tree, S)) != NULL) &&
             (statusTreeOrderStatusAndPoint(S, sweep) == compareEqual))
        {
          Spp = S;
        }
    }

  return Spp;
}

void
statusTreeShow(statusTree_t * const tree, point_t * const sweep)
{
  status_t *S;
  segment_t *s;

  if ((tree == NULL) || (sweep == NULL))
    {
      return;
    }

  (tree->debug)(NULL, 0,"%d segments", tree->size);
  pointCopy(&(tree->sweep), sweep);
  S = statusTreeFindMin(tree);
  s = statusTreeGetSegment(tree, S);
  while (s != NULL)
    {
      point_t *a, *b, i;
      coord_t ax, ay, bx, by, ix, iy;
  
      if (segmentGetPoints(s, &a, &b) == NULL)
        {
          (tree->debug)(__func__, __LINE__,
                        "segmentGetPoints() failed!"); 
          return;
        }

      if (pointGetCoords(a, &ax, &ay) == NULL)
        {
          (tree->debug)(__func__, __LINE__,
                        "pointGetCoords() failed!"); 
          return;
        }

      if (pointGetCoords(b, &bx, &by) == NULL)
        {
          (tree->debug)(__func__, __LINE__,
                        "pointGetCoords() failed!"); 
          return;
        }

      (tree->debug)(NULL, 0,
                    "  segment (%g,%g) (%g,%g)",
                    ax, ay, bx, by);

      if(pointSegmentSweepIntersection(a, b, &(tree->sweep), &i) 
         == compareEqual)
        {
          pointGetCoords(&i, &ix, &iy);
          (tree->debug)(NULL, 0, "  sweep intersection at (%g,%g)", ix, iy);
        }
      else
        {
          (tree->debug)(NULL, 0, "No sweep intersection!");
        }
            
      S = statusTreeGetNext(tree, S);
      s = statusTreeGetSegment(tree, S);
    }

  (tree->debug)(NULL, 0, "");

  return;
}

bool
statusTreeCheck(statusTree_t *tree)
{
  coord_t sweepx, sweepy;
  status_t *S, *nextS;

  if (tree == NULL)
    {
      return false;
    }

  if (pointGetCoords(&(tree->sweep), &sweepx, &sweepy) == NULL)
    {
      (tree->debug)(__func__, __LINE__, 
                    "pointGetCoords() failed for sweep!"); 
      return false;
    }

  S = statusTreeFindMin(tree);
  while (S != NULL)
    {
      segment_t *s, *nexts;
      point_t *a, *b, *nexta, *nextb, i, nexti;
      coord_t ix, iy, nextix, nextiy;
      coord_t ax, ay, nextax, nextay, bx, by, nextbx, nextby;

      /* Current segment intersection */
      s = S->segment;
      if (segmentGetPoints(s, &a, &b) == NULL)
        {
          (tree->debug)(__func__, __LINE__, 
                        "segmentGetPoints() failed!"); 
          return false;
        }

      if (pointGetCoords(a, &ax, &ay) == NULL)
        {
          (tree->debug)(__func__, __LINE__, 
                        "pointGetCoords() failed!"); 
          return false;
        }

      if (pointGetCoords(b, &bx, &by) == NULL)
        {
          (tree->debug)(__func__, __LINE__, 
                        "pointGetCoords() failed!"); 
          return false;
        }

      if (pointIsOutsideSweep(a, b, &(tree->sweep)) == true)
        {
          (tree->debug)(__func__, __LINE__,
                        "pointIsOutsideSweep() failed at sweep (%g,%g)",
                        sweepx, sweepy);
          (tree->debug)(__func__, __LINE__,
                        "segment (%g,%g) (%g,%g)",
                        ax, ay, bx, by);
          return false;
        }

      if(pointSegmentSweepIntersection(a, b, &(tree->sweep), &i) 
         != compareEqual)
        {
          (tree->debug)(__func__, __LINE__,
                        "pointSegmentSweepIntersection() failed at sweep (%g,%g)",
                        sweepx, sweepy);
          (tree->debug)(__func__, __LINE__,
                        "segment (%g,%g) (%g,%g)",
                        ax, ay, bx, by);
          return false;
        }

      /* Next segment intersection */
      if ((nextS = statusTreeGetNext(tree, S)) == NULL)
        {
          break;
        }
      nexts = nextS->segment;
      segmentGetPoints(nexts, &nexta, &nextb);      
      if (pointGetCoords(nexta, &nextax, &nextay) == NULL)
        {
          (tree->debug)(__func__, __LINE__, 
                        "pointGetCoords() failed!"); 
          return false;
        }

      if (pointGetCoords(nextb, &nextbx, &nextby) == NULL)
        {
          (tree->debug)(__func__, __LINE__, 
                        "pointGetCoords() failed!"); 
          return false;
        }

      if (pointIsOutsideSweep(nexta, nextb, &(tree->sweep)) == true)
        {
          (tree->debug)(__func__, __LINE__,
                        "pointIsOutsideSweep() failed at sweep (%g,%g)",
                        sweepx, sweepy);
          (tree->debug)(__func__, __LINE__,
                        "segment (%g,%g) (%g,%g); next segment (%g,%g) (%g,%g)",
                        ax, ay, bx, by, nextax, nextay, nextbx, nextby);
          return false;
        }

      if (pointSegmentSweepIntersection(a, b, &(tree->sweep), &nexti) 
          != compareEqual)
        {
          (tree->debug)(__func__, __LINE__,
                        "pointSegmentSweepIntersection() failed at sweep (%g,%g)",
                        sweepx, sweepy);
          (tree->debug)(__func__, __LINE__,
                        "segment (%g,%g) (%g,%g); next segment (%g,%g) (%g,%g)",
                        ax, ay, bx, by, nextax, nextay, nextbx, nextby);
          return false;
        }

      /* Check order in status tree */
      if (pointCompare(&i, &nexti) != compareEqual)
        {
          (tree->debug)(__func__, __LINE__,
                        "pointCompare() failed at sweep (%g,%g)",
                        sweepx, sweepy);

          if (pointGetCoords(&i, &ix, &iy) == NULL)
            {
              (tree->debug)(__func__, __LINE__, 
                            "pointGetCoords() failed for intersect!"); 
              return false;
            }

          (tree->debug)(__func__, __LINE__,
                        "segment (%g,%g) (%g,%g); next segment (%g,%g) (%g,%g)",
                        ax, ay, bx, by, nextax, nextay, nextbx, nextby);

          if (pointGetCoords(&nexti, &nextix, &nextiy) == NULL)
            {
              (tree->debug)(__func__, __LINE__, 
                            "pointGetCoords() failed for next intersect!"); 
              return false;
            }

          (tree->debug)(__func__, __LINE__,
                        "intersect (%g,%g); next intersect (%g,%g)",
                        ix, iy, nextix, nextiy);

          return false;
        }

      /* Loop */
      S = nextS;
    }

  return true;
}

