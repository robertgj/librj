/**
 * \file redblackTree.c
 *
 * A redblackTree_t implementation.
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

#include <stdlib.h>
#include <stdbool.h>

#include "redblackTree.h"
#include "redblackTree_private.h"

/**
 * Private helper function for red-black tree implementation.
 *
 * Colour nodes.
 *
 * \param black pointer to redblackTreeNode_t to be coloured black
 * \param red pointer to redblackTreeNode_t to be coloured red
 */
static
void
redblackTreeSetBlackRed(redblackTreeNode_t * const black, 
                        redblackTreeNode_t * const red)
{
  black->colour = redblackTreeBlack;
  red->colour = redblackTreeRed;
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Colour node black.
 *
 * \param node pointer to redblackTreeNode_t to be coloured black
 */
static
void
redblackTreeSetBlack(redblackTreeNode_t * const node)
{
  node->colour = redblackTreeBlack;
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Colour node red.
 *
 * \param node pointer to redblackTreeNode_t to be coloured red
 */
static
void
redblackTreeSetRed(redblackTreeNode_t * const node)
{
  node->colour = redblackTreeRed;
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Copy node colour.
 *
 * \param dest pointer to destination redblackTreeNode_t for colour
 * \param src pointer to source redblackTreeNode_t of colour
 */
static
void
redblackTreeCopyColour(redblackTreeNode_t * const dest, 
                       const redblackTreeNode_t * const src)
{
  dest->colour = src->colour;
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Is node black?
 *
 * \param node pointer to redblackTreeNode_t to be tested
 * \return \e true if node is black
 */
static
bool
redblackTreeIsBlack(const redblackTreeNode_t * const node)
{
  if ((node != NULL) && (node->colour == redblackTreeBlack))
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Is node red?
 *
 * \param node pointer to redblackTreeNode_t to be tested
 * \return \e true if node is red
 */
static
bool
redblackTreeIsRed(const redblackTreeNode_t * const node)
{
  if ((node != NULL) && (node->colour == redblackTreeRed))
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Does node have a left leaf?
 *
 * \param node pointer to redblackTreeNode_t to be tested
 * \return \e true if node has a left leaf
 */
static
bool
redblackTreeLeftIsLeaf(const redblackTreeNode_t * const node)
{
  if ((node != NULL) && (node->left == NULL))
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Does node have a left child?
 *
 * \param node pointer to redblackTreeNode_t to be tested
 * \return \e true if node has a left child
 */
static
bool
redblackTreeLeftIsChild(const redblackTreeNode_t * const node)
{
  if ((node != NULL) && (node->left != NULL))
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Does node have a right leaf?
 *
 * \param node pointer to redblackTreeNode_t to be tested
 * \return \e true if node has a right leaf
 */
static
bool
redblackTreeRightIsLeaf(const redblackTreeNode_t * const node)
{
  if ((node != NULL) && (node->right == NULL))
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Does node have a right child?
 *
 * \param node pointer to redblackTreeNode_t to be tested
 * \return \e true if node has a right child
 */
static
bool
redblackTreeRightIsChild(const redblackTreeNode_t * const node)
{
  if ((node != NULL) && (node->right != NULL))
    {
      return true;
    }
  else
    {
      return false;
    }
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Does node have a parent?
 *
 * \param node pointer to redblackTreeNode_t to be tested
 * \return \e true if node has a parent
 */
static
bool
redblackTreeHasParent(const redblackTreeNode_t * const node)
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
 * Private helper function for red-black tree implementation.
 *
 * Rotate node right maintaining order.
 *
 \verbatim
     y            x  
    / \          / \ 
   x   c  -->   a   y
  / \              / \   
 a   b            b   c  
 \endverbatim
 *
 * \param tree pointer to redblackTree_t
 * \param y pointer to redblackTreeNode_t to be rotated right
 */
static
void
redblackTreeRotateRight(redblackTree_t * const tree, redblackTreeNode_t *y)
{
  redblackTreeNode_t *x = y->left;

  /* x's right subtree, b, becomes y's left subtree */
  y->left = x->right;
  if (redblackTreeLeftIsChild(y)) 
    {
      (x->right)->parent = y;
    }

  /* x replaces y */
  x->parent = y->parent;
  if (redblackTreeHasParent(x)) 
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
 * Private helper function for red-black tree implementation.
 *
 * Rotate node left maintaining order.
 *
 \verbatim
     x               y   
    / \             / \  
   a   y   -->     x   c 
  / \             / \    
 b   c           a   b   
 \endverbatim
 * 
 * \param tree pointer to redblackTree_t
 * \param x pointer to redblackTreeNode_t to be rotated left
 */
static
void
redblackTreeRotateLeft(redblackTree_t * const tree, redblackTreeNode_t *x)
{
  redblackTreeNode_t *y = x->right;

  /* y's left subtree, b, becomes x's right subtree */
  x->right = y->left;
  if (redblackTreeRightIsChild(x)) 
    {
      (y->left)->parent = x;
    }

  /* y replaces x */
  y->parent = x->parent;
  if (redblackTreeHasParent(y))
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
 * Private helper function for red-black tree implementation.
 *
 * After insertion of a new node:\n
 *    - if parent node is black tree is valid\n
 *    - if parent node is red tree then fix the tree
 * 
 * \param tree pointer to redblackTree_t
 * \param node pointer to redblackTreeNode_t that was inserted
 */
static
void 
redblackTreeInsertColour(redblackTree_t * const tree, redblackTreeNode_t *node) 
{
  redblackTreeNode_t *parent, *gparent, *uncle;

  while (redblackTreeHasParent(node) && redblackTreeIsRed(node->parent))
    {
      /* parent is red so gparent isn't NULL since root is black */
      parent = node->parent;
      gparent = parent->parent;
      if (parent == gparent->left) 
        {
          uncle = gparent->right;
          if (redblackTreeRightIsChild(gparent) && redblackTreeIsRed(uncle)) 
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
              redblackTreeSetBlack(uncle);
              redblackTreeSetBlackRed(parent, gparent);
              node = gparent;
              continue;
            }
          if (redblackTreeRightIsChild(parent) && (parent->right == node))
            {
              /*
               *  The parent is red but the uncle is black; also, the 
               *  new node is the right child of its parent, and the 
               *  parent in turn is the left child of its parent. In 
               *  this case, we can perform a left rotation that 
               *  switches the roles of the new node and its parent; 
               *  then, we deal with the former parent node
               */
              redblackTreeRotateLeft(tree, parent);
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
          redblackTreeSetBlackRed(parent, gparent); 
          redblackTreeRotateRight(tree, gparent); 
        } 
      else 
        { 
          /* See comments for right-hand case */
          uncle = gparent->left; 
          if (redblackTreeLeftIsChild(gparent) && redblackTreeIsRed(uncle)) 
            { 
              redblackTreeSetBlack(uncle);
              redblackTreeSetBlackRed(parent, gparent); 
              node = gparent;
              continue; 
            } 
          if (redblackTreeLeftIsChild(parent) && (parent->left == node))
            {
              redblackTreeRotateRight(tree, parent); 
              uncle = parent; 
              parent = node; 
              node = uncle; 
            } 
          redblackTreeSetBlackRed(parent, gparent);
          redblackTreeRotateLeft(tree, gparent); 
        } 
    }


  /* 
   *  Root node is always painted black 
   */
  redblackTreeSetBlack(tree->root);
}


/**
 * Private helper function for red-black tree implementation.
 *
 *  Fix the red-black property after a normal tree deletion.
 *
 * \param tree pointer to redblackTree_t
 * \param parent pointer to parent of redblackTreeNode_t being deleted
 * \param node pointer to redblackTreeNode_t replacing that being deleted
 */
static
void 
redblackTreeRemoveColour(redblackTree_t * const tree, 
                         redblackTreeNode_t *parent, 
                         redblackTreeNode_t *node) 
{
  redblackTreeNode_t *sibling;

  while ((((node == NULL) || redblackTreeIsBlack(node))) && 
         (node != tree->root))
    {
      if (parent->left == node) 
        {
          sibling = parent->right;
          if (redblackTreeRightIsChild(parent) && redblackTreeIsRed(sibling)) 
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
              redblackTreeSetBlackRed(sibling, parent);
              redblackTreeRotateLeft(tree, parent);
              sibling = parent->right;
            }

          if ((redblackTreeLeftIsLeaf(sibling) || 
               redblackTreeIsBlack(sibling->left)) 
              && 
              (redblackTreeRightIsLeaf(sibling) || 
               redblackTreeIsBlack(sibling->right)))
            {
              /*
               *  sibling and siblings children are black. In this
               *  case set sibling to red.
               */
              redblackTreeSetRed(sibling);
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
              if (redblackTreeRightIsLeaf(sibling) || 
                  redblackTreeIsBlack(sibling->right))
                {
                  if (redblackTreeLeftIsChild(sibling)) 
                    {
                      redblackTreeSetBlack(sibling->left);
                    }

                  redblackTreeSetRed(sibling);
                  redblackTreeRotateRight(tree, sibling);
                  sibling = parent->right;
                }

              redblackTreeCopyColour(sibling, parent);
              redblackTreeSetBlack(parent);

              if (redblackTreeRightIsChild(sibling))
                {
                  redblackTreeSetBlack(sibling->right);
                }

              redblackTreeRotateLeft(tree, parent);
              node = tree->root;
              break;
            }
        }
      else
        {
          /* See comments for right-hand sibling */
          sibling = parent->left;
          if (redblackTreeLeftIsChild(parent) && redblackTreeIsRed(sibling)) 
            {
              redblackTreeSetBlackRed(sibling, parent);
              redblackTreeRotateRight(tree, parent);
              sibling = parent->left;
            }

          if ((redblackTreeLeftIsLeaf(sibling) || 
               redblackTreeIsBlack(sibling->left))
              && 
              (redblackTreeRightIsLeaf(sibling) || 
               redblackTreeIsBlack(sibling->right)))
            {
              redblackTreeSetRed(sibling);
              node = parent;
              parent = node->parent;
            }
          else 
            {
              if (redblackTreeLeftIsLeaf(sibling) || 
                  redblackTreeIsBlack(sibling->left))
                {
                  if (redblackTreeRightIsChild(sibling))
                    {
                      redblackTreeSetBlack(sibling->right);
                    }

                  redblackTreeSetRed(sibling);
                  redblackTreeRotateLeft(tree, sibling);
                  sibling = parent->left;
                }

              redblackTreeCopyColour(sibling, parent);
              redblackTreeSetBlack(parent);

              if (redblackTreeLeftIsChild(sibling))
                {
                  redblackTreeSetBlack(sibling->left);
                }

              redblackTreeRotateRight(tree, parent);
              node = tree->root;
              break;
            }
        }
    }

  if (node != NULL) 
    {
      redblackTreeSetBlack(node);
    }
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Remove a node from the tree.
 *
 * \param tree pointer to redblackTree_t
 * \param node pointer to redblackTreeNode_t to be removed
 * \return pointer to redblackTreeNode_t found. \c NULL if not found.
 */
static
redblackTreeNode_t *
redblackTreeRemoveNode(redblackTree_t * const tree, 
                       redblackTreeNode_t * const node) 
{
  redblackTreeNode_t *child, *parent, *succ;
  redblackTreeColour_e colour;

  if (tree == NULL)
    {
      return NULL;
    }

  if ((succ = node) == NULL)
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
   *  violate any red-black properties, this reduces the problem of
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
   *  equally to avoid unbalancing the tree. Here the red-black 
   *  property maintains balance.
   */
  if (redblackTreeLeftIsLeaf(succ) || redblackTreeRightIsLeaf(succ))
    {
      /* 
       *  Node has at most one child 
       */
      if (redblackTreeLeftIsLeaf(succ))
        {
          child = succ->right;
        }
      else
        {
          child = succ->left;
        }

      /* 
       *  Fix the parent links to the replacement 
       *  node for the single child cases.
       */
      parent = succ->parent;
      colour = succ->colour;
      if (child != NULL) 
        {
          child->parent = parent;
        }
      if (redblackTreeHasParent(succ)) 
        {
          if (parent->left == succ) 
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
       *  Node has two children 
       *
       *  Find the minimum node of the right sub-tree. Since this 
       *  is the successor to the node being deleted it must have 
       *  only a right child.
       */
      succ = succ->right;
      while (redblackTreeLeftIsChild(succ))
        {
          succ = succ->left;
        }

      /* 
       *  Replace the minimum right-subtree node with its right child
       *  (This is the node of the tree whose colour may be altered.)
       */
      child = succ->right;
      parent = succ->parent;
      colour = succ->colour;
      if (redblackTreeRightIsChild(succ))
        {
          child->parent = parent;
        }
      /* Note that if succ->parent == node we are modifying node here */
      if (parent->left == succ) 
        {
          parent->left = child;
        }
      else 
        {
          parent->right = child;
        }

      /*
       *  Replace node (the node being deleted) with succ
       */
      if (succ->parent == node) 
        {
          parent = succ;
        }
      /* Replace everything except entry */
      succ->parent = node->parent;
      succ->left = node->left;
      succ->right = node->right;
      redblackTreeCopyColour(succ, node);

      /* Fix link from parent of node */
      if (redblackTreeHasParent(node)) 
        {
          if ((node->parent)->left == node) 
            {
              (node->parent)->left = succ;
            }
          else 
            {
              (node->parent)->right = succ;
            }
        }
      else 
        {
          tree->root = succ;
        }

      /* Fix parent links of children of node */
      if (redblackTreeLeftIsChild(node))
        {
          (node->left)->parent = succ;
        }
      if (redblackTreeRightIsChild(node))
        {
          (node->right)->parent = succ;
        }     
    }

  /*
   *  Now fix the colour. If we are deleting a red node, we 
   *  can simply replace it with its child, which must be black.
   *  If deleting a black node we need to fix the tree.
   */
  if (colour == redblackTreeBlack)
    {
      redblackTreeRemoveColour(tree, parent, child);
    }

  /* Done */
  tree->size = tree->size-1;
  return node;
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Search for the next larger node (or successor) 
 *
 * \param node pointer to a redblackTreeNode_t
 * \return pointer to next redblackTreeNode_t found. \c NULL if not found.
 */
static
redblackTreeNode_t *
redblackTreeFindNext(redblackTreeNode_t * const node) 
{
  redblackTreeNode_t *next;

  if (node == NULL)
    {
      return NULL;
    }

  if (redblackTreeRightIsChild(node))
    {
      /* Find left-most node on right sub-tree */
      next = node->right;
      while (redblackTreeLeftIsChild(next))
        {
          next = next->left;
        }
    }
  else 
    {
      if (redblackTreeHasParent(node) && (node == (node->parent)->left)) 
        {
          /* For left sub-tree of nodes parent, next must be parent node */
          next = node->parent;
        }
      else 
        {
          /*
           *      o (return this node)
           *     / \
           *    o   ?
           *   / \
           *  ?   o
           *     / \
           *    ?   o (original node)
           *       / \
           *      ?   NULL
           */
      
          /* Find parent node of right sub-trees */
          next = node;
          while (redblackTreeHasParent(next) && 
                 (next == (next->parent)->right)) 
            {
              next = next->parent;
            }

          /* Now, go to node for which this is a left sub-tree */
          next = next->parent;
        }
    }

  return next;
}

/**
 * Private helper function for red-black tree implementation.
 *
 * Search for the next smaller node (or predecessor) 
 *
 * \param node pointer to a redblackTreeNode_t
 * \return pointer to previous redblackTreeNode_t found. 
 * \c NULL if not found.
 */
static
redblackTreeNode_t *
redblackTreeFindPrevious(redblackTreeNode_t * const node) 
{
  redblackTreeNode_t *prev;

  if (node == NULL)
    {
      return NULL;
    }

  if (redblackTreeLeftIsChild(node))
    {
      /* Find right-most node on left sub-tree */
      prev = node->left;
      while (redblackTreeRightIsChild(prev))
        {
          prev = prev->right;
        }
    }
  else 
    {
      if (redblackTreeHasParent(node) && (node == (node->parent)->right)) 
        {
          /*  
           *  For right sub-tree of nodes parent, 
           *  previous must be parent
           */
          prev = node->parent;
        }
      else 
        {
          /* Find parent node of left sub-trees */
          prev = node;
          while (redblackTreeHasParent(prev) && 
                 (prev == (prev->parent)->left)) 
            {
              prev = prev->parent;
            }

          /* Now, go to node for which this is a right sub-tree */
          prev = prev->parent;
        }
    }

  return prev;
}

/**
 * Private helper function for red-black tree implementation.
 *
 *  Find left/right-most entry.
 *
 * \param tree pointer to redblackTree_t
 * \param val \c compareGreater to find maximum, \c compareLesser to 
 * find minimum
 * \return pointer to minimum/maximum redblackTreeNode_t found. 
 * \c NULL if not found.
 */
static
redblackTreeNode_t *
redblackTreeFindMinMax(redblackTree_t * const tree, const compare_e val) 
{
  redblackTreeNode_t *node;

  if (tree == NULL)
    {
      return NULL;
    }

  if (tree->root == NULL)
    {
      return NULL;
    }

  node = tree->root;
  if (val == compareLesser) 
    {
      while (redblackTreeLeftIsChild(node))
        {
          node = node->left;
        }
    }
  else 
    {
      while (redblackTreeRightIsChild(node))
        {
          node = node->right;
        }
    }

  return node;
}

/**
 * Private helper function for red-black tree implementation.
 *
 *  Given an entry find the corresponding node.
 *
 * \param tree pointer to redblackTree_t
 * \param entry pointer to caller's data 
 * \return pointer to redblackTreeNode_t found. 
 * \c NULL if not found.
 */
static
redblackTreeNode_t *
redblackTreeFindNode(redblackTree_t * const tree, const void * const entry)
{
  redblackTreeNode_t *node;
  compare_e comp;
  
  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  /* Search for node */
  node = tree->root;
  while (node != NULL) 
    {
      comp = (tree->compare)(entry, node->entry, tree->user);
      if (comp == compareLesser) 
        {
          node = node->left;
        }
      else if (comp == compareGreater)
        {
          node = node->right;
        }
      else if (comp == compareEqual)
        {
          return node;
        }
      else
        {
          (tree->debug)(__func__, __LINE__, tree->user,
                        "Illegal compare result!");
          break;
        }
    }

  return NULL;
}

/**
 * Private helper function for red-black tree implementation.
 *
 *  Duplicate an entry.
 *
 * \param tree pointer to redblackTree_t
 * \param entry pointer to caller's data 
 * \return pointer to duplicated entry
 */
static
void *
redblackTreeDuplicateEntry(redblackTree_t * const tree, void * const entry)
{
  void *new_entry;

  if (tree == NULL)
    {
      return NULL;
    }

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

redblackTree_t *
redblackTreeCreate(const redblackTreeAllocFunc_t alloc, 
                   const redblackTreeDeallocFunc_t dealloc,
                   const redblackTreeDuplicateEntryFunc_t duplicateEntry,
                   const redblackTreeDeleteEntryFunc_t deleteEntry,
                   const redblackTreeDebugFunc_t debug,
                   const redblackTreeCompFunc_t comp,
                   void * const user)
{
  redblackTree_t *tree; 

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
  
  tree = alloc(sizeof(redblackTree_t), user);
  if (tree == NULL)
    {
      debug(__func__, __LINE__, user, 
            "Can't allocate %d for redblackTree_t",
            sizeof(redblackTree_t));
      return NULL;
    }
  tree->compare = comp;
  tree->alloc = alloc;
  tree->dealloc = dealloc;
  tree->duplicateEntry = duplicateEntry;
  tree->deleteEntry = deleteEntry;
  tree->debug = debug;
  tree->root = NULL;
  tree->current = NULL;
  tree->size = 0;
  tree->user = user;

  return tree;
}

void *
redblackTreeFind(redblackTree_t * const tree, void * const entry)
{
  redblackTreeNode_t *node;

  if (tree == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      return NULL;
    }

  node = redblackTreeFindNode(tree, entry);
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
redblackTreeInsert(redblackTree_t * const tree, void * const entry)
{
  redblackTreeNode_t *node, *parent;
  compare_e comp;

  if (tree == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      return NULL;
    }

  /* Search for an existing entry */
  node = tree->root;
  parent = NULL;
  comp = compareEqual;
  while (node != NULL) 
    {
      parent = node;
      comp = (tree->compare)(entry, parent->entry, tree->user);
      if (comp == compareLesser) 
        {
          node = node->left;
        }
      else if (comp == compareGreater) 
        {
          node = node->right;
        }
      else if (comp == compareEqual)
        {
          if (tree->deleteEntry != NULL)
            {
              (tree->deleteEntry)(node->entry, tree->user);
            }
          node->entry = redblackTreeDuplicateEntry(tree, entry);
          if (node->entry == NULL)
            {
              (tree->debug)(__func__, __LINE__, tree->user,
                            "redblackTreeDuplicateEntry() failed!");
              return NULL;
            }
          tree->current = node;
          return node->entry;
        }
      else
        {
          (tree->debug)(__func__, __LINE__, tree->user,
                        "Illegal compare result!");
          return NULL;
        }
    }

  /* Not found. Create a node for the new entry */
  node = 
    (redblackTreeNode_t *)(tree->alloc)(sizeof(redblackTreeNode_t), tree->user);
  if (node == NULL)
    {
      return NULL;
    }
  node->entry = redblackTreeDuplicateEntry(tree, entry);
  if (node->entry == NULL)
    {
      (tree->dealloc)(node, tree->user);
      (tree->debug)(__func__, __LINE__, tree->user,
                    "redblackTreeDuplicateEntry() failed!");
      return NULL;
    }
  node->parent = parent;
  node->left = node->right = NULL;
  redblackTreeSetRed(node);
  if (parent != NULL) 
    {
      if (comp == compareLesser) 
        {
          parent->left = node;
        }
      else 
        {
          parent->right = node;
        }
    }
  else
    {
      tree->root = node;
    }

  /* Fix the red-black property */
  redblackTreeInsertColour(tree, node);

  /* Done */
  tree->current = node;
  tree->size = tree->size+1;

  return node->entry;
}

void *
redblackTreeRemove(redblackTree_t * const tree, void * const entry) 
{
  redblackTreeNode_t *old;
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
  old = redblackTreeFindNode(tree, entry);
  if (tree->current == old)
    {
      tree->current = NULL;
    }
  if (old == NULL)
    {
      return entry;
    }
  if (redblackTreeRemoveNode(tree, old) == NULL)
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
redblackTreeClear(redblackTree_t * const tree)
{
  redblackTreeNode_t *node;
  redblackTreeNode_t *next;

  /* Sanity check */
  if ((tree == NULL) || (tree->dealloc == NULL))
    {
      return;
    }

  /* Delete all nodes */
  node = redblackTreeFindMinMax(tree, compareLesser);
  while (node != NULL)
    {
      next = redblackTreeFindNext(node); 
      if ((node = redblackTreeRemoveNode(tree, node)) != NULL)
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
redblackTreeDestroy(redblackTree_t * const tree)
{
  /* Sanity check */
  if ((tree == NULL) || (tree->dealloc == NULL))
    {
      return;
    }

  /* Delete all nodes */
  redblackTreeClear(tree);

  /* Deallocate tree */
  (tree->dealloc)(tree, tree->user);
}

size_t
redblackTreeGetDepth(const redblackTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  size_t thisDepth = 1;
  size_t maxDepth = 0;
  redblackTreeNode_t *prev = NULL;
  redblackTreeNode_t *current = tree->root;
  redblackTreeNode_t *next = NULL;

  /* Non-recursive depth finder */
  while (current != NULL)
    {
      /* Descend subtree, next is predecessor */
      if (prev == current->parent)
        {
          prev = current;
          next = current->left;
          if (redblackTreeLeftIsChild(current))
            {
              thisDepth++;
            }
        }

      /* Current is root of sub-tree. Descend */
      if ((next == NULL) || (prev == current->left))
        {
          prev = current;
          next = current->right;
          if (redblackTreeRightIsChild(current))
            {
              thisDepth++;
            }
        }

      /* Done with this sub-tree. Ascend */
      if((next == NULL) || (prev == current->right))
        {
          thisDepth--;
          prev = current;
          next = current->parent;
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
redblackTreeGetSize(const redblackTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  return tree->size;
}

void *
redblackTreeGetMax(redblackTree_t * const tree) 
{
  redblackTreeNode_t *node;

  if (tree == NULL)
    {
      return NULL;
    }

  node = redblackTreeFindMinMax(tree, compareGreater);

  return (node == NULL) ? NULL : node->entry;
}

void *
redblackTreeGetMin(redblackTree_t * const tree) 
{
  redblackTreeNode_t *node;

  if (tree == NULL)
    {
      return NULL;
    }

  node = redblackTreeFindMinMax(tree, compareLesser);

  return (node == NULL) ? NULL : node->entry;
}

void *
redblackTreeGetNext(redblackTree_t * const tree, const void * const entry)
{
  redblackTreeNode_t *node;

  if (tree == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
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
      node = redblackTreeFindNode(tree, entry);
    }
  node = redblackTreeFindNext(node);
  tree->current = node;
  if (node == NULL)
    {
      return NULL;
    }

  return node->entry;
}

void *
redblackTreeGetPrevious(redblackTree_t * const tree, const void * const entry)
{
  redblackTreeNode_t *node;

  if (tree == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
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
      node = redblackTreeFindNode(tree, entry);
    }
  node = redblackTreeFindPrevious(node);
  tree->current = node;
  if (node == NULL)
    {
      return NULL;
    }

  return node->entry;
}

void *
redblackTreeGetLower(redblackTree_t * const tree, const void * const entry)
{
  compare_e comp;
  redblackTreeNode_t *node;
  redblackTreeNode_t *lower;

  if (tree == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      return NULL;
    }

  node = tree->root;
  lower = node;
  while (node != NULL) 
    {
      comp = (tree->compare)(entry, node->entry, tree->user);
      if (comp == compareLesser) 
        {
          lower = node->left;
          if (redblackTreeLeftIsLeaf(node))
            {
              lower =  redblackTreeFindPrevious(node);
              break;
            }
        }
      else if (comp == compareGreater)
        {
          lower = node->right;
          if (redblackTreeRightIsLeaf(node))
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
          (tree->debug)(__func__, __LINE__, tree->user,
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
redblackTreeGetUpper(redblackTree_t * const tree, const void * const entry)
{
  compare_e comp;
  redblackTreeNode_t *node;
  redblackTreeNode_t *upper;

  if (tree == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      return NULL;
    }

  node = tree->root;
  upper = node;
  while (node != NULL) 
    {
      comp = (tree->compare)(entry, node->entry, tree->user);
      if (comp == compareLesser) 
        {
          upper = node->left;
          if (redblackTreeLeftIsLeaf(node))
            {
              upper = node;
              break;
            }
        }
      else if (comp == compareGreater)
        {
          upper = node->right;
          if (redblackTreeRightIsLeaf(node))
            {
              upper =  redblackTreeFindNext(node);
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
          (tree->debug)(__func__, __LINE__, tree->user,
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
redblackTreeWalk(redblackTree_t * const tree, const redblackTreeWalkFunc_t walk)
{
  bool res;
  redblackTreeNode_t *node, *nextNode;

  if (tree == NULL)
    {
      return false;
    }
  if (walk == NULL)
    {
      return false;
    }

  node = redblackTreeFindMinMax(tree, compareLesser);
  while (node != NULL)
    {
      nextNode = redblackTreeFindNext(node); 
      if ((res = walk(node->entry, tree->user)) == false)
        {
          return false;
        }
      node = nextNode; 
    }

  return true;
}

bool
redblackTreeCheck(redblackTree_t * const tree)
{
  redblackTreeNode_t *node, *nextNode, *prevNode;
  size_t numNodes;  

  if(tree == NULL)
    {
      return false;
    }

  /* Check node order */
  numNodes = 0;
  node = redblackTreeFindMinMax(tree, compareLesser); 
  while (node != NULL)
    {
      nextNode = redblackTreeFindNext(node);
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

  /* Check node order */
  numNodes = 0;
  node = redblackTreeFindMinMax(tree, compareGreater); 
  while (node != NULL)
    {
      prevNode = redblackTreeFindPrevious(node);
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
  node = redblackTreeFindMinMax(tree, compareLesser);
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

      node = redblackTreeFindNext(node);
    }

  /* Check colour */
  node = redblackTreeFindMinMax(tree, compareLesser); 
  while (node != NULL)
    {
      if (redblackTreeIsRed(node))
        {
          if (redblackTreeRightIsChild(node))
            {
              if (redblackTreeIsRed(node->right))
                {
                  (tree->debug)(__func__, __LINE__, tree->user,
                                "Illegal colour (red)!");
                  return false;
                }
            }
          if (redblackTreeLeftIsChild(node))
            {
              if (redblackTreeIsRed(node->left))
                {
                  (tree->debug)(__func__, __LINE__, tree->user,
                                "Illegal colour (red)!");
                  return false;
                }
            }
        }

      /* Loop */
      node = redblackTreeFindNext(node);
    }

  return true;
}
