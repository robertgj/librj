/**
 * \file  splayTree.c
 *
 * A splayTree_t implemetation.
 *
 * \e splayTreeDestroy() , \e splayTreeGetDepth() , \e splayTreeWalk() and
 * \e splayTreeCheck() recursively descend the tree from the root to 
 * avoid splaying the tree with a next operation. There are several ways
 * to avoid this recursion:\n
 *   - maintain a depth-wise stack of parent nodes. For a splay tree, this
 *     stack may need to be the same as the number of entries in the tree.\n
 *   - add a parent pointer to each node\n
 *   - use the \c NULL pointers at the leaves of the tree to thread the tree
 *     (doesn't help with \e splayTreeGetDepth() ).\n
 *
 * \todo It may be worth balancing the tree every so often.
 */

/* This is a modified version of the splay tree in FreeBSD
 * /usr/src/sys/sys/tree.h. Here is the copyright notice from that file:
 *
 *  $NetBSD: tree.h,v 1.8 2004/03/28 19:38:30 provos Exp $ 
 *  $OpenBSD: tree.h,v 1.7 2002/10/17 21:51:54 art Exp $ 
 * $FreeBSD: src/sys/sys/tree.h,v 1.1.1.4.2.1 2005/01/31 23:26:57 imp Exp $ 
 *
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
#include <stdarg.h>
#include <stdbool.h>

#include "splayTree.h" 
#include "splayTree_private.h" 

/**
 * Private helper function for splayTree_t.
 *
 * Rotate node right maintaining order.
 *
 \verbatim
       root             l
        /\     -->     /\
       l  r          ll  root
      /\                  /\
    ll  rr              rr  r
 \endverbatim
 *
 * \param tree pointer to \c splayTree_t
 */
static
void
splayTreeRotateRight(splayTree_t * const tree)
{
  splayTreeNode_t *node = (tree->root)->left;

  (tree->root)->left = node->right;
  node->right = tree->root;
  tree->root = node;
}

/**
 * Private helper function for splayTree_t.
 *
 * Rotate node left maintaining order.
 *
 \verbatim
     root                r
      /\     -->        /\
     l  r           root  rr
        /\           /\
      ll  rr        l  ll
 \endverbatim
 *
 * \param tree pointer to \c splayTree_t
 */
static
void
splayTreeRotateLeft(splayTree_t * const tree)
{
  splayTreeNode_t *node = (tree->root)->right;
  (tree->root)->right = node->left;
  node->left = tree->root;
  tree->root = node;
}

/**
 * Private helper function for splayTree_t.
 *
 * Link root to right child of node, which becomes root.
 * 
 \verbatim
 left  root  right        left      B     right
        /\          --->   /\
       A  B                  root
                              / 
                             A
 \endverbatim
 *
 * \param tree pointer to \c splayTree_t
 * \param node pointer to pointer to \c splayTreeNode_t to be moved to root
 */
static
void
splayTreeLinkRight(splayTree_t * const tree, splayTreeNode_t ** const node)
{
  (*node)->right = tree->root;
  *node = tree->root;
  tree->root = (tree->root)->right;
}

/**
 * Private helper function for splayTree_t.
 *
 * Link root to left child of node, which becomes root.
 *
 \verbatim
 left  root  right        left   A      right
        /\           --->                 /\
       A  B                           root
                                        \
                                         B
 \endverbatim
 *
 * \param tree pointer to \c splayTree_t
 * \param node pointer to pointer to \c splayTreeNode_t to be moved to root
 */
static
void
splayTreeLinkLeft(splayTree_t * const tree, splayTreeNode_t ** const node)
{
  (*node)->left = tree->root;
  *node = tree->root;
  tree->root = (tree->root)->left;
}

/**
 * Private helper function for splayTree_t.
 *
 * Attach left and right sub-trees to root.
 *
 \verbatim
 left  root  right           root
        /\                    /\
       A  B        --->   left  right
                            \     /
                             A   B
 \endverbatim
 *
 * \param tree pointer to \c splayTree_t
 * \param node pointer to \c splayTreeNode_t 
 * \param left pointer to \c splayTreeNode_t left hand tree
 * \param right pointer to \c splayTreeNode_t right hand tree
 */
