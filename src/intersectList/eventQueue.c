/**
 * \file eventQueue.c
 *
 *  Given a list of segments, each segment endpoint is an event. The
 *  event queue is a binary tree of events. A list of segments having
 *  the same upper endpoint is stored with each endpoint.
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

/*
 * From the <a href="http://en.wikipedia.org/wiki/Red-black_tree">
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
#include <stdarg.h>
#include <stdbool.h>

#include "point.h"
#include "segment.h"
#include "segmentList.h"
#include "eventQueue.h"

/**
 * Event queue node colour. 
 *
 * Enumeration of red-black tree node colour.
 */
typedef enum eventQueueColour_e 
  {
    eventQueueBlack = 0, eventQueueRed
  }
eventQueueColour_e;

/**
 * \c event_t structure. 
 *
 * Internal representation of an event type. This type is intended 
 * to be opaque. It is defined here to simplify memory allocation.
 *
 * Point memory is allocated here so we don't have to distinguish
 * between points allocated for segment end points and points 
 * allocated when internal segment intersections are found.
 *
 * The other entries are for the red-black tree used for the queue.
 *
 */
struct event_t
{
  point_t point;
  /**< Event point. */

  segmentList_t *upperList;
  /**< List of segments having \e point as upper end point. */

  struct event_t *left;
  /**< Pointer to left-hand child of this event node in the tree. */

  struct event_t *right;
  /**< Pointer to right-hand child of this event node in the tree. */

  struct event_t *parent;
  /**< Pointer to parent of this event node in the tree. */

  eventQueueColour_e colour;
  /**< Colour of this event node in the tree. */
};

/**
 * eventQueue_t structure.
 * 
 * Private implementation of eventQueue_t. The event queue is
 * implemented as a red-black tree. This implementation uses a parent
 * pointer in event_t rather than a stack of parent nodes in
 * eventQueue_t. 
 */
struct eventQueue_t 
{
  eventQueueAllocFunc_t alloc; 
  /**< Memory allocator callback function for eventQueue_t. */

  eventQueueDeallocFunc_t dealloc;
  /**< Memory deallocator callback function for eventQueue_t. */

  eventQueueDebugFunc_t debug;
  /**< Debugging message callback function. */

  event_t *root;
  /**< Root entry of the event queue red-black tree. */

  size_t size;
  /**< Number of entries in the eventQueue. */
};

/**
 * Private helper function for event queue implementation.
 *
 * Colour nodes.
 *
 * \param black pointer to event_t to be coloured black
 * \param red pointer to event_t to be coloured red
 */
static
void
eventQueueSetBlackRed(event_t *black, event_t *red)
{
  black->colour = eventQueueBlack;
  red->colour = eventQueueRed;
}

/**
 * Private helper function for event queue implementation.
 *
 * Colour node black.
 *
 * \param event pointer to event_t to be coloured black
 */
static
void
eventQueueSetBlack(event_t *event)
{
  event->colour = eventQueueBlack;
}

/**
 * Private helper function for event queue implementation.
 *
 * Colour node red.
 *
 * \param event pointer to event_t to be coloured red
 */
static
void
eventQueueSetRed(event_t *event)
{
  event->colour = eventQueueRed;
}

/**
 * Private helper function for event queue implementation.
 *
 * Copy node colour.
 *
 * \param dest pointer to destination event_t for colour
 * \param src pointer to source event_t of colour
 */
static
void
eventQueueCopyColour(event_t * const dest, const event_t * const src)
{
  dest->colour = src->colour;
}

/**
 * Private helper function for event queue implementation.
 *
 * Is node black?
 *
 * \param event pointer to event_t to be tested
 * \return \e true if node is black
 */
