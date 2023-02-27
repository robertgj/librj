/**
 * \file sgTree.c
 *
 * Implementation of scapegoat rebalancing of a binary tree type. The
 * sub-tree is rebalanced with the Stout/Warren recursive algorithm.
 *
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "compare.h"
#include "stack.h"
#include "sgTree.h"
#include "sgTree_private.h"

/**
 * Private helper function for scapegoat tree implementation.
 *
 * Flatten the tree at node x
 *
 * \param x pointer to sgTreeNode_t
 * \param y pointer to sgTreeNode_t
 * \return pointer to sgTreeNode_t
 */
static
sgTreeNode_t *
sgTreeFlatten(sgTreeNode_t *x, sgTreeNode_t *y)
{
  if (x == NULL)
    {
      return y;
    }

  x->right = sgTreeFlatten(x->right,y);
  return sgTreeFlatten(x->left, x);
}

/**
 * Private helper function for scapegoat tree implementation.
 *
 * Build a 1/2-weight balanced tree from the list of n nodes at x
 *
 * \param n number of nodes in the list at x is n+1
 * \param x pointer to pointer to sgTreeNode_t
 * \return pointer to n+1'th node whose left element points to the root
 * of the tree
 */
static
sgTreeNode_t *
sgTreeBuildTree(size_t n, sgTreeNode_t *x)
{
  if (n == 0)
    {
      x->left = NULL;
      return x;
    }
  sgTreeNode_t *r = sgTreeBuildTree(n/2 , x);
  sgTreeNode_t *s = sgTreeBuildTree((n-1)/2 , r->right);
  r->right = s->left;
  s->left = r;
  return s;
}

/**
 * Private helper function for scapegoat tree implementation.
 *
 * Rebuild the tree at the scapegoat node
 *
 * \param n number of nodes in the list at x is n+1
 * \param scapegoat pointer to pointer to sgTreeNode_t
 * \return pointer to n+1'th node whose left element points to the root
 * of the tree
 */
static
sgTreeNode_t *
sgTreeRebuildTree(size_t n, sgTreeNode_t *scapegoat)
{
  sgTreeNode_t w = {.left=NULL, .right=NULL, .entry=NULL};
  sgTreeNode_t *z = sgTreeFlatten(scapegoat, &w);
  sgTreeBuildTree(n, z);
  return w.left;
}

/**
 * Private helper function for scapegoat tree implementation.
 *
 *  Duplicate an entry.
 *
 * \param tree pointer to sgTree_t
 * \param entry pointer to caller's data 
 * \return pointer to duplicated entry
 */
static
void *
sgTreeDuplicateEntry(sgTree_t * const tree, void * const entry)
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
 * Private helper function for sgTree_t.
 *
 * Recursively find size of sub-tree at node. Doesn't do tail recursion?
 *
 * \param tree pointer to \c sgTree_t
 * \param node pointer to node of scapegoat tree.
 * \return size
 */
static
size_t
sgTreeRecurseSizeNode(sgTree_t * const tree,     
                      const sgTreeNode_t * const node)
{
  if (node == NULL)
    {
      return 0;
    }
  
  return
    sgTreeRecurseSizeNode(tree, node->left)
    + sgTreeRecurseSizeNode(tree, node->right)
    + 1;
}

/**
 * Private helper function for sgTree_t.
 *
 * Find the size of the sub-tree at node non-recursively with a stack.
 *
 * \param tree pointer to \c sgTree_t
 * \param node pointer to node of scapegoat tree.
 * \return size
 */
static
size_t
sgTreeNonRecurseSizeNode(sgTree_t * const tree,     
                         sgTreeNode_t * const node)
{
  if ((tree == NULL) || (node == NULL))
    {
      return 0;
    }

  size_t thisSize = 1;
  sgTreeNode_t *prev = NULL;
  sgTreeNode_t *current = node;
  sgTreeNode_t *next = NULL;

  /* Non-recursive sub-tree size finder */
  stackClear(tree->stack);
  while (current != NULL)
    {
      /* Descend subtree, next is predecessor */
      if (prev == stackPeek(tree->stack))
        {
          prev = current;
          next = current->left;
          if (next != NULL)
            {
              thisSize++;
              if (stackPush(tree->stack,current) == NULL)
                {
                  (tree->debug)(__func__, __LINE__, tree->user,
                                "stackPush() failed!");
                  return 0;
                }
            }
        }

      /* Current is root of sub-tree. Descend */
      if ((next == NULL) || (prev == current->left))
        {
          prev = current;
          next = current->right;
          if (next != NULL)
            {
              thisSize++;
              if (stackPush(tree->stack,current) == NULL)
                {
                  (tree->debug)(__func__, __LINE__, tree->user,
                                "stackPush() failed!");
                  return 0;
                }
            }
        }

      /* Ascend */
      if((next == NULL) || (prev == current->right))
        {
          prev = current;
          next = stackPop(tree->stack);
        }

      current = next;
    }

  return thisSize;
}

