/**
 * \file swTree.c
 *
 * Implementation of Stout and Warren rebalancing of a binary tree type. The
 * whole tree is rebalanced when the depth of an inserted or deleted node
 * exceeds depth_factor+floor(lg(size+1)).
 *
 */

#include <stdlib.h>
#include <stdbool.h>

#include "compare.h"
#include "swTree.h"
#include "swTree_private.h"
#include "swTree_lg.h"

/**
 * Private helper function for Stout/Warren tree implementation.
 *
 * Check if the depth requires that the tree be rebalanced
 *
 * \param tree pointer to swTree_t
 * \param depth of tree
 * \return depth requires that the tree be rebalanced
 */
static
bool
swTreeCheckDepth(swTree_t * const tree, const size_t depth)
{
  return ((tree->size > 0)
          && (tree->depth_factor > 0)
          && (depth > (tree->depth_factor+swTreeFloorLg(tree->size))));
}

/**
 * Private helper function for Stout/Warren tree implementation.
 *
 * Convert the tree with pseudo_root *root into a vine with
 * pseudo_root node *root and store the number of nodes in *size.
 *
 * \param root pointer to root swTreeNode_t
 * \return size of tree
 */
static
size_t
swTreeTreeToVine(swTreeNode_t * const root)
{
  size_t size = 0;
  swTreeNode_t *vine_tail = root;
  swTreeNode_t *remainder = vine_tail->right;

  while (remainder != NULL)
    {
      if (remainder ->left == NULL)
        {
          /* Move vine_tail down one */
          vine_tail = remainder;
          remainder = remainder->right;
          size = size + 1;
        }
      else
        {
          /* Rotate */
          swTreeNode_t *tempptr = remainder->left;
          remainder->left = tempptr->right;
          tempptr->right = remainder;
          remainder = tempptr;
          vine_tail->right = tempptr;
        }
    }

  return size;
}

/**
 * Private helper function for Stout/Warren tree implementation.
 *
 * Compress count spine nodes in the tree with root
 *
 * \param root pointer to the root of the swTreeNode_t
 * \param count number of "spine" nodes in the tree at root
 */
static
void
swTreeCompression(swTreeNode_t * const root, size_t count)
{
  swTreeNode_t * scanner = root;
  for (size_t i=0; i < count; i++)
    {
      swTreeNode_t * child = scanner->right;
      scanner->right = child->right;
      scanner = scanner->right;
      child->right = scanner->left;
      scanner->left = child;
    }
}
  
/**
 * Private helper function for Stout/Warren tree implementation.
 *
 * Convert the vine to a tree
 *
 * \param root pointer to root swTreeNode_t
 * \param size of tree
 */
static
void
swTreeVineToTree(swTreeNode_t * const root, size_t size)
{
  size_t leaf_count = size+1-swTreeTwoFloorLg(size);
  swTreeCompression(root, leaf_count);
  size=size-leaf_count;
  while(size>1)
    {
      swTreeCompression(root,size/2);
      size=size/2;
    }
}

/**
 * Private helper function for Stout/Warren tree implementation.
 *
 * Rebalance the sub-tree starting at *node. First convert the sub-tree
 * to a "vine" or singly-linked-list and then to a balanced tree. This
 * process is claimed to be linear in time in the number of nodes and require
 * constant space additional to the number of nodes.
 *
 * \param node pointer to pointer to swTreeNode_t
 */
static
void
swTreeRebalance(swTreeNode_t **node)
{
  size_t size;
  swTreeNode_t pseudo_root;

  if ((node == NULL) || (*node == NULL))
    {
      return;
    }
  pseudo_root.right = *node;
  size = swTreeTreeToVine(&pseudo_root);
  swTreeVineToTree(&pseudo_root,size);
  *node = pseudo_root.right;
}

/**
 * Private helper function for Stout/Warren tree implementation.
 *
 *  Duplicate an entry.
 *
 * \param tree pointer to swTree_t
 * \param entry pointer to caller's data 
 * \return pointer to duplicated entry
 */