static
bool
eventQueueIsBlack(const event_t * const event)
{
  if ((event != NULL) && (event->colour == eventQueueBlack))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for event queue implementation.
 *
 * Is node red?
 *
 * \param event pointer to event_t to be tested
 * \return \e true if node is red
 */
static
bool
eventQueueIsRed(const event_t * const event)
{
  if ((event != NULL) && (event->colour == eventQueueRed))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for event queue implementation.
 *
 * Does node have a left leaf?
 *
 * \param event pointer to event_t to be tested
 * \return \e true if node has a left leaf
 */
static
bool
eventQueueLeftIsLeaf(const event_t * const event)
{
  if ((event != NULL) && (event->left == NULL))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for event queue implementation.
 *
 * Does node have a left child?
 *
 * \param event pointer to event_t to be tested
 * \return \e true if node has a left child
 */
static
bool
eventQueueLeftIsChild(const event_t * const event)
{
  if ((event != NULL) && (event->left != NULL))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for event queue implementation.
 *
 * Does node have a right leaf?
 *
 * \param event pointer to event_t to be tested
 * \return \e true if node has a right leaf
 */
static
bool
eventQueueRightIsLeaf(const event_t * const event)
{
  if ((event != NULL) && (event->right == NULL))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for event queue implementation.
 *
 * Does node have a right child?
 *
 * \param event pointer to event_t to be tested
 * \return \e true if node has a right child
 */
static
bool
eventQueueRightIsChild(const event_t * const event)
{
  if ((event != NULL) && (event->right != NULL))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for event queue implementation.
 *
 * Does node have a parent?
 *
 * \param event pointer to event_t to be tested
 * \return \e true if node has a parent
 */
static
bool
eventQueueHasParent(const event_t * const event)
{
  if ((event != NULL) && (event->parent != NULL))
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * Private helper function for event queue implementation.
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
 * \param queue pointer to eventQueue_t
 * \param y pointer to event_t to be rotated right
 */
static
void
eventQueueRotateRight(eventQueue_t * const queue, event_t * const y)
{
  event_t *x = y->left;

  if (queue == NULL)
    {
      return;
    }

  if (x == NULL)
    {
      /* Should not get here! */
      fprintf(stderr, "%s:%d : x == NULL!", __func__, __LINE__);
      exit(-1);
    }

  /* x's right subtree, b, becomes y's left subtree */
  y->left = x->right;
  if (eventQueueLeftIsChild(y)) 
    {
      (x->right)->parent = y;
    }

  /* x replaces y */
  x->parent = y->parent;
  if (eventQueueHasParent(x)) 
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
      queue->root = x;
    }

  /* x becomes y's parent */
  x->right = y;
  y->parent = x;
}

/**
 * Private helper function for event queue implementation.
 *
 * Rotate node left maintaining order.
 *
 \verbatim
      x               y   
     / \             / \  
    a   y   -->     x   c 
       / \         / \    
      b   c       a   b   
 \endverbatim
 * 
 * \param queue pointer to eventQueue_t
 * \param x pointer to event_t to be rotated left
 */
static
void
eventQueueRotateLeft(eventQueue_t * const queue, event_t * const x)
{
  event_t *y = x->right;

  if (queue == NULL)
    {
      return;
    }

  /* y's left subtree, b, becomes x's right subtree */
  x->right = y->left;
  if (eventQueueRightIsChild(x)) 
    {
      (y->left)->parent = x;
    }

  /* y replaces x */
  y->parent = x->parent;
  if (eventQueueHasParent(y))
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
      queue->root = y;
    }

  /* y becomes x's parent */
  y->left = x;
  x->parent = y;
}

/**
 * Private helper function for event queue implementation.
 *
 * After insertion of a new node:\n
 *    - if parent node is black tree is valid\n
 *    - if parent node is red tree then fix the tree
 * 
 * \param queue pointer to eventQueue_t
 * \param event pointer to event_t that was inserted
 */
static
void 
eventQueueInsertColour(eventQueue_t * const queue, event_t *event) 
{
  event_t *parent, *gparent, *uncle;

  if (queue == NULL)
    {
      return;
    }

  while (eventQueueHasParent(event) && eventQueueIsRed(event->parent))
    {
      /* parent is red so gparent isn't NULL since root is black */
      parent = event->parent;
      gparent = parent->parent;
      if (parent == gparent->left) 
        {
          uncle = gparent->right;
          if (eventQueueRightIsChild(gparent) && eventQueueIsRed(uncle)) 
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
              eventQueueSetBlack(uncle);
              eventQueueSetBlackRed(parent, gparent);
              event = gparent;
              continue;
            }
          if (eventQueueRightIsChild(parent) && (parent->right == event))
            {
              /*
               *  The parent is red but the uncle is black; also, the 
               *  new node is the right child of its parent, and the 
               *  parent in turn is the left child of its parent. In 
               *  this case, we can perform a left rotation that 
               *  switches the roles of the new node and its parent; 
               *  then, we deal with the former parent node
               */
              eventQueueRotateLeft(queue, parent);
              uncle = parent;
              parent = event;
              event = uncle;
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
          eventQueueSetBlackRed(parent, gparent); 
          eventQueueRotateRight(queue, gparent); 
        } 
      else 
        { 
          /* See comments for right-hand case */
          uncle = gparent->left; 
          if (eventQueueLeftIsChild(gparent) && eventQueueIsRed(uncle)) 
            { 
              eventQueueSetBlack(uncle);
              eventQueueSetBlackRed(parent, gparent); 
              event = gparent;
              continue; 
            } 
          if (eventQueueLeftIsChild(parent) && (parent->left == event))
            {
              eventQueueRotateRight(queue, parent); 
              uncle = parent; 
              parent = event; 
              event = uncle; 
            } 
          eventQueueSetBlackRed(parent, gparent);
          eventQueueRotateLeft(queue, gparent); 
        } 
    }


  /* 
   *  Root node is always painted black 
   */
  eventQueueSetBlack(queue->root);
}

/**
 * Private helper function for event queue implementation.
 *
 *  Fix the red-black property after a normal tree deletion.
 *
 * \param queue pointer to eventQueue_t
 * \param parent pointer to parent of event_t being deleted
 * \param event pointer to event_t replacing that being deleted
 */
static
void 
eventQueueRemoveColour(eventQueue_t * const queue, 
                       event_t *parent, 
                       event_t *event) 
{
  event_t *sibling;

  if (queue == NULL)
    {
      return;
    }

  while ((((event == NULL) || eventQueueIsBlack(event))) && 
         (event != queue->root))
    {
      if (parent->left == event) 
        {
          sibling = parent->right;
          if (sibling == NULL)
            {
              /* Should not get here! */
              fprintf(stderr, "%s:%d : sibling == NULL!", __func__, __LINE__);
              exit(-1);
            }
          if (eventQueueRightIsChild(parent) && eventQueueIsRed(sibling)) 
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
              eventQueueSetBlackRed(sibling, parent);
              eventQueueRotateLeft(queue, parent);
              sibling = parent->right;
            }

          if ((eventQueueLeftIsLeaf(sibling) || 
               eventQueueIsBlack(sibling->left))
              && 
              (eventQueueRightIsLeaf(sibling) || 
               eventQueueIsBlack(sibling->right)))
            {
              /*
               *  sibling and siblings children are black. In this
               *  case set sibling to red.
               */
              eventQueueSetRed(sibling);
              event = parent; 
              parent = event->parent;
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
              if (eventQueueRightIsLeaf(sibling) || 
                  eventQueueIsBlack(sibling->right))
                {
                  if (eventQueueLeftIsChild(sibling)) 
                    {
                      eventQueueSetBlack(sibling->left);
                    }

                  eventQueueSetRed(sibling);
                  eventQueueRotateRight(queue, sibling);
                  sibling = parent->right;
                }

              eventQueueCopyColour(sibling, parent);
              eventQueueSetBlack(parent);

              if (eventQueueRightIsChild(sibling))
                {
                  eventQueueSetBlack(sibling->right);
                }

              eventQueueRotateLeft(queue, parent);
              event = queue->root;
              break;
            }
        }
      else
        {
          /* See comments for right-hand sibling */
          sibling = parent->left;
          if (eventQueueLeftIsChild(parent) && eventQueueIsRed(sibling)) 
            {
              eventQueueSetBlackRed(sibling, parent);
              eventQueueRotateRight(queue, parent);
              sibling = parent->left;
            }

          if ((eventQueueLeftIsLeaf(sibling) || 
               eventQueueIsBlack(sibling->left))
              && 
              (eventQueueRightIsLeaf(sibling) || 
               eventQueueIsBlack(sibling->right)))
            {
              eventQueueSetRed(sibling);
              event = parent;
              parent = event->parent;
            }
          else 
            {
              if (eventQueueLeftIsLeaf(sibling) || 
                  eventQueueIsBlack(sibling->left))
                {
                  if (eventQueueRightIsChild(sibling))
                    {
                      eventQueueSetBlack(sibling->right);
                    }

                  eventQueueSetRed(sibling);
                  eventQueueRotateLeft(queue, sibling);
                  sibling = parent->left;
                }

              eventQueueCopyColour(sibling, parent);
              eventQueueSetBlack(parent);

              if (eventQueueLeftIsChild(sibling))
                {
                  eventQueueSetBlack(sibling->left);
                }

              eventQueueRotateRight(queue, parent);
              event = queue->root;
              break;
            }
        }
    }

  if (event != NULL) 
    {
      eventQueueSetBlack(event);
    }
}

/**
 * Private helper function for event queue implementation.
 *
 * Remove a node from the tree.
 *
 * \param queue pointer to eventQueue_t
 * \param event pointer to event_t to be removed
 * \return pointer to event_t found. \c NULL if not found.
 */
static
event_t *
eventQueueRemoveEvent(eventQueue_t * const queue, event_t * const event) 
{
  event_t *child, *parent, *succ;
  eventQueueColour_e colour;

  if (queue == NULL)
    {
      return NULL;
    }

  if ((succ = event) == NULL)
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
  if (eventQueueLeftIsLeaf(succ) || eventQueueRightIsLeaf(succ))
    {
      /* 
       *  Node has at most one child 
       */
      if (eventQueueLeftIsLeaf(succ))
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
      if (eventQueueHasParent(succ)) 
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
          queue->root = child;
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
      while (eventQueueLeftIsChild(succ))
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
      if (eventQueueRightIsChild(succ))
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
      if (succ->parent == event) 
        {
          parent = succ;
        }
      /* Replace everything except entry */
      succ->parent = event->parent;
      succ->left = event->left;
      succ->right = event->right;
      eventQueueCopyColour(succ, event);

      /* Fix link from parent of node */
      if (eventQueueHasParent(event)) 
        {
          if ((event->parent)->left == event) 
            {
              (event->parent)->left = succ;
            }
          else 
            {
              (event->parent)->right = succ;
            }
        }
      else 
        {
          queue->root = succ;
        }

      /* Fix parent links of children of node */
      if (eventQueueLeftIsChild(event))
        {
          (event->left)->parent = succ;
        }
      if (eventQueueRightIsChild(event))
        {
          (event->right)->parent = succ;
        }     
    }

  /*
   *  Now fix the colour. If we are deleting a red node, we 
   *  can simply replace it with its child, which must be black.
   *  If deleting a black node we need to fix the tree.
   */
  if (colour == eventQueueBlack)
    {
      eventQueueRemoveColour(queue, parent, child);
    }

  /* Done */
  queue->size = queue->size-1;
  return event;
}

/**
 * Private helper function for event queue implementation.
 *
 *  Find right-most or greatest event.
 *
 * \param queue pointer to eventQueue_t
 * \return pointer to maximum event_t found. 
 * \c NULL if not found.
 */
static
event_t *
eventQueueFindRightMost(eventQueue_t * const queue)
{
  event_t *event;

  if (queue->root == NULL)
    {
      return NULL;
    }

  event = queue->root;
  while (eventQueueRightIsChild(event))
    {
      event = event->right;
    }

  return event;
}

/**
 * Private helper function for event queue implementation.
 *
 * Search for the next smaller event (or predecessor) 
 *
 * \param event pointer to a event_t
 * \return pointer to previous event_t found. 
 * \c NULL if not found.
 */
static
event_t *
eventQueueFindPrevious(event_t * const event) 
{
  event_t *prev;

  if (event == NULL)
    {
      return NULL;
    }

  if (eventQueueLeftIsChild(event))
    {
      /* Find right-most event on left sub-tree */
      prev = event->left;
      while (eventQueueRightIsChild(prev))
        {
          prev = prev->right;
        }
    }
  else 
    {
      if (eventQueueHasParent(event) && (event == (event->parent)->right)) 
        {
          /*  
           *  For right sub-tree of events parent, 
           *  previous must be parent
           */
          prev = event->parent;
        }
      else 
        {
          /* Find parent event of left sub-trees */
          prev = event;
          while (eventQueueHasParent(prev) && 
                 (prev == (prev->parent)->left)) 
            {
              prev = prev->parent;
            }

          /* Now, go to event for which this is a right sub-tree */
          prev = prev->parent;
        }
    }

  return prev;
}

/**
 * Private helper function for event queue implementation.
 *
 * Search the event queue for an event corresponding to the given point. 
 *
 * \param queue \c eventQueue_t pointer to the event queue
 * \param point pointer to \c point_t to be found
 * \return \c event_t pointer to the event found in the queue.
 * \c NULL indicates failure to find the event.
 */
static event_t *eventQueueFindPoint(eventQueue_t * const queue,
                                    point_t * const point)
{
  event_t *event;
  compare_e comp;
  
  if (queue == NULL)
    {
      return NULL;
    }

  if (point == NULL)
    {
      queue->debug(__func__, __LINE__, "invalid point==NULL!");
      return NULL;
    }

  /* Search for node */
  event = queue->root;
  while (event != NULL) 
    {
      comp = pointCompare(point, &(event->point));
      if (comp == compareLesser) 
        {
          event = event->left;
        }
      else if (comp == compareGreater)
        {
          event = event->right;
        }
      else if (comp == compareEqual)
        {
          return event;
        }
      else
        {
          (queue->debug)(__func__, __LINE__, "Illegal compare result!");
          break;
        }
    }

  return NULL;
}

/**
 * Private helper function for event queue implementation.
 *
 * Insert the endpoints of a segment into the event queue
 *
 * \param queue \c eventQueue_t pointer
 * \param s \c segment_t pointer to segment to be inserted
 * \return \c event_t pointer to event inserted
 */
static
event_t *
eventQueueInsertSegment(eventQueue_t * const queue, segment_t * const s)
{
  point_t *a, *b;
  event_t *e;

  if (queue == NULL)
    {
      return NULL;
    }

  if (s == NULL)
    {
      queue->debug(__func__, __LINE__, "invalid s==NULL!");
      return NULL;
    }

  /* Order segment endpoints */
  if (segmentOrderSegmentEndPoints(s, &a, &b) == NULL)
    {
      (queue->debug)(__func__, __LINE__,
                 "segmentOrderSegmentEndPoints() failed!");
      return NULL;
    }

  /* Lower point */
  if (eventQueueInsertPoint(queue, a) == NULL)
    {
      (queue->debug)(__func__, __LINE__, "eventQueueInsertPoint() failed!");
      return NULL;
    }

  /* Upper point */
  if ((e = eventQueueInsertPoint(queue, b)) == NULL)
    {
      (queue->debug)(__func__, __LINE__, 
                 "eventQueueInsertPoint() failed!");
      return NULL;
    }

  /* Only add non-zero length segments to upper endpoint list */
  if (pointCompare(a, b) != compareEqual)
    {
      segmentList_t *l;

      /* Create list if necessary */
      l = eventGetUpperList(e);
      if (l == NULL)
        {
          l = segmentListCreate(queue->alloc, 
                                queue->dealloc, 
                                NULL, 
                                queue->debug);
          if (l == NULL)
            {
              (queue->debug)(__func__, __LINE__,
                         "segmentListCreate() failed!");
              return NULL;
            }
          eventSetUpperList(e, l);
        }

      /* Add to list */
      if (segmentListInsert(l, s) == NULL)
        {
          (queue->debug)(__func__, __LINE__,
                     "segmentListInsert() failed!");
          return NULL;
        }
    }
  return e;
}

point_t * 
eventGetPoint(event_t * const event)
{
  if (event == NULL)
    {
      return NULL;
    }

  return &(event->point);
}

segmentList_t *
eventGetUpperList(const event_t * const event)
{
  if (event == NULL)
    {
      return NULL;
    }

  return event->upperList;
}

event_t *
eventSetPoint(event_t * const event, point_t * const point)
{
  if ((event == NULL) || (point == NULL))
    {
      return NULL;
    }

  pointCopy(&(event->point), point);
  
  return event;
}

event_t *
eventSetUpperList(event_t * const event, segmentList_t * const list)
{
  if (event == NULL)
    {
      return NULL;
    }

  event->upperList = list;

  return event;
}

eventQueue_t *
eventQueueCreate(const eventQueueAllocFunc_t alloc, 
                 const eventQueueDeallocFunc_t dealloc, 
                 const eventQueueDebugFunc_t debug)
{
  eventQueue_t *queue = NULL;

  if (debug == NULL)
    {
      return NULL;
    }
  if (alloc == NULL)
    {
      debug(__func__, __LINE__, "Invalid alloc() function!");
      return NULL;
    }
  if (dealloc == NULL)
    {
      debug(__func__, __LINE__, "Invalid dealloc() function!");
      return NULL;
    }

  /* Allocate space for event queue */
  queue = (eventQueue_t *)alloc(sizeof(eventQueue_t));
  if (queue == NULL)
    {
      debug(__func__, __LINE__, 
            "Can't allocate %d for eventQueue_t",
            sizeof(eventQueue_t));
      return NULL;
    }

  /* Initialise queue. Tree doesn't deallocate event_t's */
  queue->alloc = alloc;
  queue->dealloc = dealloc;
  queue->debug = debug;
  queue->root = NULL;
  queue->size = 0;

  return queue;
}

event_t *
eventQueueInsertPoint(eventQueue_t * const queue, point_t * const point)
{
  event_t *event, *parent;
  compare_e comp;

  if (queue == NULL)
    {
      return NULL;
    }

  if (point == NULL)
    {
      queue->debug(__func__, __LINE__, "invalid point==NULL!");
      return NULL;
    }

   /* Search for an existing event */
  event = queue->root;
  parent = NULL;
  comp = compareEqual;
  while (event != NULL) 
    {
      parent = event;
      comp = pointCompare(point, &(parent->point));
      if (comp == compareLesser) 
        {
          event = event->left;
        }
      else if (comp == compareGreater) 
        {
          event = event->right;
        }
      else if (comp == compareEqual)
        {
          return event;
        }
      else
        {
          (queue->debug)(__func__, __LINE__, "Illegal compare result!");
          return NULL;
        }
    }

  /* Create an event for the new point */
  event = (event_t *)(queue->alloc)(sizeof(event_t));
  if (event == NULL)
    {
      return NULL;
    }
  pointCopy(&(event->point), point);
  event->upperList = NULL;
  event->parent = parent;
  event->left = event->right = NULL;
  if (parent != NULL) 
    {
      if (comp == compareLesser) 
        {
          parent->left = event;
        }
      else 
        {
          parent->right = event;
        }
    }
  else
    {
      queue->root = event;
    }

  /* Fix the red-black property */
  eventQueueSetRed(event);
  eventQueueInsertColour(queue, event);

  /* Done */
  queue->size = queue->size+1;

  return event;
}

eventQueue_t *
eventQueueInsertList(eventQueue_t * const queue, segmentList_t * const list)
{
  segmentListEntry_t *entry;
  segment_t *segment;

  if (queue == NULL)
    {
      return NULL;
    }

  if (list == NULL)
    {
      queue->debug(__func__, __LINE__, "invalid list==NULL!");
      return NULL;
    }

  /* Add event point segments */
  entry = segmentListGetFirst(list);
  while(entry != NULL)
    {
      segment = segmentListGetSegment(list, entry);
      if(eventQueueInsertSegment(queue, segment) == NULL)
        {
          (queue->debug)(__func__, __LINE__, "Can't insert segment");
          return NULL;
        }
      entry = segmentListGetNext(list, entry);
    }
  return queue;
}

void
eventQueueRemovePoint(eventQueue_t * const queue, point_t * const point)
{
  event_t *event;

  /* Sanity checks */
  if (queue == NULL)
    {
      return;
    }
  if (point == NULL)
    {
      queue->debug(__func__, __LINE__, "invalid point==NULL!");
      return;
    }
 
  event = eventQueueFindPoint(queue, point);
  if ((event = eventQueueRemoveEvent(queue, event)) != NULL)
    {
      segmentListDestroy(event->upperList);
      (queue->dealloc)(event);
    }
}

void
eventQueueDeleteEvent(eventQueue_t * const queue, event_t * const event)
{
  /* Sanity checks */
  if (queue == NULL)
    {
      return;
    }

  if (event == NULL)
    {
      queue->debug(__func__, __LINE__, "invalid event==NULL!");
      return;
    }

   /*
   * Tricky code alert!!
   *
   * Assume the event was removed from tree by eventQueueGetMax
   */
  segmentListDestroy(event->upperList);
  (queue->dealloc)(event);

  return;
}

void 
eventQueueClear(eventQueue_t * const queue)
{
  event_t *event, *prevEvent;

  if (queue == NULL)
    {
      return;
    }
  
  event = eventQueueFindRightMost(queue);
  while (event != NULL)
    {
      prevEvent = eventQueueFindPrevious(event);
      event = eventQueueRemoveEvent(queue, event);
      if (event != NULL)
        {
          segmentListDestroy(event->upperList);
          (queue->dealloc)(event);
        }
      event = prevEvent;
    }  

  return;
}

void 
eventQueueDestroy(eventQueue_t * const queue)
{
  if (queue == NULL)
    {
      return;
    }

  eventQueueClear(queue);
  (queue->dealloc)(queue);

  return;
}

bool
eventQueueIsEmpty(eventQueue_t * const queue)
{
  if (queue == NULL)
    {
      return false;
    }

  if (queue->size == 0)
    {
      return true;
    }

  return false;
}

event_t *
eventQueueGetMax(eventQueue_t * const queue)
{
  event_t *event;

  if (queue == NULL)
    {
      return NULL;
    }

  /*
   * Tricky code alert!!
   *
   * Get the "largest" event. 
   * Remove event from the tree but don't deallocate it.
   */
  event = eventQueueFindRightMost(queue);
  if (event == NULL)
    {
      return NULL;
    }
  eventQueueRemoveEvent(queue, event);

  return event;
}

void
eventQueueShow(eventQueue_t * const queue)
{
  event_t *event;
  if (queue == NULL)
    {
      return;
    }

  (queue->debug)(NULL, 0,"%d events", queue->size);
  event = eventQueueFindRightMost(queue);
  while (event != NULL)
    {
      point_t *point;
      coord_t x, y;
      real_t tol;
      segmentList_t *list;
  
      if ((point = eventGetPoint(event)) == NULL)
        {
          (queue->debug)(__func__, __LINE__, 
                         "eventGetPoint() failed!"); 
          return;
        }
  
      if (pointGetCoords(point, &x, &y) == NULL)
        {
          (queue->debug)(__func__, __LINE__,  
                         "pointGetCoords() failed!"); 
          return;
        }
  
      if (pointGetTolerance(point, &tol) == NULL)
        {
          (queue->debug)(__func__, __LINE__, 
                         "pointGetTolerance() failed!"); 
          return;
        }
  
      (queue->debug)(NULL, 0, "Event point (%g,%g) tol. %g", x, y, tol);
  
      list = eventGetUpperList(event);
      if (list != NULL)
        {
          (queue->debug)(NULL, 0, "Upper segment points:"); 
          segmentListShow(list);
        }

      event = eventQueueFindPrevious(event);
    }

  (queue->debug)(NULL, 0, "");

  return;
}
