#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Print message to `stderr` and exit with code 1.
 */
void failureExit(char* message)
{
   fprintf(stderr, "%s\n", message);
   exit(1);
}

/**
 * New malloc function with `NULL` fail check.
 */
void *malloc2(size_t size)
{
   char *newmem = malloc(size);
   if (newmem == NULL) {
      failureExit("ERROR: Not enough memory to read the file.");
   }
   newmem[size - 1] = 0;
   return (void*) newmem;
}

#endif