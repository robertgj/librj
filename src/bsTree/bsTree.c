/**
 * \file bsTree.c
 *
 * Implementation of a balanced search tree type.
 *
 * See: "Balanced Search Trees Made Simple", Arne Andersson, Proc. Workshop
 * on Algorithms and Data Structures, pages 60-71. 1993
 */


#include <stdlib.h>
#include <stdbool.h>

#include "compare.h"
#include "bsTree.h"
#include "bsTree_private.h"

/**
 * Private helper function for balanced search tree implementation.
 *
 *  Rotate node to the right. The levels of q and n do not change.
 *
 *      n                  q
 *     /                  / \
 *    q          ->      l   n
 *   / \                    /
 *  l   r                  r 
 *
 * \param node pointer to pointer to bsTreeNode_t
 */
static
void
bsTreeSkew(bsTreeNode_t **node)
{
  if ((*node)->left->level == (*node)->level)
    {
      bsTreeNode_t *tmp;
      /* Rotate right */
      tmp = (*node);
      *node = (*node)->left;
      tmp->left = (*node)->right;
      (*node)->right = tmp;
    }
}

/**
 * Private helper function for balanced search tree implementation.
 *
 *  Rotate node to the left. The level of r increases by 1.
 *
 *      n                r
 *       \              /
 *        r     ->     n
 *       /              \
 *      l                l
 *
 * \param node pointer to pointer to bsTreeNode_t
 */
static
void
bsTreeSplit(bsTreeNode_t **node)
{
  if ((*node)->right->right->level == (*node)->level)
    {
      bsTreeNode_t *tmp;
      /* Rotate left */
      tmp = *node;
      *node = (*node)->right;
      tmp->right = (*node)->left;
      (*node)->left = tmp;
      (*node)->level = ((*node)->level) + 1;
    }
}

/**
 * Private helper function for balanced search tree implementation.
 *
 *  Duplicate an entry.
 *
 * \param tree pointer to bsTree_t
 * \param entry pointer to caller's data 
 * \return pointer to duplicated entry
 */
static
void *
bsTreeDuplicateEntry(bsTree_t * const tree, void * const entry)
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

/**
 * Private helper function for balance search tree implementation.
 *
 * Insert a node into the tree.
 *
 * \param tree pointer to bsTree_t
 * \param node pointer to bsTreeNode_t of the tree
 * \param entry to be inserted into the tree
 * \param ok pointer to flag operation was successful
 * \return pointer \c NULL if failed.
 */
static
void *
bsTreeRecurseInsertNode(bsTree_t * const tree,
                        bsTreeNode_t **node,
                        void * const entry,
                        bool * ok)
{
  if ((tree == NULL) || (node == NULL) || ((*node) == NULL) || (entry == NULL))
    {
      return NULL;
    }

  void *ptr = NULL;
  
  if ((*node) == tree->bottom)
    {
      (*node) = (bsTreeNode_t *)(tree->alloc(sizeof(bsTreeNode_t), tree->user));
      if ((*node) == NULL)
        {
          tree->debug(__func__, __LINE__, tree->user, 
                      "Can't allocate %d for bsTreeNode_t",
                      sizeof(bsTreeNode_t));
          return NULL;
        }

      /* Copy the new entry */
      (*node)->entry = bsTreeDuplicateEntry(tree, entry);
      if ((*node)->entry == NULL)
        {
          (tree->dealloc)(*node, tree->user);
          (tree->debug)(__func__, __LINE__, tree->user,
                        "redblackTreeDuplicateEntry() failed!");
          return NULL;
        }
      (*node)->left = tree->bottom;
      (*node)->right = tree->bottom; 
      (*node)->level = 1;
      *ok = true;
      
      tree->size = tree->size + 1;
      
      ptr = (*node)->entry;
    }
  else
    {
       compare_e comp = (tree->compare)(entry, (*node)->entry, tree->user);
      if (comp == compareLesser) 
        {
          ptr = bsTreeRecurseInsertNode(tree, &((*node)->left), entry, ok);
        }
      else if (comp == compareGreater)
        {
          ptr = bsTreeRecurseInsertNode(tree, &((*node)->right), entry, ok);
        }
      else if (comp == compareEqual)
        {
          /* Replace the existing entry */
          if (tree->deleteEntry != NULL)
            {
              (tree->deleteEntry)((*node)->entry, tree->user);
            }
          (*node)->entry = bsTreeDuplicateEntry(tree, entry);
          if ((*node)->entry == NULL)
            {
              (tree->debug)(__func__, __LINE__, tree->user,
                            "bsTreeDuplicateEntry() failed!");
              return NULL;
            }
          *ok = true;
          ptr = (*node)->entry;
        }
      else
        {
          *ok = false;
          (tree->debug)(__func__, __LINE__, tree->user,
                        "Illegal compare result!");
          return NULL;
        }

      /* Recursively re-balance the tree */
      bsTreeSkew(node);
      bsTreeSplit(node);
    }
  
  return ptr;
}