static
void
splayTreeAssemble(splayTree_t * const tree, 
                  splayTreeNode_t * const node, 
                  splayTreeNode_t * const left, 
                  splayTreeNode_t * const right)
{
  left->right = (tree->root)->left;
  right->left = (tree->root)->right;
  (tree->root)->left = node->right;
  (tree->root)->right = node->left;
}

/**
 * Private helper function for splayTree_t.
 *
 * Splay the entry to the root of the tree. 
 *
 * \param tree pointer to \c splayTree_t
 * \param entry \c void pointer to caller's \c entry data
 */
static
void
splayTreeSplay(splayTree_t * const tree, const void * const entry) 
{
  splayTreeNode_t node, *left, *right;
  compare_e comp;

  (&node)->left = (&node)->right = NULL;
  left = right = &node;
  while ((comp = (tree->compare)(entry, 
                                 (tree->root)->entry,
                                 tree->user)) != compareEqual)
    {
      if (comp == compareLesser)
        {
          if ((tree->root)->left == NULL) 
            {
              break;
            }
          if ((tree->compare)(entry, 
                              ((tree->root)->left)->entry,
                              tree->user) == compareLesser)
            {
              splayTreeRotateRight(tree);
              if ((tree->root)->left == NULL)
                {
                  break;
                }
            }
          splayTreeLinkLeft(tree, &right);
        }
      else if (comp == compareGreater) 
        {
          if ((tree->root)->right == NULL) 
            {
              break;
            }
          if ((tree->compare)(entry, 
                              ((tree->root)->right)->entry,
                              tree->user) == compareGreater)
            {
              splayTreeRotateLeft(tree);
              if ((tree->root)->right == NULL) 
                {
                  break;
                }
            }
          splayTreeLinkRight(tree, &left);
        }
      else
        {
          break;
        }
    }

  splayTreeAssemble(tree, &node, left, right);
  return;
}

/**
 * Private helper function for splayTree_t.
 *
 * Follow the left or right children of the root to find the minimum or
 * maximum entry in the tree. 
 *
 * \param tree pointer to \c splayTree_t
 * \param val if \c compareLesser minimum, if \c compareGreater maximum
 * \return pointer to \c splayTree_t left- or right-most node
 */
static
splayTreeNode_t *
splayTreeFindMinMax(splayTree_t * const tree, compare_e val) 
{
  if ((val != compareLesser) && (val != compareGreater))
    {
      (tree->debug)(__func__, __LINE__, tree->user, "Invalid compare_e value!");
      return NULL;
    }

  if (tree == NULL)
    {
      return NULL;
    }

  splayTreeNode_t * node = tree->root;
  splayTreeNode_t * parent = NULL;
  while (node != NULL) 
    {
      parent = node;
      if (val == compareLesser) 
        {
          node = node->left;
        }
      else 
        {
          node = node->right;
        }
    }

  return parent;
}

/**
 * Private helper function for splayTree_t.
 *
 * Recursively find tree's depth. Doesn't do tail recursion?
 *
 * \param node pointer to node of splay tree.
 * \return depth of splay tree
 */
static
size_t
splayTreeRecurseDepth(splayTreeNode_t * const node)
{
  if (node == NULL)
    {
      return 0;
    }
  else
    {
      size_t leftDepth, rightDepth, maxDepth;

      leftDepth = splayTreeRecurseDepth(node->left);
      rightDepth = splayTreeRecurseDepth(node->right);
      if (leftDepth > rightDepth)
        {
          maxDepth = leftDepth;
        }
      else
        {
          maxDepth = rightDepth;
        }
      return (maxDepth+1);
    }
}

/**
 * Private helper function for splayTree_t.
 *
 * Recursive walk of tree nodes. Bails out at the first failure. 
 * Doesn't do tail recursion?
 *
 * \param tree pointer to \c splayTree_t
 * \param node pointer to node of splay tree.
 * \param walk function to apply to each entry
 * \return \c bool indicating success
 */
static
bool
splayTreeRecurseWalk(splayTree_t * const tree, 
                     splayTreeNode_t * const node, 
                     const splayTreeWalkFunc_t walk)
{
  if ((node->left != NULL) && 
      (splayTreeRecurseWalk(tree, node->left, walk) == false))
    {
      return false;
    }

  if ((node->right != NULL) && 
      (splayTreeRecurseWalk(tree, node->right, walk) == false))
    {
      return false;
    }

  return walk(node->entry, tree->user);
}

