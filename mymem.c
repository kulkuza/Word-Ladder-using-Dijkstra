/*mymem.c*/

//
// wrapper funtions for malloc and free
//
// Michael Mei
// Linux Mint 18(Sarah) with gcc
// U. of Illinois, Chicago
// CS251, Fall 2016
// HW #14
//

#include <stdio.h>
#include <stdlib.h>

#include "mymem.h"

static int g_mallocs = 0;
static int g_mallocFailures = 0;
static int g_frees = 0;
static int g_freeErrors = 0;

void *mymalloc(unsigned int size)
{
  g_mallocs++;

  void *ptr = malloc(size);

  if (ptr == NULL)
    g_mallocFailures++;

  return ptr;
}

void myfree(void *ptr)
{
  g_frees++;

  if (ptr == NULL)
    g_freeErrors++;

  free(ptr);
}

void mymem_stats()
{
  printf("** Memory stats: malloc (%d, %d), free (%d, %d)\n",
    g_mallocs, g_mallocFailures, g_frees, g_freeErrors);
}