/**
 * Private helper function for balance search tree implementation.
 *
 * Remove a node from the tree. From Andersen, Section 3:
 *
 *  "In order to handle deletion of internal nodes without a lot of code, we
 *   use two global pointers, \e deleted and \e last. These pointers are set
 *   during the top-down traversal in the following simple manner: At each
 *   node we make a binary comparison, if the key to be deleted is less than
 *   the node's value we turn left, otherwise we turn right (i.e. even if the
 *   searched element is present in the node we turn right). We let \e last
 *   point to each internal node on the path, and we let \e deleted point
 *   to each node where we turn right. When we reach the bottom of the tree,
 *   \e deleted will point to the node containing the element to be deleted
 *   (if it is present in the tree) and \e last will point to the node which
 *   is to be removed (which may be the same node). Then, we just move the
 *   element from \e last to \e deleted and remove \e last.
 *
 * \param tree pointer to bsTree_t
 * \param node pointer to bsTreeNode_t of the tree
 * \param entry to be found and removed from the tree
 * \param ok pointer to flag operation was successful
 * \return pointer to entry found and removed. \c NULL if not found.
 */
static
void *
bsTreeRecurseRemoveNode(bsTree_t * const tree,
                        bsTreeNode_t **node,
                        void * const entry,
                        bool * ok)
{
  if ((tree == NULL) || (node == NULL) || ((*node) == NULL) || (entry == NULL))
    {
      return NULL;
    }

  *ok = false;
  void *ptr = NULL;
  compare_e comp;

  if ((*node) != tree->bottom)
    {
      /* 1. Search down the tree and set pointers last and deleted
       *    If (*node)->entry == entry, then, for an internal node,
       *    the order is (*node)->right->left->...->left and tree->last
       *    will be the successor to tree->deleted. Replace the entry
       *    of tree->deleted with that for tree->last and update the
       *    left or right child of the parent of tree->last.
       */
      tree->last = (*node);
      comp = (tree->compare)(entry, (*node)->entry, tree->user);
      if (comp == compareError)
        {
          (tree->debug)(__func__, __LINE__, tree->user,
                        "Illegal compare result!");
          return NULL;
        }
      else if (comp == compareLesser) 
        {
          ptr = bsTreeRecurseRemoveNode(tree, &((*node)->left), entry, ok);
        }
      else
        {
          tree->deleted = (*node);
          ptr = bsTreeRecurseRemoveNode(tree, &((*node)->right), entry, ok);
        }
      /* 2. At the bottom of the tree we remove the element (if it is present) */
      if (((*node) == tree->last) && (tree->deleted != tree->bottom))
        {  
          ptr = tree->deleted->entry;
          tree->deleted->entry = (*node)->entry;
          tree->deleted = tree->bottom;
          *node = (*node)->right;
          
          if(tree->deleteEntry != NULL)
            {
              (tree->deleteEntry)(ptr, tree->user);
            }
          tree->dealloc(tree->last, tree->user);
          tree->last = NULL;

          tree->size = tree->size - 1;
          *ok = true;
        }
      /* 3. On the way back, we rebalance the tree */      
      else if (((*node)->left->level < (((*node)->level) - 1))
               || ((*node)->right->level < (((*node)->level) - 1)))
        {
          (*node)->level = ((*node)->level) - 1;
          if ((*node)->right->level > (*node)->level)
            {
              (*node)->right->level = (*node)->level;
            }
          bsTreeSkew(node);
          bsTreeSkew(&((*node)->right));
          bsTreeSkew(&((*node)->right->right));
          bsTreeSplit(node);
          bsTreeSplit(&((*node)->right));
        }
    }
  
  return ptr;
}
  
/**
 * Private helper function for bsTree_t.
 *
 * Follow the left or right children of the root to find the minimum or
 * maximum entry in the tree. 
 *
 * \param tree pointer to \c bsTree_t
 * \param val if \c compareLesser minimum, if \c compareGreater maximum
 * \return pointer to \c bsTree_t left- or right-most node
 */
static
bsTreeNode_t *
bsTreeFindMinMax(bsTree_t * const tree, compare_e val) 
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

  bsTreeNode_t * node = tree->root;
  bsTreeNode_t * parent = NULL;
  while (node != tree->bottom) 
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
 * Private helper function for bsTree_t.
 *
 * Recursively find tree's depth. Doesn't do tail recursion?
 *
 * \param tree pointer to \c bsTree_t
 * \param node pointer to node of bsTree_t
 * \return depth of bs tree
 */