static
void *
swTreeDuplicateEntry(swTree_t * const tree, void * const entry)
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
 * Private helper function for swTree_t.
 *
 * Follow the left or right children of the root to find the minimum or
 * maximum entry in the tree. 
 *
 * \param tree pointer to \c swTree_t
 * \param val if \c compareLesser minimum, if \c compareGreater maximum
 * \return pointer to \c swTree_t left- or right-most node
 */
static
swTreeNode_t *
swTreeFindMinMax(swTree_t * const tree, compare_e val) 
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

  swTreeNode_t * node = tree->root;
  swTreeNode_t * parent = NULL;
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
 * Private helper function for swTree_t.
 *
 * Recursively find tree's depth. Doesn't do tail recursion?
 *
 * \param tree pointer to \c swTree_t
 * \param node pointer to node of swTree_t
 * \return depth of sw tree
 */
static
size_t
swTreeRecurseDepth(swTree_t const * tree, swTreeNode_t * const node)
{
  size_t depth = 0;
  
  if (node != NULL)
    {
      size_t leftDepth = swTreeRecurseDepth(tree, node->left);
      size_t rightDepth = swTreeRecurseDepth(tree, node->right);
      if (leftDepth > rightDepth)
        {
          depth = leftDepth+1;
        }
      else
        {
          depth = rightDepth+1;
        }
    }

  return depth;
}

/**
 * Private helper function for swTree_t.
 *
 * Recursive walk of tree nodes. Bails out at the first failure. 
 * Doesn't do tail recursion?
 *
 * \param tree pointer to \c swTree_t
 * \param node pointer to node of tree.
 * \param walk function to apply to each entry
 * \return \c bool indicating success
 */
static
bool
swTreeRecurseWalk(swTree_t * const tree, 
                  swTreeNode_t * const node, 
                  const swTreeWalkFunc_t walk)
{
  if (node == NULL)
    {
      return false;
    }
  
  if ((node->left != NULL) && 
      (swTreeRecurseWalk(tree, node->left, walk) == false))
    {
      return false;
    }

  bool val = walk(node->entry, tree->user);
  
  if ((node->right != NULL) && 
      (swTreeRecurseWalk(tree, node->right, walk) == false))
    {
      return false;
    }

  return val;
}

/**
 * Private helper function for swTree_t.
 *
 * Recursively find size of sub-tree at node.
 *
 * \param tree pointer to \c swTree_t
 * \param node pointer to node of tree.
 * \return size
 */
static
size_t
swTreeRecurseSize(swTree_t * const tree,     
                  const swTreeNode_t * const node)
{
  if (node == NULL)
    {
      return 0;
    }

  return
    swTreeRecurseSize(tree, node->left)
    + swTreeRecurseSize(tree, node->right)
    + 1;
}

/**
 * Private helper function for swTree_t.
 *
 * Recursively check node order. Doesn't do tail recursion?
 *
 * \param tree pointer to \c swTree_t
 * \param node pointer to node of sw tree.
 * \return \c bool indicating success
 */
static
bool
swTreeRecurseCheck(swTree_t * const tree,     
                   const swTreeNode_t * const node)
{
  if (node == NULL)
    {
      return false;
    }
  
  /* Check left child */
  if (node->left != NULL)
    {
      if ((tree->compare)(node->left->entry, 
                          node->entry,
                          tree->user) != compareLesser)
        {
          return false;
        }
      if (swTreeRecurseCheck(tree, node->left) == false)
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
      if (swTreeRecurseCheck(tree, node->right) == false)
        {
          return false;
        }
    }

  return true;
}

/**
 * Private helper function for swTree_t.
 *
 * Recursively clear nodes from the tree. Doesn't do tail recursion?
 *
 * \param tree pointer to \c swTree_t
 * \param node pointer to node of sw tree.
 */
