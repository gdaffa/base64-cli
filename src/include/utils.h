#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define ERR_MSG_FREAD "An error occured when opening the file."
#define ERR_MSG_FWRITE "An error occured when creating the file."
#define ERR_MSG_MALLOC "Not enough memory to read the file."

/**
 * Print message to `stderr` and exit with code 1.
 */
void failureExit(char* message)
{
   fprintf(stderr, "ERROR: ");
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
      failureExit(ERR_MSG_MALLOC);
   }
   if (size > 0) {
      newmem[size - 1] = 0;
   }

   return (void*) newmem;
}

#endif