static
size_t
bsTreeRecurseDepth(bsTree_t const * tree, bsTreeNode_t * const node)
{
  if (node == tree->bottom)
    {
      return 0;
    }
  else
    {
      size_t leftDepth, rightDepth, maxDepth;

      leftDepth = bsTreeRecurseDepth(tree, node->left);
      rightDepth = bsTreeRecurseDepth(tree, node->right);

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
 * Private helper function for bsTree_t.
 *
 * Recursive walk of tree nodes. Bails out at the first failure. 
 * Doesn't do tail recursion?
 *
 * \param tree pointer to \c bsTree_t
 * \param node pointer to node of bs tree.
 * \param walk function to apply to each entry
 * \return \c bool indicating success
 */
static
bool
bsTreeRecurseWalk(bsTree_t * const tree, 
                     bsTreeNode_t * const node, 
                     const bsTreeWalkFunc_t walk)
{
  if ((node->left != tree->bottom) && 
      (bsTreeRecurseWalk(tree, node->left, walk) == false))
    {
      return false;
    }

  bool val = false;
  if (node != tree->bottom)
    {
      val = walk(node->entry, tree->user);
    }
  
  if ((node->right != tree->bottom) && 
      (bsTreeRecurseWalk(tree, node->right, walk) == false))
    {
      return false;
    }

  return val;
}

/**
 * Private helper function for bsTree_t.
 *
 * Recursively find size of sub-tree at node.
 *
 * \param tree pointer to \c bsTree_t
 * \param node pointer to node of balanced search tree.
 * \return size
 */
static
size_t
bsTreeRecurseSize(bsTree_t * const tree,     
                  const bsTreeNode_t * const node)
{
  if (node == tree->bottom)
    {
      return 0;
    }

  return
    bsTreeRecurseSize(tree, node->left)
    + bsTreeRecurseSize(tree, node->right)
    + 1;
}

/**
 * Private helper function for bsTree_t.
 *
 * Recursively check node order. Doesn't do tail recursion?
 *
 * \param tree pointer to \c bsTree_t
 * \param node pointer to node of bs tree.
 * \return \c bool indicating success
 */
static
bool
bsTreeRecurseCheck(bsTree_t * const tree,     
                   const bsTreeNode_t * const node)
{
  /* Check left child */
  if (node->left != tree->bottom)
    {
      if (node->level < node->left->level)
        {
          (tree->debug)(__func__, __LINE__, tree->user,
                        "node->left->level inconsistent!");
          return false;
        }
      if ((tree->compare)(node->entry, 
                          node->left->entry,
                          tree->user) != compareGreater)
        {
          return false;
        }
      if (bsTreeRecurseCheck(tree, node->left) == false)
        {
          return false;
        }
    }

  /* Check right child */
  if (node->right != tree->bottom)
    {
      if (node->level < node->right->level)
        {
          (tree->debug)(__func__, __LINE__, tree->user,
                        "node->right->level inconsistent!");
          return false;
        }
      if ((tree->compare)(node->entry, 
                          node->right->entry,
                          tree->user) != compareLesser)
        {
          return false;
        }
      if (bsTreeRecurseCheck(tree, node->right) == false)
        {
          return false;
        }
    }

  return true;
}

/**
 * Private helper function for bsTree_t.
 *
 * Recursively clear nodes from the tree. Doesn't do tail recursion?
 *
 * \param tree pointer to \c bsTree_t
 * \param node pointer to node of bs tree.
 */
static
void
bsTreeRecurseClear(bsTree_t * const tree, bsTreeNode_t * const node)
{
  /* Destroy left child */
  if (node->left != tree->bottom)
    {
      bsTreeRecurseClear(tree, node->left);
    }

  /* Destroy right child */
  if (node->right != tree->bottom)
    {
      bsTreeRecurseClear(tree, node->right);
    }

  /* Delete entry */
  if ((tree->deleteEntry != NULL) && (node->entry != NULL))
    {
      (tree->deleteEntry)(node->entry, tree->user);
    }

  /* Deallocate node */
  if (tree->dealloc != NULL)
    {
      (tree->dealloc)(node, tree->user);
    }
  tree->size = tree->size - 1;

  return;
}

/**
 * Private helper function for balanced search tree implementation.
 *
 *  Given an entry find the corresponding node.
 *
 * \param tree pointer to bsTree_t
 * \param entry pointer to caller's data 
 * \return pointer to bsTreeNode_t found. 
 * \c NULL if not found.
 */
static
bsTreeNode_t *
bsTreeFindNode(bsTree_t * const tree, const void * const entry)
{
  bsTreeNode_t *node;
  compare_e comp;
  
  if ((tree == NULL) || (tree->root == tree->bottom) || (entry == NULL))
    {
      return NULL;
    }

  /* Search for node */
  node = tree->root;
  while (node != tree->bottom) 
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

bsTree_t *bsTreeCreate
(const bsTreeAllocFunc_t alloc, 
 const bsTreeDeallocFunc_t dealloc,
 const bsTreeDuplicateEntryFunc_t duplicateEntry,
 const bsTreeDeleteEntryFunc_t deleteEntry,
 const bsTreeDebugFunc_t debug,
 const bsTreeCompFunc_t comp,
 void * const user)
{
  bsTree_t *tree; 

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
  
  tree = alloc(sizeof(bsTree_t), user);
  if (tree == NULL)
    {
      debug(__func__, __LINE__, user, 
            "Can't allocate %d for bsTree_t",
            sizeof(bsTree_t));
      return NULL;
    }
  
  tree->bottom = alloc(sizeof(bsTreeNode_t), user);
  if (tree->bottom == NULL)
    {
      debug(__func__, __LINE__, user, 
            "Can't allocate %d for bsTreeNode_t",
            sizeof(bsTreeNode_t));
      dealloc(tree,user);
      return NULL;
    }

  tree->compare = comp;
  tree->alloc = alloc;
  tree->dealloc = dealloc;
  tree->duplicateEntry = duplicateEntry;
  tree->deleteEntry = deleteEntry;
  tree->debug = debug; 
  tree->root = tree->bottom; 
  tree->size = 0;
  tree->user = user;

  tree->bottom->level = 0;
  tree->bottom->left = tree->bottom;
  tree->bottom->right = tree->bottom;
  tree->bottom->entry = NULL;
  tree->deleted = tree->bottom;
  tree->last = NULL;

  return tree;
}

void *bsTreeFind(bsTree_t * const tree, void * const entry)
{
  bsTreeNode_t *node;

  if (tree == NULL)
    {
      return NULL;
    }
  if (entry == NULL)
    {
      return NULL;
    }

  node = bsTreeFindNode(tree, entry);
  if (node == NULL)
    {
      return NULL;
    }
  else
    {
      return node->entry;
    }
}

void *bsTreeInsert(bsTree_t * const tree, void * const entry)
{
  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  bool ok = false;
  void *new_entry = bsTreeRecurseInsertNode(tree, &(tree->root), entry, &ok);

  return new_entry;
}

void *
bsTreeRemove(bsTree_t * const tree, void * const entry) 
{
  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  /* Remove node from tree */
  tree->deleted = tree->bottom;
  tree->last = NULL;
  bool ok = false;
  void *oldEntry = bsTreeRecurseRemoveNode(tree, &(tree->root), entry, &ok);
  if (oldEntry  == NULL)
    {
      return entry;
    }

  return oldEntry;
}

void bsTreeClear(bsTree_t * const tree)
{
  if ((tree == NULL) || (tree->root == tree->bottom))
    {
      return;
    }

  bsTreeRecurseClear(tree, tree->root);
  tree->root = tree->bottom; 
  return;
}

void bsTreeDestroy(bsTree_t * const tree)
{
  if ((tree == NULL) || (tree->dealloc == NULL))
    {
      return;
    }

  /* Free all the nodes in the tree */
  bsTreeClear(tree);
  
  /* Free sentinel */
  tree->dealloc(tree->bottom, tree->user);

  /* Free the tree */
  tree->dealloc(tree, tree->user);
}

#include <stdio.h>

size_t bsTreeGetDepth(const bsTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  size_t depth = bsTreeRecurseDepth(tree, tree->root);
  return depth;
}

size_t bsTreeGetSize(const bsTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  return tree->size;
}

void *bsTreeGetMin(bsTree_t * const tree)
{
  if (tree == NULL)
    {
      return NULL;
    }
  
  bsTreeNode_t *node = bsTreeFindMinMax(tree, compareLesser);

  if (node == NULL)
    {
      return NULL;
    }

  return node->entry;
}

void *bsTreeGetMax(bsTree_t * const tree)
{
  if (tree == NULL)
    {
      return NULL;
    }
  
  bsTreeNode_t *node = bsTreeFindMinMax(tree, compareGreater);

  if (node == NULL)
    {
      return NULL;
    }

  return node->entry;
}

bool bsTreeWalk(bsTree_t * const tree, const bsTreeWalkFunc_t walk)
{
  if ((tree == NULL) || (walk == NULL))
    {
      return false;
    }

  return bsTreeRecurseWalk(tree, tree->root, walk);
}

bool bsTreeCheck(bsTree_t * const tree)
{
  if ((tree == NULL) || (tree->compare == NULL))
    {
      return false;
    }

  size_t size = bsTreeRecurseSize(tree, tree->root);
  if (size != tree->size)
    {
      tree->debug(__func__, __LINE__, tree->user, "size mismatch(%u != %u)!",
                  size, tree->size);
      return false;
    }
 
  return bsTreeRecurseCheck(tree, tree->root);
}
