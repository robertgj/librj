/** 
 * \file  stack.c 
 * A \c stack_t implementation.
 */

#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "stack.h"
#include "stack_private.h"

stack_t *
stackCreate(const size_t space,
            const stackAllocFunc_t alloc, 
            const stackDeallocFunc_t dealloc, 
            const stackDuplicateEntryFunc_t duplicateEntry, 
            const stackDeleteEntryFunc_t deleteEntry, 
            const stackDebugFunc_t debug,
            void * const user)
{
  stack_t *stack = NULL;

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
  if (space == 0)
    {
      debug(__func__, __LINE__, user, "Request for space==0!");
      return NULL;
    }

  /* Allocate stack state */
  stack = alloc(sizeof(stack_t), user);
  if (stack == NULL)
    {
      debug(__func__, __LINE__, user, 
            "Couldn't allocate %d bytes for stack_t!", 
            sizeof(stack_t));

      return NULL;
    }

  /* Allocate stack memory */
  stack->bottom = (stackEntry_t *)alloc(space*sizeof(stackEntry_t), user);
  if (stack->bottom == NULL)
    {
      dealloc(stack, user);
      debug(__func__, __LINE__, user, 
            "Couldn't allocate %d bytes for stack memory!", 
            space*sizeof(stackEntry_t));

      return NULL;
    }

  /* Initalise new stack_t */
  stack->alloc = alloc;
  stack->dealloc = dealloc;
  stack->duplicateEntry = duplicateEntry;
  stack->deleteEntry = deleteEntry;
  stack->debug = debug;
  stack->size = 0;
  stack->space = space;
  stack->user = user;
  
  return stack;
}

stackEntry_t
stackPush(stack_t * const stack, stackEntry_t const entry)
{
  if ((stack == NULL) || (stack->space == 0))
    {
      return NULL;
    }

  /* Check stack space. Double space if necessary. */
  if (stack->size == stack->space)
    {
      size_t old_alloc = stack->space*sizeof(stackEntry_t);
      stackEntry_t *newStackEntry = 
        (stackEntry_t *)stack->alloc(2*old_alloc, stack->user);
      if (newStackEntry == NULL)
        {
          stack->debug(__func__, __LINE__, stack->user, 
                "Couldn't alloc new stack space %zd!", 2*old_alloc);
          return NULL;
        }

      memcpy(newStackEntry, stack->bottom, old_alloc);
      stack->dealloc(stack->bottom, stack->user);
      stack->bottom = newStackEntry;
      stack->space = 2*stack->space;
    }

  /* Push */
  void *new_entry;
  if (stack->duplicateEntry != NULL)
    {
      new_entry = stack->duplicateEntry(entry, stack->user);
      if (new_entry == NULL)
        {
          stack->debug(__func__, __LINE__, stack->user, 
                      "Couldn't duplicate new entry!");
          return NULL;
        }
    }
  else 
    {
      new_entry = entry;
    }
  (stack->bottom)[stack->size] = new_entry;
  stack->size++;

  return new_entry;
}

void *
stackPop(stack_t * const stack)
{
  stackEntry_t entry;

  if (stack == NULL)
    {
      return NULL;
    }
  if ((stack->space == 0) || (stack->size == 0))
    {
      return NULL;
    }

  /* Pop */
  stack->size--;
  entry = stack->bottom[stack->size];
  if (stack->deleteEntry != NULL)
    {
      (stack->deleteEntry)(entry, stack->user);
      entry = NULL;
    }

  return (void *)entry;
}

stackEntry_t
stackPeek(stack_t * const stack)
{
  if ((stack == NULL) || (stack->space == 0) || (stack->size == 0))
    {
      return NULL;
    }

  return stack->bottom[(stack->size)-1];
}

void
stackClear(stack_t * const stack)
{
  if (stack == NULL)
    {
      return;
    }
  
  if (stack->deleteEntry == NULL)
    {
      stack->size = 0;
      return;
    }
  
  while (stack->size != 0)
    {
      stack->size--;
      if (stack->deleteEntry != NULL)
        {
          (stack->deleteEntry)(stack->bottom[stack->size], stack->user);
        }
    }

  return;
}

void 
stackDestroy(stack_t * const stack)
{
  if (stack == NULL)
    {
      return;
    }

  /* Clear stack */
  stackClear(stack);

  /* Free stack memory */
  (stack->dealloc)(stack->bottom, stack->user);

  /* Free the stack_t */
  (stack->dealloc)(stack, stack->user);
}

size_t 
stackGetSize(const stack_t * const stack)
{
  if (stack == NULL)
    {
      return 0;
    }
  return stack->size;
}

size_t 
stackGetSpace(const stack_t * const stack)
{
  if (stack == NULL)
    {
      return 0;
    }
  return stack->space;
}

bool
stackWalk(stack_t * const stack, const stackWalkFunc_t walk)
{
  bool res;

  if ((stack == NULL) ||  (stack->bottom == NULL) || (walk == NULL))
    {
      return false;
    }

  for (size_t i = 0; i < stack->size; i++)
    {
      if ((res = walk(stack->bottom[i], stack->user)) == false)
        {
          return false;
        }
    }

  return true;
}