static
size_t
sgTreeSizeNode(sgTree_t * const tree,     
               sgTreeNode_t * const node)
{
  if ((tree == NULL) || (node == NULL))
    {
      return 0;
    }

  if (tree->use_recursize_size_node == true)
    {
      return sgTreeRecurseSizeNode(tree, node);
    }
  else  
    {
      return sgTreeNonRecurseSizeNode(tree, node);
    }
}

/**
 * Private helper function for sgTree_t.
 *
 * Recursively find tree's depth. Doesn't do tail recursion?
 *
 * \param node pointer to node of sgTree_t
 * \return depth of sw tree
 */
static
size_t
sgTreeRecurseDepth(sgTreeNode_t * const node)
{
  size_t depth = 0;
  
  if (node != NULL)
    {
      size_t leftDepth = sgTreeRecurseDepth(node->left);
      size_t rightDepth = sgTreeRecurseDepth(node->right);
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
 * Private helper function for scapegoat tree implementation.
 *
 * Insert a node into the tree.
 *
 * \param tree pointer to sgTree_t
 * \param node pointer to sgTreeNode_t of the tree
 * \param entry to be inserted into the tree
 * \return pointer \c NULL if failed.
 */
static
void *
sgTreeRecurseInsertNode(sgTree_t * const tree,
                        sgTreeNode_t **node,
                        void * const entry)
{
  if ((tree == NULL) || (node == NULL) || (entry == NULL))
    {
      return NULL;
    }

  void *ptr = NULL;

  if ((*node) == NULL)
    {
      (*node) = (sgTreeNode_t *)(tree->alloc(sizeof(sgTreeNode_t), tree->user));
      if ((*node) == NULL)
        {
          tree->debug(__func__, __LINE__, tree->user, 
                      "Can't allocate %d for sgTreeNode_t",
                      sizeof(sgTreeNode_t));
          return NULL;
        }

      /* Copy the new entry */
      (*node)->entry = sgTreeDuplicateEntry(tree, entry);
      if ((*node)->entry == NULL)
        {
          (tree->dealloc)(*node, tree->user);
          (tree->debug)(__func__, __LINE__, tree->user,
                        "sgTreeDuplicateEntry() failed!");
          return NULL;
        }
      (*node)->left = NULL;
      (*node)->right = NULL; 
      
      tree->subtree_size = 1;
      tree->subtree_depth = 0;
      
      tree->size = tree->size + 1;
      if (tree->size > tree->max_size)
        {
          tree->max_size = tree->size;
        }
      
      ptr = (*node)->entry;
    }
  else
    {
      size_t brother_subtree_size = 0; 
      compare_e comp = (tree->compare)(entry, (*node)->entry, tree->user);
      if (comp == compareLesser) 
        {
          ptr = sgTreeRecurseInsertNode(tree, &((*node)->left), entry);
          if (tree->rebalance_done == false)
            {
              brother_subtree_size = sgTreeSizeNode(tree, (*node)->right);
            }
        }
      else if (comp == compareGreater)
        {
          ptr = sgTreeRecurseInsertNode(tree, &((*node)->right), entry);
          if (tree->rebalance_done == false)
            {
              brother_subtree_size = sgTreeSizeNode(tree, (*node)->left);
            }
        }
      else if (comp == compareEqual)
        {
          /* Replace the existing entry */
          ptr = (*node)->entry;
          if (tree->deleteEntry != NULL)
            {
              (tree->deleteEntry)((*node)->entry, tree->user);
            }
          
          (*node)->entry = sgTreeDuplicateEntry(tree, entry);
          if ((*node)->entry == NULL)
            {
              (tree->debug)(__func__, __LINE__, tree->user,
                            "sgTreeDuplicateEntry() failed!");
              return NULL;
            }

          /* Find the sub-tree size and depth at *node */
          if (tree->rebalance_done == false)
            {
              tree->subtree_size = sgTreeSizeNode(tree, (*node)->right);
              brother_subtree_size = sgTreeSizeNode(tree, (*node)->left);
              if (tree->use_alpha_weight_balance == false)
                {
                  tree->subtree_depth = sgTreeRecurseDepth(*node)-1;
                }
            }
        }
      else
        {
          (tree->debug)(__func__, __LINE__, tree->user,
                        "Illegal compare result!");
          return NULL;
        }
      
      size_t node_subtree_size = tree->subtree_size;
      tree->subtree_size = 1 + node_subtree_size + brother_subtree_size;
      tree->subtree_depth = 1 + tree->subtree_depth;
  
      /* Check if this is an alpha-weight/height balanced scapegoat node */
      if (tree->rebalance_done == false)
        {
          bool do_rebalance = false;
          if (tree->use_alpha_weight_balance)
            {
              size_t alpha_size =
                (size_t)(tree->alpha*(double)(tree->subtree_size));
              do_rebalance = (node_subtree_size > alpha_size)
                || (brother_subtree_size > alpha_size);
            }
          else
            {
              double h_alpha =
                floor(-log((double)(tree->subtree_size))/(tree->ln_alpha));
              do_rebalance = (tree->subtree_depth > h_alpha ? true : false);
            }
                     
          if (do_rebalance)
            {
              *node = sgTreeRebuildTree(tree->subtree_size, *node);
              tree->max_size = tree->size;
              tree->rebalance_done = true;
            }
        }
    }
  
  return ptr;
}

/**
 * Private helper function for sgTree_t.
 *
 * Follow the left or right children of the root to find the minimum or
 * maximum entry in the tree. 
 *
 * \param tree pointer to \c sgTree_t
 * \param val if \c compareLesser minimum, if \c compareGreater maximum
 * \return pointer to \c sgTree_t left- or right-most node
 */
static
sgTreeNode_t *
sgTreeFindMinMax(sgTree_t * const tree, compare_e val) 
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

  sgTreeNode_t *node = tree->root;
  sgTreeNode_t *parent = NULL;
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
 * Private helper function for sgTree_t.
 *
 * Recursive walk of tree nodes. Bails out at the first failure. 
 * Doesn't do tail recursion?
 *
 * \param tree pointer to \c sgTree_t
 * \param node pointer to node of sw tree.
 * \param walk function to apply to each entry
 * \return \c bool indicating success
 */
static
bool
sgTreeRecurseWalk(sgTree_t * const tree, 
                  sgTreeNode_t * const node, 
                  const sgTreeWalkFunc_t walk)
{
  if (node == NULL)
    {
      return false;
    }
  
  if ((node->left != NULL) && 
      (sgTreeRecurseWalk(tree, node->left, walk) == false))
    {
      return false;
    }

  bool val = walk(node->entry, tree->user);
  
  if ((node->right != NULL) && 
      (sgTreeRecurseWalk(tree, node->right, walk) == false))
    {
      return false;
    }

  return val;
}

/**
 * Private helper function for sgTree_t.
 *
 * Recursively check node order. Doesn't do tail recursion?
 *
 * \param tree pointer to \c sgTree_t
 * \param node pointer to node of sw tree.
 * \return \c bool indicating success
 */
static
bool
sgTreeRecurseCheck(sgTree_t * const tree,     
                   const sgTreeNode_t * const node)
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
      if (sgTreeRecurseCheck(tree, node->left) == false)
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
      if (sgTreeRecurseCheck(tree, node->right) == false)
        {
          return false;
        }
    }

  return true;
}