/**
 * Private helper function for splayTree_t.
 *
 * Recursively check node order. Doesn't do tail recursion?
 *
 * \param tree pointer to \c splayTree_t
 * \param node pointer to node of splay tree.
 * \return \c bool indicating success
 */
static
bool
splayTreeRecurseCheck(splayTree_t * const tree,     
                      const splayTreeNode_t * const node)
{
  /* Check left child */
  if (node->left != NULL)
    {
      if ((tree->compare)(node->left->entry, 
                          node->entry,
                          tree->user) != compareLesser)
        {
          return false;
        }
      if (splayTreeRecurseCheck(tree, node->left) == false)
        {
          return false;
        }
    }

  /* Check right child */
  if (node->right != NULL)
    {
      if ((tree->compare)(node->entry, 
                          node->right->entry,
                          tree->user) != compareLesser)
        {
          return false;
        }
      if (splayTreeRecurseCheck(tree, node->right) == false)
        {
          return false;
        }
    }

  return true;
}

/**
 * Private helper function for splayTree_t.
 *
 * Recursively clear nodes from the tree. Doesn't do tail recursion?
 *
 * \param tree pointer to \c splayTree_t
 * \param node pointer to node of splay tree.
 */
static
void
splayTreeRecurseClear(splayTree_t * const tree, splayTreeNode_t * const node)
{
  /* Destroy left child */
  if (node->left != NULL)
    {
      splayTreeRecurseClear(tree, node->left);
    }

  /* Destroy right child */
  if (node->right != NULL)
    {
      splayTreeRecurseClear(tree, node->right);
    }

  /* Delete entry */
  if ((tree->deleteEntry != NULL) && (node->entry != NULL))
    {
      (tree->deleteEntry)(node->entry, tree->user);
    }

  /* Deallocate node */
  (tree->dealloc)(node, tree->user);
  tree->size = tree->size - 1;

  return;
}

/**
 * Private helper function for splayTree implementation.
 *
 *  Duplicate an entry.
 *
 * \param tree pointer to splayTree_t
 * \param entry pointer to caller's data 
 * \return pointer to duplicated entry
 */
static
void *
splayTreeDuplicateEntry(splayTree_t * const tree, void * const entry)
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

splayTree_t *
splayTreeCreate(const splayTreeAllocFunc_t alloc, 
                const splayTreeDeallocFunc_t dealloc,
                const splayTreeDuplicateEntryFunc_t duplicateEntry,
                const splayTreeDeleteEntryFunc_t deleteEntry,
                const splayTreeDebugFunc_t debug, 
                const splayTreeCompFunc_t comp,
                void * const user)
{
  splayTree_t *tree = NULL;

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

  /* Allocate space for splayTree_t */
  tree = alloc(sizeof(splayTree_t), user);
  if (tree == NULL)
    {
      debug(__func__, __LINE__, user,
            "Can't allocate %d for splayTree_t",
            sizeof(splayTree_t));
      return NULL;
    }

  /* Initialise new tree */
  tree->alloc = alloc;
  tree->dealloc = dealloc;
  tree->duplicateEntry = duplicateEntry;
  tree->deleteEntry = deleteEntry;
  tree->debug = debug;
  tree->compare = comp;
  tree->root = NULL;
  tree->size = 0;
  tree->user = user;

  return tree;
}

void * 
splayTreeFind(splayTree_t * const tree, void * const entry) 
{ 
  if ((tree == NULL) || (tree->root == NULL) || (entry == NULL))
    {
      return NULL;
    }

  /* Splay entry to root */
  splayTreeSplay(tree, entry);

  /* Test root */
  if ((tree->compare)(entry, (tree->root)->entry, tree->user) == compareEqual) 
    {
      return (tree->root)->entry;
    }

  return NULL;
}

