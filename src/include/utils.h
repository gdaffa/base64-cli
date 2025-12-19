#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void failureExit(char* message)
{
   fprintf(stderr, "%s\n", message);
   exit(1);
}

#endif