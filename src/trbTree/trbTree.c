/**
 * \file trbTree.c
 *
 * An implementation of a threaded red-black tree.
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

#include "trbTree.h"
#include "trbTree_private.h"

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Colour nodes.
 *
 * \param black pointer to \c trbTreeNode_t to be coloured black
 * \param red pointer to \c trbTreeNode_t to be coloured red
 */
static
void
trbTreeSetBlackRed(trbTreeNode_t * const black, trbTreeNode_t * const red)
{
  black->colour = trbTreeBlack;
  red->colour = trbTreeRed;
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Colour node black.
 *
 * \param node pointer to \c trbTreeNode_t to be coloured black
 */
static
void
trbTreeSetBlack(trbTreeNode_t * const node)
{
  node->colour = trbTreeBlack;
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Colour node red.
 *
 * \param node pointer to \c trbTreeNode_t to be coloured red
 */
static
void
trbTreeSetRed(trbTreeNode_t * const node)
{
  node->colour = trbTreeRed;
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Copy node colour.
 *
 * \param dest pointer to destination \c trbTreeNode_t for colour
 * \param src pointer to source \c trbTreeNode_t of colour
 */
static
void
trbTreeCopyColour(trbTreeNode_t * const dest, trbTreeNode_t * const src)
{
  dest->colour = src->colour;
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Is node black?
 *
 * \param node pointer to \c trbTreeNode_t to be tested
 * \return \e true if node is black
 */
static
bool
trbTreeIsBlack(const trbTreeNode_t * const node)
{
  if ((node != NULL) && (node->colour == trbTreeBlack))
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Is node red?
 *
 * \param node pointer to \c trbTreeNode_t to be tested
 * \return \e true if node is red
 */
static
bool
trbTreeIsRed(const trbTreeNode_t * const node)
{
  if ((node != NULL) && (node->colour == trbTreeRed))
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Does node have a left leaf?
 *
 * \param node pointer to \c trbTreeNode_t to be tested
 * \return \e true if node has a left leaf.
 */
static
bool
trbTreeLeftIsLeaf(const trbTreeNode_t * const node)
{
  if ((node != NULL) && (node->leftTag == trbTreeLeaf))
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Does node have a left child?
 *
 * \param node pointer to \c trbTreeNode_t to be tested
 * \return \e true if node has a left child
 */
static
bool
trbTreeLeftIsChild(const trbTreeNode_t * const node)
{
  if ((node != NULL) && (node->leftTag == trbTreeChild))
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
 * Private helper function for threaded red-black tree implementation.
 *
 * Does node have a right leaf?
 *
 * \param node pointer to \c trbTreeNode_t to be tested
 * \return \e true if node has a right leaf
 */
static
bool
trbTreeRightIsLeaf(const trbTreeNode_t * const node)
{
  if ((node != NULL) && (node->rightTag == trbTreeLeaf))
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Does node have a right child?
 *
 * \param node pointer to \c trbTreeNode_t to be tested
 * \return \e true if node has a right child
 */
static
bool
trbTreeRightIsChild(const trbTreeNode_t * const node)
{
  if ((node != NULL) && (node->rightTag == trbTreeChild))
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
 * Private helper function for threaded red-black tree implementation.
 *
 * Does node have a parent?
 *
 * \param node pointer to \c trbTreeNode_t to be tested
 * \return \e true if node has a parent
 */
static
bool
trbTreeHasParent(const trbTreeNode_t * const node)
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
 * Private helper function for threaded red-black tree implementation.
 *
 * Is it a valid node?
 *
 * \param tree pointer to \c trbTree_t
 * \param node pointer to \c trbTreeNode_t to be tested
 * \return \e true if \c node is not NULL, \e max or \e min.
 */
static
bool
trbTreeIsValid(const trbTree_t * const tree, const trbTreeNode_t * const node)
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
 * Private helper function for threaded red-black tree implementation.
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
 * \param tree pointer to \c trbTree_t
 * \param y pointer to \c trbTreeNode_t to be rotated right
 */
static
void
trbTreeRotateRight(trbTree_t * const tree, trbTreeNode_t *y)
{
  trbTreeNode_t *x = y->left;

  /* x's right subtree, b, becomes y's left subtree */
  y->left = x->right;
  y->leftTag = x->rightTag;
  if (trbTreeLeftIsChild(y)) 
    {
      (x->right)->parent = y;
    }
  else
    {
      x->rightTag = trbTreeChild;
      y->leftTag = trbTreeLeaf;
      y->left = x;
    }

  /* x replaces y */
  x->parent = y->parent;
  if (trbTreeHasParent(x)) 
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
 * Private helper function for threaded red-black tree implementation.
 *
 * Rotate node left maintaining order. Adjust the thread for the left-hand 
 * child of y.
 *
 \verbatim
     x               y   
    / \             / \  
   a   y   -->     x   c 
  / \             / \    
 b   c           a   b   
 \endverbatim
 * 
 * \param tree pointer to \c trbTree_t
 * \param x pointer to \c trbTreeNode_t to be rotated left
 */
static
void
trbTreeRotateLeft(trbTree_t * const tree, trbTreeNode_t *x)
{
  trbTreeNode_t *y = x->right;

  /* y's left subtree, b, becomes x's right subtree */
  x->right = y->left;
  x->rightTag = y->leftTag;  
  if (trbTreeRightIsChild(x)) 
    {
      (y->left)->parent = x;
    }
  else
    {
      y->leftTag = trbTreeChild;
      x->rightTag = trbTreeLeaf;
      x->right = y;
    }

  /* y replaces x */
  y->parent = x->parent;
  if (trbTreeHasParent(y))
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
 * Private helper function for threaded red-black tree implementation.
 *
 * After insertion of a new node:\n
 *    - if parent node is black tree is valid\n
 *    - if parent node is red tree then fix the tree
 * 
 * \param tree pointer to \c trbTree_t
 * \param node pointer to \c trbTreeNode_t that was inserted
 */
static
void 
trbTreeInsertColour(trbTree_t * const tree, trbTreeNode_t *node) 
{
  trbTreeNode_t *parent, *gparent, *uncle;

  while (trbTreeHasParent(node) && trbTreeIsRed(node->parent))
    {
      /* parent is red so gparent isn't NULL since root is black */
      parent = node->parent;
      gparent = parent->parent;
      if (parent == gparent->left) 
        {
          uncle = gparent->right;
          if (trbTreeRightIsChild(gparent) && trbTreeIsRed(uncle)) 
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
              trbTreeSetBlack(uncle);
              trbTreeSetBlackRed(parent, gparent);
              node = gparent;
              continue;
            }
          if (trbTreeRightIsChild(parent) && (parent->right == node))
            {
              /*
               *  The parent is red but the uncle is black; also, the 
               *  new node is the right child of its parent, and the 
               *  parent in turn is the left child of its parent. In 
               *  this case, we can perform a left rotation that 
               *  switches the roles of the new node and its parent; 
               *  then, we deal with the former parent node
               */
              trbTreeRotateLeft(tree, parent);
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
          trbTreeSetBlackRed(parent, gparent); 
          trbTreeRotateRight(tree, gparent); 
        } 
      else 
        { 
          /* See comments for right-hand case */
          uncle = gparent->left; 
          if (trbTreeLeftIsChild(gparent) && trbTreeIsRed(uncle)) 
            { 
              trbTreeSetBlack(uncle);
              trbTreeSetBlackRed(parent, gparent); 
              node = gparent;
              continue; 
            } 
          if (trbTreeLeftIsChild(parent) && (parent->left == node))
            {
              trbTreeRotateRight(tree, parent); 
              uncle = parent; 
              parent = node; 
              node = uncle; 
            } 
          trbTreeSetBlackRed(parent, gparent);
          trbTreeRotateLeft(tree, gparent); 
        } 
    }


  /* 
   *  Root node is always painted black 
   */
  trbTreeSetBlack(tree->root);
}


/**
 * Private helper function for threaded red-black tree implementation.
 *
 *  Fix the threaded red-black property after a normal tree deletion.
 *
 * \param tree pointer to \c trbTree_t
 * \param parent pointer to parent of \c trbTreeNode_t being deleted
 * \param node pointer to \c trbTreeNode_t replacing that being deleted
 */
static
void 
trbTreeRemoveColour(trbTree_t * const tree, 
                    trbTreeNode_t *parent, 
                    trbTreeNode_t *node) 
{
  trbTreeNode_t *sibling;

  while ((((node == NULL) || trbTreeIsBlack(node))) && 
         (node != tree->root))
    {
      if ((trbTreeLeftIsChild(parent) && (parent->left == node))
          ||
          (trbTreeLeftIsLeaf(parent) && (node == NULL))
          )
        {
          sibling = parent->right;
          if (trbTreeRightIsChild(parent) && trbTreeIsRed(sibling)) 
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
              trbTreeSetBlackRed(sibling, parent);
              trbTreeRotateLeft(tree, parent);
              sibling = parent->right;
              if (trbTreeRightIsLeaf(parent))
                {
                  continue;
                }
            }

          if ((trbTreeLeftIsLeaf(sibling) || 
               trbTreeIsBlack(sibling->left)) 
              && 
              (trbTreeRightIsLeaf(sibling) || 
               trbTreeIsBlack(sibling->right)))
            {
              /*
               *  sibling and siblings children are black. In this
               *  case set sibling to red.
               */
              trbTreeSetRed(sibling);
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
              if (trbTreeRightIsLeaf(sibling) || 
                  trbTreeIsBlack(sibling->right))
                {
                  if (trbTreeLeftIsChild(sibling)) 
                    {
                      trbTreeSetBlack(sibling->left);
                    }

                  trbTreeSetRed(sibling);
                  trbTreeRotateRight(tree, sibling);
                  sibling = parent->right;
                }

              trbTreeCopyColour(sibling, parent);
              trbTreeSetBlack(parent);

              if (trbTreeRightIsChild(sibling))
                {
                  trbTreeSetBlack(sibling->right);
                }

              trbTreeRotateLeft(tree, parent);
              node = tree->root;
              break;
            }
        }
      else
        {
          /* See comments for right-hand sibling */
          sibling = parent->left;
          if (trbTreeLeftIsChild(parent) && trbTreeIsRed(sibling)) 
            {
              trbTreeSetBlackRed(sibling, parent);
              trbTreeRotateRight(tree, parent);
              sibling = parent->left;
              if (trbTreeLeftIsLeaf(parent))
                {
                  continue;
                }
            }

          if ((trbTreeLeftIsLeaf(sibling) || 
               trbTreeIsBlack(sibling->left))
              && 
              (trbTreeRightIsLeaf(sibling) || 
               trbTreeIsBlack(sibling->right)))
            {
              trbTreeSetRed(sibling);
              node = parent;
              parent = node->parent;
            }
          else 
            {
              if (trbTreeLeftIsLeaf(sibling) || 
                  trbTreeIsBlack(sibling->left))
                {
                  if (trbTreeRightIsChild(sibling))
                    {
                      trbTreeSetBlack(sibling->right);
                    }

                  trbTreeSetRed(sibling);
                  trbTreeRotateLeft(tree, sibling);
                  sibling = parent->left;
                }

              trbTreeCopyColour(sibling, parent);
              trbTreeSetBlack(parent);

              if (trbTreeLeftIsChild(sibling))
                {
                  trbTreeSetBlack(sibling->left);
                }

              trbTreeRotateRight(tree, parent);
              node = tree->root;
              break;
            }
        }
    }

  if (trbTreeIsValid(tree, node))
    {
      trbTreeSetBlack(node);
    }
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Remove a node from the tree.
 *
 * \param tree pointer to \c trbTree_t
 * \param node pointer to \c trbTreeNode_t to be removed
 * \return pointer to \c trbTreeNode_t found. NULL if not found.
 */
static
trbTreeNode_t *
trbTreeRemoveNode(trbTree_t * const tree, trbTreeNode_t * const node) 
{
  trbTreeNode_t *child, *parent, *old, *succ, *pred;
  trbTreeColour_e colour;

  if (tree == NULL)
    {
      return NULL;
    }

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

  if (trbTreeLeftIsLeaf(node) && trbTreeRightIsLeaf(node))
    {
      parent = node->parent;
      colour = node->colour;
      child = NULL;
      if (trbTreeHasParent(node))
        {
          if (node == parent->left)
            {
              parent->left = node->left;
              parent->leftTag = trbTreeLeaf;
            }
          else
            {
              parent->right = node->right;
              parent->rightTag = trbTreeLeaf;
            }
        }
      else
        {
          tree->root = NULL;
        }
    }
  else if (trbTreeLeftIsLeaf(node) && trbTreeRightIsChild(node))
    {
      parent = node->parent;
      colour = node->colour;

      /* Child is successor of node in left subtree of node->right */
      child = node->right;
      if (trbTreeLeftIsChild(child))
        {
          while (trbTreeLeftIsChild(child))
            {
              child = child->left;
            }
          if (trbTreeRightIsChild(child))
            {
              child->parent->left = child->right;
              child->right->parent = child->parent;
            }
          else
            {
              child->parent->leftTag = trbTreeLeaf;
            }
          child->right = node->right;
          child->rightTag = trbTreeChild;
        }
      
      /* Install child. Recall left is a leaf! */
      child->left = node->left;
      child->leftTag = node->leftTag;
      child->parent = node->parent;
      if (trbTreeHasParent(node)) 
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
  else if (trbTreeLeftIsChild(node) && trbTreeRightIsLeaf(node))
    {
      parent = node->parent;
      colour = node->colour;

      /* Child is successor of node in right subtree of node->left */
      child = node->left;
      if (trbTreeRightIsChild(child))
        {
          while (trbTreeRightIsChild(child))
            {
              child = child->right;
            }
          if (trbTreeLeftIsChild(child))
            {
              child->parent->right = child->left;
              child->left->parent = child->parent;
            }
          else
            {
              child->parent->rightTag = trbTreeLeaf;
            }
          child->left = node->left;
          child->leftTag = trbTreeChild;
        }

      /* Install child. Recall right is a leaf! */
      child->right = node->right;
      child->rightTag = node->rightTag;
      child->parent = node->parent;
      if (trbTreeHasParent(node)) 
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
      if (trbTreeLeftIsLeaf(node->right))
        {
          /* Find child (whose colour may need to be fixed) */
          succ = node->right;
          colour = succ->colour;
          parent = succ;
          if (trbTreeRightIsChild(succ))
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
          while (trbTreeLeftIsChild(succ))
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
          if (trbTreeRightIsChild(succ))
            {
              succ->parent->left = succ->right;
              succ->right->parent = succ->parent;
              child = succ->right;
            }
          else
            {
              succ->parent->leftTag = trbTreeLeaf;
              child = NULL;
            }

         /* Replace succ right pointer */
          succ->right = node->right;
          succ->rightTag = trbTreeChild;

          /* Fix node right parent */
          node->right->parent = succ;
        }

      /* 
       *  Install succ. Recall left is a child! 
       */
      succ->left = node->left;
      succ->leftTag = node->leftTag;
      trbTreeCopyColour(succ, node);
      succ->parent = node->parent;

      /* 
       *  Fix parent link 
       */
      if (trbTreeHasParent(node)) 
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
      while (trbTreeRightIsChild(pred))
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
  if (colour == trbTreeBlack)
    {
      trbTreeRemoveColour(tree, parent, child);
    }

  /* Done */
  tree->size = tree->size-1;
  return node;
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Search for the next larger node (or successor) 
 *
 * \param node pointer to a \c trbTreeNode_t
 * \return pointer to next \c trbTreeNode_t found. \c NULL if not found.
 */
static
trbTreeNode_t * 
trbTreeFindNext(trbTreeNode_t * const node) 
{
  trbTreeNode_t *next;

  if (node == NULL)
    {
      return NULL;
    }

  if (trbTreeRightIsChild(node)) 
    {
      /* Find left-most node on right sub-tree */
      next = node->right;
      while (trbTreeLeftIsChild(next))
        {
          next = next->left;
        }
    }
  else 
    {
      /* Follow thread */
      next = node->right;
    }

  return next;
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 * Search for the next smaller node (or predecessor) 
 *
 * \param node pointer to a \c trbTreeNode_t
 * \return pointer to previous \c trbTreeNode_t found. 
 * \c NULL if not found.
 */
static
trbTreeNode_t * 
trbTreeFindPrevious(trbTreeNode_t * const node) 
{
  trbTreeNode_t *prev;

  if (node == NULL)
    {
      return NULL;
    }

  if (trbTreeLeftIsChild(node))
    {
      /* Find right-most node on left sub-tree */
      prev = node->left;
      while (trbTreeRightIsChild(prev))
        {
          prev = prev->right;
        }
    }
  else 
    {
      /* Follow thread */
      prev = node->left;
    }

  return prev;
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 *  Find left/right-most entry.
 *
 * \param tree pointer to \c trbTree_t
 * \param val \c compareLesser to find maximum, \c compareGreater to 
 * find minimum
 * \return pointer to minimum/maximum \c trbTreeNode_t found. 
 * \c NULL if not found.
 */
static
trbTreeNode_t *  
trbTreeFindMinMax(trbTree_t * const tree, const compare_e val) 
{
  trbTreeNode_t *node;

  if (tree == NULL)
    {
      return NULL;
    }

  node = tree->root;
  if (val == compareLesser) 
    {
      while (trbTreeLeftIsChild(node))
        {
          node = node->left;
        }
    }
  else 
    {
      while (trbTreeRightIsChild(node))
        {
          node = node->right;
        }
    }

  return node;
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 *  Given an entry find the corresponding node.
 *
 * \param tree pointer to \c trbTree_t
 * \param entry pointer to caller's data 
 * \return pointer to \c trbTreeNode_t found. 
 * \c NULL if not found.
 */
static
trbTreeNode_t * 
trbTreeFindNode(trbTree_t * const tree, const void * const entry)
{
  trbTreeNode_t *node;
  compare_e comp;
  
  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  /* Search for node */
  node = tree->root;
  while (trbTreeIsValid(tree, node))
    {
      comp = (tree->compare)(entry, node->entry, tree->user);
      if (comp == compareLesser) 
        {
          if (trbTreeLeftIsChild(node))
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
          if (trbTreeRightIsChild(node))
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
          return node;
        }
      else
        {
          tree->debug(__func__, __LINE__, tree->user,
                      "Illegal compare result!");
          break;
        }
    }

  return NULL;
}

/**
 * Private helper function for threaded red-black tree implementation.
 *
 *  Duplicate an entry.
 *
 * \param tree pointer to \c trbTree_t
 * \param entry pointer to caller's data 
 * \return pointer to duplicated entry
 */
static
void *
trbTreeDuplicateEntry(trbTree_t * const tree, void * const entry)
{
  void *new_entry;

  if (tree->duplicateEntry != NULL)
    {
      new_entry = tree->duplicateEntry(entry, tree->user);
      if (new_entry == NULL)
        {
          tree->debug(__func__, __LINE__, tree->user, 
                      "Couldn't duplicate entry!");
          return NULL;
        }   
    }
  else 
    {
      new_entry = entry;
    }

  return new_entry;
}

trbTree_t *
trbTreeCreate(const trbTreeAllocFunc_t alloc, 
              const trbTreeDeallocFunc_t dealloc,
              const trbTreeDuplicateEntryFunc_t duplicateEntry,
              const trbTreeDeleteEntryFunc_t deleteEntry,
              const trbTreeDebugFunc_t debug,
              const trbTreeCompFunc_t comp,
              void * const user)
{
  trbTree_t *tree; 

  if (debug == NULL)
    {
      return NULL;
    }
  if (alloc == NULL)
    {
      debug(__func__, __LINE__, user, "Invalid alloc() function!");
      return NULL;
    }
  if (dealloc == NULL)
    {
      debug(__func__, __LINE__, user, "Invalid dealloc() function!");
      return NULL;
    }
  if (comp == NULL)
    {
      debug(__func__, __LINE__, user, "Invalid comp() function!");
      return NULL;
    }

  tree = alloc(sizeof(trbTree_t), user);
  if (tree == NULL)
    {
      debug(__func__, __LINE__, user, 
            "Can't allocate %d for trbTree_t",
            sizeof(trbTree_t));
      return NULL;
    }
  tree->alloc = alloc;
  tree->dealloc = dealloc;
  tree->duplicateEntry = duplicateEntry; 
  tree->deleteEntry = deleteEntry;
  tree->debug = debug;
  tree->compare = comp;
  tree->user = user;
  tree->root = NULL;
  tree->current = NULL;
  tree->size = 0;

  return tree;
}

void *
trbTreeFind(trbTree_t * const tree, void * const entry)
{
  trbTreeNode_t *node;

  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  node = trbTreeFindNode(tree, entry);
  if (node == NULL)
    {
      return NULL;
    }
  else
    {
      tree->current = node;
      return node->entry;
    }
}

void *
trbTreeInsert(trbTree_t * const tree, void * const entry)
{
  trbTreeNode_t *node, *parent;
  compare_e comp;

  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  /* Search for an existing entry */
  node = tree->root;
  parent = NULL;
  comp = compareEqual;
  while (trbTreeIsValid(tree, node))
    {
      parent = node;
      comp = (tree->compare)(entry, parent->entry, tree->user);
      if (comp == compareLesser) 
        {
          if (trbTreeLeftIsChild(node))
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
          if (trbTreeRightIsChild(node))
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
          if (tree->deleteEntry != NULL)
            {
              (tree->deleteEntry)(node->entry, tree->user);
            }
          node->entry = trbTreeDuplicateEntry(tree, entry);
          if (node->entry == NULL)
            {
              (tree->debug)(__func__, __LINE__, tree->user,
                            "trbTreeDuplicateEntry() failed!");
              return NULL;
            }
          tree->current = node;
          return entry;
        }
      else
        {
          tree->debug(__func__, __LINE__, tree->user,
                      "Illegal compare result!");
          return NULL;
        }
    }

  /* Create a node for the new entry */
  node = (trbTreeNode_t *)(tree->alloc)(sizeof(trbTreeNode_t), tree->user);
  if (node == NULL)
    {
      return NULL;
    }

  /* 
   *  Inserting a node is much easier than removing 
   *  one because left and right are leaves!
   */
  node->entry = trbTreeDuplicateEntry(tree, entry);
  if (node->entry == NULL)
    {
      (tree->dealloc)(node, tree->user);
      (tree->debug)(__func__, __LINE__, tree->user,
                    "trbTreeDuplicateEntry() failed!");
      return NULL;
    }
  node->parent = parent;
  node->leftTag = trbTreeLeaf;
  node->rightTag = trbTreeLeaf;
  trbTreeSetRed(node);
  if (parent != NULL) 
    {
      if (comp == compareLesser) 
        {
          /* parent->left must have been a leaf! */
          node->right = parent; 
          node->left = parent->left;
          parent->left = node;
          parent->leftTag = trbTreeChild;
        }
      else 
        {
          /* parent->right must have been a leaf! */
          node->left = parent;
          node->right = parent->right;
          parent->right = node;
          parent->rightTag = trbTreeChild;
        }
    }
  else
    {
      tree->root = node;
      node->left = NULL;
      node->leftTag = trbTreeLeaf;
      node->right = NULL;
      node->rightTag = trbTreeLeaf;
    }

  /* Fix the red-black property */
  trbTreeInsertColour(tree, node);

  /* Done */
  tree->current = node;
  tree->size = tree->size+1;

  return node->entry;
}

void *
trbTreeRemove(trbTree_t * const tree, void * const entry) 
{
  trbTreeNode_t *old;
  void *oldEntry;

  if (tree == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      return NULL;
    }
  
  /* Remove node from tree */
  old = trbTreeFindNode(tree, entry);
  if (tree->current == old)
    {
      tree->current = NULL;
    }
  if (old == NULL)
    {
      return entry;
    }
  if (trbTreeRemoveNode(tree, old) == NULL)
    {
      return entry;
    }

  /* Deallocate the deleted node and entry */
  oldEntry = old->entry;
  (tree->dealloc)(old, tree->user);
  if(tree->deleteEntry != NULL)
    {
      (tree->deleteEntry)(oldEntry, tree->user);
      return NULL;
    }
  else
    {
      return oldEntry;
    }
}

void
trbTreeClear(trbTree_t * const tree)
{
  trbTreeNode_t *node;
  trbTreeNode_t *next;


  /* Sanity check */
  if ((tree == NULL) || (tree->dealloc == NULL))
    {
      return;
    }

  /* Delete all nodes */
  node = trbTreeFindMinMax(tree, compareLesser);
  while (trbTreeIsValid(tree, node))
    {
      next = trbTreeFindNext(node); 
      if ((node = trbTreeRemoveNode(tree, node)) != NULL)
        {
          if ((tree->deleteEntry != NULL) && (node->entry != NULL))
            {
              (tree->deleteEntry)(node->entry, tree->user);
            }
          tree->dealloc(node, tree->user);
        }
      node = next;
    }
}

void
trbTreeDestroy(trbTree_t * const tree)
{
  /* Sanity check */
  if ((tree == NULL) || (tree->dealloc == NULL))
    {
      return;
    }

  /* Delete all nodes */
  trbTreeClear(tree);

  /* Deallocate tree */
  (tree->dealloc)(tree, tree->user);
}

size_t
trbTreeGetDepth(const trbTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  size_t thisDepth = 1;
  size_t maxDepth = 0;
  trbTreeNode_t *prev = NULL;
  trbTreeNode_t *current = tree->root;
  trbTreeNode_t *next = NULL;
  bool nextIsLeaf = true;

  /* Non-recursive depth finder */
  while (current != NULL)
    {
      /* Descend subtree, next is predecessor */
      if (prev == current->parent)
        {
          prev = current;
          next = current->left;
          if (trbTreeLeftIsChild(current))
            {
              nextIsLeaf = false;
              thisDepth++;
            }
          else
            {
              nextIsLeaf = true;
            }
        }

      /* Current is root of sub-tree. Descend */
      if ((nextIsLeaf) || (prev == current->left))
        {
          prev = current;
          next = current->right;
          if (trbTreeRightIsChild(current))
            {
              nextIsLeaf = false;
              thisDepth++;
            }
          else
            {
              nextIsLeaf = true;
            }
        }

      /* Done with this sub-tree. Ascend */
      if((nextIsLeaf) || (prev == current->right))
        {
          thisDepth--;
          prev = current;
          next = current->parent;
          nextIsLeaf = false;
        }
    
      current = next;

      /* Compare */
      if (thisDepth > maxDepth)
        {
          maxDepth = thisDepth;
        }
      if (current == tree->root)
        {
          thisDepth = 1;
        }
    }

  return maxDepth;
}

size_t
trbTreeGetSize(const trbTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  return tree->size;
}

void *
trbTreeGetMax(trbTree_t * const tree) 
{
  trbTreeNode_t *node = trbTreeFindMinMax(tree, compareGreater);
    
  return (node == NULL) ? NULL : node->entry;
}

void *
trbTreeGetMin(trbTree_t * const tree) 
{
  trbTreeNode_t *node = trbTreeFindMinMax(tree, compareLesser);

  return (node == NULL) ? NULL : node->entry;
}

void *
trbTreeGetNext(trbTree_t * const tree, const void * const entry)
{
  trbTreeNode_t *node;

  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  /* Check current node */
  if ((tree->current != NULL) && 
      ((tree->compare)(entry, tree->current->entry, tree->user) 
       == compareEqual))
    {
      node = tree->current;
    }
  else
    {
      node = trbTreeFindNode(tree, entry);
    }
  node = trbTreeFindNext(node);
  tree->current = node;
  if (node == NULL)
    {
      return NULL;
    }

  return node->entry;
}

void *
trbTreeGetPrevious(trbTree_t * const tree, const void * const entry)
{
  trbTreeNode_t *node;

  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  /* Check current node */
  if ((tree->current != NULL) && 
      ((tree->compare)(entry, tree->current->entry, tree->user) 
       == compareEqual))
    {
      node = tree->current;
    }
  else
    {
      node = trbTreeFindNode(tree, entry);
    }
  node = trbTreeFindPrevious(node);
  tree->current = node;
  if (node == NULL)
    {
      return NULL;
    }
  return node->entry;
}

void *
trbTreeGetLower(trbTree_t * const tree, const void * const entry)
{
  compare_e comp;
  trbTreeNode_t *node;
  trbTreeNode_t *lower;
  
  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  node = tree->root;
  lower = node;
  while (trbTreeIsValid(tree, node))
    {
      comp = (tree->compare)(entry, node->entry, tree->user);
      if (comp == compareLesser) 
        {
          lower = node->left;
          if (trbTreeLeftIsLeaf(node))
            {
              lower =  trbTreeFindPrevious(node);
              break;
            }
        }
      else if (comp == compareGreater)
        {
          lower = node->right;
          if (trbTreeRightIsLeaf(node))
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
          tree->debug(__func__, __LINE__, tree->user,
                      "Illegal compare result!");
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
      return lower->entry;
    }
}

void *
trbTreeGetUpper(trbTree_t * const tree, const void * const entry)
{
  compare_e comp;
  trbTreeNode_t *node;
  trbTreeNode_t *upper;

  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  node = tree->root;
  upper = node;

  while (trbTreeIsValid(tree, node))
    {
      comp = (tree->compare)(entry, node->entry, tree->user);
      if (comp == compareLesser) 
        {
          upper = node->left;
          if (trbTreeLeftIsLeaf(node))
            {
              upper = node;
              break;
            }
        }
      else if (comp == compareGreater)
        {
          upper = node->right;
          if (trbTreeRightIsLeaf(node))
            {
              upper =  trbTreeFindNext(node);
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
          tree->debug(__func__, __LINE__, tree->user,
                      "Illegal compare result!");
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
      return upper->entry;
    }
}

bool
trbTreeWalk(trbTree_t * const tree, const trbTreeWalkFunc_t walk)
{
  bool res;
  trbTreeNode_t *node, *nextNode;

  if ((tree == NULL) || (walk == NULL))
    {
      return false;
    }

  node = trbTreeFindMinMax(tree, compareLesser);
  while (node != NULL)
    {
      nextNode = trbTreeFindNext(node); 
      if ((res = walk(node->entry, tree->user)) == false)
        {
          return false;
        }
      node = nextNode; 
    }

  return true;
}

#ifdef DEBUG_TRB_TREE_SHOW
/* For testing */
static
void
trbTreeNodeShow(trbTreeNode_t * const node, const size_t depth)
{
  fprintf(stderr,"\n%lu ",depth);
  for(size_t i=0;i<depth;i++)
    {
      fprintf(stderr,"\t");
    }
  fprintf(stderr,"node %p, %d, ", (void *)node, *(int *)node->entry);
  if (node->colour == trbTreeRed)
    {
      fprintf(stderr, "red");
    }
  else
    {
      fprintf(stderr, "black");
    }
  if ((node == NULL) || (node->leftTag == trbTreeLeaf))
    {
      fprintf(stderr, ", left is leaf %p", (void *)(node->left));
    }
  if ((node == NULL) || (node->leftTag == trbTreeChild))
    {
      fprintf(stderr, ", left is child %p", (void *)(node->left));
    }
  if ((node == NULL) || (node->rightTag == trbTreeLeaf))
    {
      fprintf(stderr, ", right is leaf %p", (void *)(node->right));
    }
  if ((node == NULL) || (node->rightTag == trbTreeChild))
    {
      fprintf(stderr, ", right is child %p", (void *)(node->right));
    }
  if (node->parent != NULL)
    {
      fprintf(stderr,", parent %p, %d",
              (void *)node->parent, *(int *)node->parent->entry);
    }
  else
    {
      fprintf(stderr,", root");
    }
}

static
void
trbTreeShow(trbTree_t * const tree)
{
  size_t thisDepth = 1;
  size_t maxDepth = 0;
  trbTreeNode_t *prev = NULL;
  trbTreeNode_t *current = tree->root;
  trbTreeNode_t *next = NULL;
  bool nextIsLeaf = true;

  if (tree == NULL)
    {
      return;
    }

  /* Non-recursive depth finder */
  while (current != NULL)
    {
      /* Descend subtree, next is predecessor */
      if (prev == current->parent)
        {
          prev = current;
          next = current->left;
          if (trbTreeLeftIsChild(current))
            {
              nextIsLeaf = false;
              thisDepth++;
            }
          else
            {
              nextIsLeaf = true;
            }
        }

      /* Current is root of sub-tree. Descend */
      if ((nextIsLeaf) || (prev == current->left))
        {
          prev = current;
          next = current->right;
          if (trbTreeRightIsChild(current))
            {
              nextIsLeaf = false;
              thisDepth++;
            }
          else
            {
              nextIsLeaf = true;
            }
        }

      /* Done with this sub-tree. Ascend */
      if((nextIsLeaf) || (prev == current->right))
        {
          thisDepth--;
          trbTreeNodeShow(current, thisDepth);
          prev = current;
          next = current->parent;
          nextIsLeaf = false;
        }
    
      current = next;

      /* Compare */
      if (thisDepth > maxDepth)
        {
          maxDepth = thisDepth;
        }
      if (current == tree->root)
        {
          thisDepth = 1;
        }
    }

  fprintf(stderr,"\n");
}
#endif

bool
trbTreeCheck(trbTree_t * const tree)
{
  trbTreeNode_t *node;
  size_t numNodes;

  if(tree == NULL)
    {
      return false;
    }

  /* Check forward node order */
  numNodes = 0;
  node = trbTreeFindMinMax(tree, compareLesser);
  while (node != NULL)
    {
      trbTreeNode_t *nextNode;

      /* Check node order */
      nextNode = trbTreeFindNext(node);
      if ((nextNode != NULL) &&
          ((tree->compare)(node->entry, nextNode->entry, tree->user) 
           != compareLesser))
        {   
          (tree->debug)(__func__, __LINE__, tree->user,
                        "Illegal node order!");
          return false;
        }

      /* Loop */
      numNodes++;
      node = nextNode;
    }

  /* Check size */
  if (numNodes != tree->size)
    {
      (tree->debug)(__func__, __LINE__, tree->user,
                    "Wrong size (%d should be %d)!", 
                    numNodes, tree->size);
      return false;
    }

  /* Check reverse node order */
  numNodes = 0;
  node = trbTreeFindMinMax(tree, compareGreater);
  while (node != NULL)
    {
      trbTreeNode_t *prevNode;

      /* Check node order */
      prevNode = trbTreeFindPrevious(node);
      if ((prevNode != NULL) &&
          ((tree->compare)(prevNode->entry, node->entry, tree->user) 
           != compareLesser))
        {   
          (tree->debug)(__func__, __LINE__, tree->user,
                        "Illegal node order!");
          return false;
        }

      /* Loop */
      numNodes++;
      node = prevNode;
    }

  /* Check size */
  if (numNodes != tree->size)
    {
      (tree->debug)(__func__, __LINE__, tree->user,
                    "Wrong size (%d should be %d)!", 
                    numNodes, tree->size);
      return false;
    }

  /* Check parent */
  node = trbTreeFindMinMax(tree, compareLesser);
  while (node != NULL)
    {
      if ((node->parent != NULL) &&
          (node->parent->left != node) &&
          (node->parent->right != node))
        {
          (tree->debug)(__func__, __LINE__, tree->user,
                        "No link from parent node!");
          return false;
        }

      node = trbTreeFindNext(node);
    }

  /* Check colour */
  node = trbTreeFindMinMax(tree, compareLesser);
  while (node != NULL)
    {
      if (trbTreeIsRed(node))
        {
          /* Right child must be black */
          if (trbTreeRightIsChild(node))
            {
              if (trbTreeIsRed(node->right))
                {
                  (tree->debug)(__func__, __LINE__, tree->user,
                                "Illegal colour (red)!");
                  return false;
                }
            }

          /* Left child must be black */
          if (trbTreeLeftIsChild(node))
            {
              if (trbTreeIsRed(node->left))
                {
                  (tree->debug)(__func__, __LINE__, tree->user,
                                "Illegal colour (red)!");
                  return false;
                }
            }
        }
      
      node = trbTreeFindNext(node);
    }

#ifdef DEBUG_TRB_TREE_SHOW
  trbTreeShow(tree);
#endif
  return true;
}