void *
splayTreeInsert(splayTree_t * const tree, void * const entry) 
{
  splayTreeNode_t *node;

  if ((tree == NULL) || (tree->alloc == NULL) || (entry == NULL))
    {
      return NULL;
    }

  if(tree->root == NULL)
    {
      if ((node = (tree->alloc)(sizeof(splayTreeNode_t), tree->user)) == NULL)
        {
          return NULL;
        }
      node->left = node->right = NULL;
    }
  else 
    {
      compare_e comp;

      /* Splay entry to root */
      splayTreeSplay(tree, entry);

      /* Insert entry at appropriate child */
      comp = (tree->compare)(entry, (tree->root)->entry, tree->user);
      if(comp == compareLesser) 
        {
          /*
           *      Root               Node
           *       /\         -->     /\
           *      L  R               L  Root
           *                             /\
           *                         NULL  R
           */
          if ((node = (tree->alloc)(sizeof(splayTreeNode_t), 
                                    tree->user)) == NULL)
            {
              return NULL;
            }
          node->left = (tree->root)->left;
          node->right = tree->root;
          (tree->root)->left = NULL;
        }
      else if (comp == compareGreater) 
        {
          /*
           *      Root                 Node
           *       /\         -->       /\
           *      L  R              Root  R
           *                         /\
           *                        L  NULL
           */
          if ((node = (tree->alloc)(sizeof(splayTreeNode_t),
                                    tree->user)) == NULL)
            {
              return NULL;
            }
          node->right = (tree->root)->right;
          node->left = tree->root;
          (tree->root)->right = NULL;
        }
      else 
        {
          /* Replace entry */
          if (tree->deleteEntry != NULL)
            {
              (tree->deleteEntry)((tree->root)->entry, tree->user);
            }
          (tree->root)->entry = splayTreeDuplicateEntry(tree, entry);
          if ((tree->root)->entry == NULL)
            {
              (tree->debug)(__func__, __LINE__, tree->user,
                            "splayTreeDuplicateEntry() failed!");
              return NULL;
            }
          return (tree->root)->entry;
        }
    }

  /* Housekeeping */
  tree->root = node;
  (tree->root)->entry = splayTreeDuplicateEntry(tree, entry);
  if ((tree->root)->entry == NULL)
    {
      (tree->dealloc)(node, tree->user);
      (tree->debug)(__func__, __LINE__, tree->user,
                    "splayTreeDuplicateEntry() failed!");
      return NULL;
    }
  tree->size = tree->size+1;

  return (tree->root)->entry;
}

void * 
splayTreeRemove(splayTree_t * const tree, void * const entry) 
{
  if ((tree == NULL) || (tree->dealloc == NULL) || (entry == NULL))
    {
      return NULL;
    }
  if ((tree->root == NULL)) 
    {
      return NULL;
    }

  splayTreeSplay(tree, entry);
  if ((tree->compare)(entry, (tree->root)->entry, tree->user) == compareEqual) 
    {
      splayTreeNode_t *node;

      node = tree->root;
      if ((tree->root)->left == NULL) 
        {
          tree->root = (tree->root)->right;
        }
      else 
        {
          splayTreeNode_t *tmp;

          tmp = (tree->root)->right;
          tree->root = (tree->root)->left;
          splayTreeSplay(tree, entry);
          (tree->root)->right = tmp;
        }

      /* Delete entry */
      if (tree->deleteEntry != NULL)
        {
          (tree->deleteEntry)(node->entry, tree->user);

          /* Deallocate node */
          (tree->dealloc)(node, tree->user);
          tree->size = tree->size-1;

          return NULL;
        }
      else
        {
          /* Deallocate node */
          (tree->dealloc)(node, tree->user);
          tree->size = tree->size-1;
      
          return entry;
        }
    }

  return NULL;
}

void 
splayTreeClear(splayTree_t * const tree)
{
  if ((tree == NULL) || (tree->dealloc == NULL))
    {
      return;
    }

  if (tree->root != NULL)
    {
      splayTreeRecurseClear(tree, tree->root);
    }
  tree->root = NULL;

  return;
}

void 
splayTreeDestroy(splayTree_t * const tree)
{
  if ((tree == NULL) || (tree->dealloc == NULL))
    {
      return;
    }

  splayTreeClear(tree);
  (tree->dealloc)(tree, tree->user);

  return;
}

size_t
splayTreeGetDepth(const splayTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  return splayTreeRecurseDepth(tree->root);
}

