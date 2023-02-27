/**
 * \file interp_callbacks.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "compare.h"
#include "interp_data.h"
#include "interp_utility.h"

void *
interpAlloc(const size_t size, void * const user)
{
  (void)user;
  char *m = (char *)malloc(size);
  if (m == NULL)
    {
      interpError(__func__, __LINE__, "interpAlloc(%zd) failed!", size);
      return NULL;
    }
  memset(m, 0xaa, size);
  return m;
}

void *
interpRealloc(void * const pointer, const size_t amount, void * const user)
{
  (void)user;
  char *m = (char *)realloc(pointer, amount);
  if (m == NULL)
    {
      interpError(__func__, __LINE__, "interpRealloc(%zd) failed!", amount);
      return NULL;
    }
  return m;
}

void 
interpDealloc(void * const ptr, void * const user)
{
  (void)user;
  if (ptr == NULL)
    {
      interpError(__func__, __LINE__, "Tried to free NULL pointer!");
    }
  free(ptr);
}

void *
interpDuplicateEntry(void * const entry, void * const user)
{
  if (entry == NULL)
    {
      interpError(__func__, __LINE__, "Tried to duplicate NULL pointer!");
      return NULL;
    }

  data_t *new_entry = interpAlloc(sizeof(data_t), user);

  if (new_entry != NULL)
    {
      memcpy(new_entry, entry, sizeof(data_t));
    }
  
  return new_entry;
}


bool
interpDeleteEntry(void * const entry, void * const user)
{
  (void)user;
  if (entry == NULL)
    {
      interpError(__func__, __LINE__, "Tried to free NULL pointer!");
      return false;
    }
  free(entry);
  return true;
}

void
interpDebug(const char *function, 
            const unsigned int line, 
            void * const user,
            const char *format, 
            ...)
{
  va_list ap;

  (void)user;

  fprintf(stderr, "\n");

  if (function != NULL)
    {
      fprintf(stderr, "\n%s(line %u): ", function, line);
    }

  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);

  fprintf(stderr, "\n\n");

  exit(-1);
}

compare_e interpComp(const void * const a,
                     const void * const b,
                     void * const user)
{
  (void) user;
  if ((a == NULL) || (b == NULL))
    {
      interpError(__func__, __LINE__, " NULL pointer!");
      return compareError;
    }
 
  if (*(const data_t *)a < *(const data_t *)b)
    {
      return compareLesser;
    }
  else if (*(const data_t *)a > *(const data_t *)b)
    {
      return compareGreater;
    }
  else 
    {
      return compareEqual;
    }
}