/**
 * Private helper function for sgTree_t.
 *
 * Recursively clear nodes from the tree. Doesn't do tail recursion?
 *
 * \param tree pointer to \c sgTree_t
 * \param node pointer to node of sw tree.
 */
static
void
sgTreeRecurseClear(sgTree_t * const tree, sgTreeNode_t * const node)
{
  if (node == NULL)
    {
      return;
    }
    
  /* Destroy left child */
  if (node->left != NULL)
    {
      sgTreeRecurseClear(tree, node->left);
    }

  /* Destroy right child */
  if (node->right != NULL)
    {
      sgTreeRecurseClear(tree, node->right);
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
 * Private helper function for scapegoat tree implementation.
 *
 *  Given an entry find the corresponding node.
 *
 * \param tree pointer to sgTree_t
 * \param entry pointer to caller's data 
 * \return pointer to sgTreeNode_t found. 
 * \c NULL if not found.
 */
static
sgTreeNode_t *
sgTreeFindNode(sgTree_t * const tree, const void * const entry)
{
  sgTreeNode_t *node;
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

sgTree_t *sgTreeCreate
(const sgTreeAllocFunc_t alloc, 
 const sgTreeDeallocFunc_t dealloc,
 const sgTreeDuplicateEntryFunc_t duplicateEntry,
 const sgTreeDeleteEntryFunc_t deleteEntry,
 const sgTreeDebugFunc_t debug,
 const sgTreeCompFunc_t comp,
 void * const user)
{
  sgTree_t *tree; 

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
  
  tree = alloc(sizeof(sgTree_t), user);
  if (tree == NULL)
    {
      debug(__func__, __LINE__, user, 
            "Can't allocate %d for sgTree_t",
            sizeof(sgTree_t));
      return NULL;
    }

  tree->compare = comp;
  tree->alloc = alloc;
  tree->dealloc = dealloc;
  tree->duplicateEntry = duplicateEntry;
  tree->deleteEntry = deleteEntry;
  tree->debug = debug;

  tree->stack = stackCreate(32, alloc, dealloc, NULL, NULL, debug, user);
  if (tree->stack == NULL)
    {
      debug(__func__, __LINE__, user, "Can't allocate stack_t sgTree_t");
      return NULL;
    }
                            
  tree->root = NULL; 
  tree->size = 0;
  tree->max_size = tree->size;
  tree->subtree_size = 0;
  tree->subtree_depth = 0;
  tree->rebalance_done = false;
  tree->alpha = sgTree_alpha;
  tree->ln_alpha = log(tree->alpha);
  tree->use_alpha_weight_balance = false;
  tree->use_recursize_size_node = true;
  tree->user = user;

  return tree;
}

void *sgTreeFind(sgTree_t * const tree, void * const entry)
{
  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  sgTreeNode_t *node = sgTreeFindNode(tree, entry);
  if (node == NULL)
    {
      return NULL;
    }
  else
    {
      return node->entry;
    }
}

void *sgTreeInsert(sgTree_t * const tree, void * const entry)
{
  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  tree->subtree_size = 0;
  tree->subtree_depth = 0;
  tree->rebalance_done = false;
  return sgTreeRecurseInsertNode(tree, &(tree->root), entry);
}

void *
sgTreeRemove(sgTree_t * const tree, void * const entry) 
{
  if ((tree == NULL) || (entry == NULL))
    {
      return NULL;
    }

  sgTreeNode_t *removed = NULL;
  sgTreeNode_t **removed_subnode = &(tree->root);
  void *removed_entry = NULL;

  /* Find the node and entry that is to be removed */
  sgTreeNode_t *node = tree->root;
  while(node != NULL)
    {
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
      sgTreeNode_t *succ_parent = removed;
      sgTreeNode_t *succ = removed->right;
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
  if (tree->size == 0)
    {
      tree->max_size = 0;
    }
  else if (tree->size < (size_t)(tree->alpha * (double)tree->max_size))
    {
      tree->root = sgTreeRebuildTree(tree->size, tree->root);
      tree->max_size = tree->size;
    }
  
  return removed_entry;
}

void sgTreeClear(sgTree_t * const tree)
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

  sgTreeRecurseClear(tree, tree->root);
  tree->root = NULL; 
  tree->size = 0;
  return;
}

void sgTreeDestroy(sgTree_t * const tree)
{
  if ((tree == NULL) || (tree->dealloc == NULL))
    {
      return;
    }

  /* Destroy the stack */
  stackDestroy(tree->stack);
  
  /* Free all the nodes in the tree */
  sgTreeClear(tree);
  
  /* Free the tree */
  tree->dealloc(tree, tree->user);
}

size_t sgTreeGetDepth(const sgTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  return sgTreeRecurseDepth(tree->root);
}

size_t sgTreeGetSize(const sgTree_t * const tree)
{
  if (tree == NULL)
    {
      return 0;
    }

  return tree->size;
}

void *sgTreeGetMin(sgTree_t * const tree)
{
  if (tree == NULL)
    {
      return NULL;
    }
  
  sgTreeNode_t *node = sgTreeFindMinMax(tree, compareLesser);

  if (node == NULL)
    {
      return NULL;
    }

  return node->entry;
}

void *sgTreeGetMax(sgTree_t * const tree)
{
  if (tree == NULL)
    {
      return NULL;
    }
  
  sgTreeNode_t *node = sgTreeFindMinMax(tree, compareGreater);

  if (node == NULL)
    {
      return NULL;
    }

  return node->entry;
}

bool sgTreeWalk(sgTree_t * const tree, const sgTreeWalkFunc_t walk)
{
  if ((tree == NULL) || (walk == NULL) || (tree->root == NULL))
    {
      return false;
    }

  return sgTreeRecurseWalk(tree, tree->root, walk);
}

bool sgTreeCheck(sgTree_t * const tree)
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
  
  size_t size = sgTreeSizeNode(tree, tree->root);
  if (size != tree->size)
    {
      tree->debug(__func__, __LINE__, tree->user, "size mismatch(%u != %u)!",
                  size, tree->size);
      return false;
    }
  
  return sgTreeRecurseCheck(tree, tree->root);
}