size_t
splayTreeGetSize(const splayTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  return tree->size;
}

void *
splayTreeGetNext(splayTree_t * const tree, const void * const entry) 
{
  splayTreeNode_t *node;

  if ((tree == NULL) || (tree->root == NULL) || (entry == NULL))
    {
      return NULL;
    }

  splayTreeSplay(tree, entry);
  node = tree->root;
  if (node->right != NULL) 
    {
      node = node->right;
      while (node->left != NULL) 
        {
          node = node->left;
        }
    }
  else 
    {
      return NULL;
    }

  return node->entry;
}

void *
splayTreeGetPrevious(splayTree_t * const tree, const void * const entry) 
{
  splayTreeNode_t *node;

  if ((tree == NULL) || (tree->root == NULL) || (entry == NULL))
    {
      return NULL;
    }

  splayTreeSplay(tree, entry);
  node = tree->root;
  if (node->left != NULL) 
    {
      node = node->left;
      while (node->right != NULL) 
        {
          node = node->right;
        }
    }
  else 
    {
      return NULL;
    }

  return node->entry;
}


void * 
splayTreeGetMin(splayTree_t * const tree) 
{
  splayTreeNode_t *node;

  if (tree == NULL)
    {
      return NULL;
    }
  
  node = splayTreeFindMinMax(tree, compareLesser);

  if (node == NULL)
    {
      return NULL;
    }

  return node->entry;
}

void * 
splayTreeGetMax(splayTree_t * const tree) 
{
  splayTreeNode_t *node;

  if (tree == NULL)
    {
      return NULL;
    }
  
  node = splayTreeFindMinMax(tree, compareGreater);

  if (node == NULL)
    {
      return NULL;
    }

  return node->entry;
}

bool
splayTreeWalk(splayTree_t * const tree, const splayTreeWalkFunc_t walk)
{
  if ((tree == NULL) || (walk == NULL) || (tree->root == NULL))
    {
      return false;
    }

  return splayTreeRecurseWalk(tree, tree->root, walk);
}

bool
splayTreeCheck(splayTree_t * const tree)
{
  if ((tree == NULL) || (tree->compare == NULL) || (tree->root == NULL))
    {
      return false;
    }

  return splayTreeRecurseCheck(tree, tree->root);
}

bool
splayTreeBalance(splayTree_t * const tree)
{
  splayTreeNode_t *node, **parentp;

  if ((tree == NULL) || (tree->root == NULL))
    {
      return false;
    }

  /* First convert to a list */
  node = tree->root;
  parentp = &(tree->root);
  while (node != NULL)
    {
      if (node->right != NULL)
        {
          splayTreeNode_t *right;

          /* Rotate left */
          right = node->right;
          node->right = right->left;
          right->left = node;
          node = right;
        }

      else
        {     
          *parentp = node;
          parentp = &(node->left);
          node = node->left;
        }
    }

  /* Check */
  node = tree->root;
  while (node != NULL)
    {
      if (node->right != NULL)
        {
          tree->debug(__func__, __LINE__, tree->user, "node->right!=NULL");
          return false;
        }
      else 
        {
          node = node->left;
        }
    }

  /* Now balance the tree */
  if (tree->size <= 2)
    {
      return true;
    }

  for (size_t depth=tree->size; depth>1; depth=depth/2)
    {
      splayTreeNode_t *now, *next;

      /* Move alternate nodes to the right child of their left child */
      now = tree->root;
      tree->root = now->left;
      next = now->left;
      if (next == NULL)
        {
          tree->debug(__func__, __LINE__, tree->user, 
                      "next==NULL at depth=%zd", depth);
          return false;
        }
      now->left = next->right; 
      next->right = now; 
      for (size_t i = 1; i<depth/2; i++)
        {
          now = next->left;
          if (now == NULL)
            {
              tree->debug(__func__, __LINE__, tree->user, 
                          "now==NULL at depth=%zd", depth);
              return false;
            }
          next->left = now->left;
          next = next->left;
          if (next == NULL)
            {
              tree->debug(__func__, __LINE__, tree->user, 
                          "next==NULL at depth=%zd", depth);
              return false;
            }
          now->left = next->right;
          next->right = now;
        }
    }
  return true;
}