static
void
swTreeRecurseClear(swTree_t * const tree, swTreeNode_t * const node)
{
  if (node == NULL)
    {
      return;
    }
    
  /* Destroy left child */
  if (node->left != NULL)
    {
      swTreeRecurseClear(tree, node->left);
    }

  /* Destroy right child */
  if (node->right != NULL)
    {
      swTreeRecurseClear(tree, node->right);
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
 * Private helper function for Stout/Warren tree implementation.
 *
 *  Given an entry find the corresponding node.
 *
 * \param tree pointer to swTree_t
 * \param entry pointer to caller's data 
 * \return pointer to swTreeNode_t found. 
 * \c NULL if not found.
 */
static
swTreeNode_t *
swTreeFindNode(swTree_t * const tree, const void * const entry)
{
  swTreeNode_t *node;
  compare_e comp;
  
  if ((tree == NULL) || (tree->root == NULL) || (entry == NULL))
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

swTree_t *swTreeCreate
(const swTreeAllocFunc_t alloc, 
 const swTreeDeallocFunc_t dealloc,
 const swTreeDuplicateEntryFunc_t duplicateEntry,
 const swTreeDeleteEntryFunc_t deleteEntry,
 const swTreeDebugFunc_t debug,
 const swTreeCompFunc_t comp,
 void * const user)
{
  swTree_t *tree; 

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
  
  tree = alloc(sizeof(swTree_t), user);
  if (tree == NULL)
    {
      debug(__func__, __LINE__, user, 
            "Can't allocate %d for swTree_t",
            sizeof(swTree_t));
      return NULL;
    }

  tree->compare = comp;
  tree->alloc = alloc;
  tree->dealloc = dealloc;
  tree->duplicateEntry = duplicateEntry;
  tree->deleteEntry = deleteEntry;
  tree->debug = debug; 
  tree->root = NULL; 
  tree->size = 0;
  tree->depth_factor = swTree_depth_factor;
  tree->user = user;

  return tree;
}

void *swTreeFind(swTree_t * const tree, void * const entry)
{
  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  swTreeNode_t *node = swTreeFindNode(tree, entry);
  if (node == NULL)
    {
      return NULL;
    }
  else
    {
      return node->entry;
    }
}

void *swTreeInsert(swTree_t * const tree, void * const entry)
{
  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  /* Search for an existing entry */
  swTreeNode_t *node = tree->root;
  swTreeNode_t *parent = NULL;
  size_t depth = 0;
  compare_e comp = compareError;

  while (node != NULL) 
    {
      depth = depth+1;
      parent = node;
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
          if (tree->deleteEntry != NULL)
            {
              (tree->deleteEntry)(node->entry, tree->user);
            }
          node->entry = swTreeDuplicateEntry(tree, entry);
          if (node->entry == NULL)
            {
              (tree->debug)(__func__, __LINE__, tree->user,
                            "swTreeDuplicateEntry() failed!");
              return NULL;
            }
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
  node = (swTreeNode_t *)(tree->alloc)(sizeof(swTreeNode_t), tree->user);
  if (node == NULL)
    {
      (tree->debug)(__func__, __LINE__, tree->user, "tree->alloc() failed!");
      return NULL;
    }
  node->entry = swTreeDuplicateEntry(tree, entry);
  if (node->entry == NULL)
    {
      (tree->dealloc)(node, tree->user);
      (tree->debug)(__func__, __LINE__, tree->user,
                    "swTreeDuplicateEntry() failed!");
      return NULL;
    }
  node->left = node->right = NULL;
  if (parent == NULL) 
    {
      tree->root = node;
    }
  else
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

  tree->size = tree->size+1;

  /* Re-balance the tree */
  if (swTreeCheckDepth(tree, depth))
    {
      swTreeRebalance(&(tree->root));
    }
    
  return node->entry;
}

void *
swTreeRemove(swTree_t * const tree, void * const entry) 
{
  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  swTreeNode_t *removed = NULL;
  swTreeNode_t **removed_subnode = &(tree->root);
  void *removed_entry = NULL;
  size_t depth = 0;

  /* Find the node and entry that is to be removed */
  swTreeNode_t *node = tree->root;
  while(node != NULL)
    {
      depth = depth+1;
      compare_e comp = (tree->compare)(entry, node->entry, tree->user);
      if (comp == compareEqual)  
        {
          removed = node;
          removed_entry = removed->entry;
          break;
        }
      else if (comp == compareLesser)  
        {
          removed_subnode = &(node->left);
          node = node->left;
        }
      else if (comp == compareGreater)
        {
          removed_subnode = &(node->right);
          node = node->right;
        }
      else       
        {
          (tree->debug)(__func__, __LINE__, tree->user,
                        "Illegal compare result!");
          return NULL;
        }
    }

  /* Entry not found */
  if (removed == NULL)
    {
      return NULL;
    }

  /* Patch the tree */
  if ((removed->left == NULL) && (removed->right == NULL))
    {
      *removed_subnode = NULL;
    }
  else if (removed->left == NULL)
    {
      *removed_subnode = removed->right;
    }
  else if (removed->right == NULL)
    {
      *removed_subnode = removed->left;
    }
  else
    {
      /* Find the successor node */ 
      swTreeNode_t *succ_parent = removed;
      swTreeNode_t *succ = removed->right;
      while(succ->left != NULL)
        {
          succ_parent = succ;
          succ = succ->left;
        }
     
      /* Move the successor entry to replace removed->entry */  
      removed->entry = succ->entry;
      if (removed == succ_parent)
        {
          /*      removed
           *        / \
           *          succ
           *          / \
           *      NULL   succ->right
           */
          removed->right = succ->right;
        }
      else
        {
          /*      removed
           *        / \
           *           removed->right
           *              / \
           *             .
           *            /
           *         succ_parent
           *          / \
           *       succ
           *        / \
           *    NULL   succ->right
           */
          succ_parent->left = succ->right;
        }
     
      /* Free the successor node */
      removed = succ;
    }
  
  /* Free the node */
  if(tree->deleteEntry != NULL)
    {
      (tree->deleteEntry)(removed_entry, tree->user);
    }
  (tree->dealloc)(removed, tree->user);

  /* Adjust size */
  tree->size = tree->size-1;

  /* Rebalance the tree */
  if (swTreeCheckDepth(tree, depth))
    {
      swTreeRebalance(&(tree->root));
    }
  
  return removed_entry;
}

void swTreeClear(swTree_t * const tree)
{
  if (tree == NULL)
    {
      return;
    }
  else if (tree->root == NULL)
    {
      tree->size = 0;
      return;
    }

  swTreeRecurseClear(tree, tree->root);
  tree->root = NULL;
  tree->size = 0;
  return;
}

void swTreeDestroy(swTree_t * const tree)
{
  if ((tree == NULL) || (tree->dealloc == NULL))
    {
      return;
    }

  /* Free all the nodes in the tree */
  swTreeClear(tree);
  
  /* Free the tree */
  tree->dealloc(tree, tree->user);
}

size_t swTreeGetDepth(const swTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  size_t depth = swTreeRecurseDepth(tree, tree->root);
  return depth;
}

size_t swTreeGetSize(const swTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  return tree->size;
}

void *swTreeGetMin(swTree_t * const tree)
{
  if (tree == NULL)
    {
      return NULL;
    }
  
  swTreeNode_t *node = swTreeFindMinMax(tree, compareLesser);

  if (node == NULL)
    {
      return NULL;
    }

  return node->entry;
}

void *swTreeGetMax(swTree_t * const tree)
{
  if (tree == NULL)
    {
      return NULL;
    }
  
  swTreeNode_t *node = swTreeFindMinMax(tree, compareGreater);

  if (node == NULL)
    {
      return NULL;
    }

  return node->entry;
}

bool swTreeWalk(swTree_t * const tree, const swTreeWalkFunc_t walk)
{
  if ((tree == NULL) || (walk == NULL) || (tree->root == NULL))
    {
      return false;
    }

  return swTreeRecurseWalk(tree, tree->root, walk);
}

bool swTreeCheck(swTree_t * const tree)
{
  if ((tree == NULL) || (tree->compare == NULL))
    {
      return false;
    }

  if (tree->root == NULL)
    {
      if (tree->size == 0)
        {
          return true;
        }
      else
        {
          return false;
        }
    }
  
  size_t size = swTreeRecurseSize(tree, tree->root);
  if (size != tree->size)
    {
      tree->debug(__func__, __LINE__, tree->user, "size mismatch(%u != %u)!",
                  size, tree->size);
      return false;
    }
 
  return swTreeRecurseCheck(tree, tree->root);
}
