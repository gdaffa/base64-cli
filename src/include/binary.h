#ifndef _BINARY_H
#define _BINARY_H

#define BYTE_SIZE 8

#include "utils.h"

/**
 * Transform a char (1 byte) to a binary format.
 */
void fBin_parseChar(char chr, char *binaryHolder)
{
   //! loop unrolling for faster execution
   binaryHolder[7] = chr & 1 ? '1' : '0';
   binaryHolder[6] = chr & 2 ? '1' : '0';
   binaryHolder[5] = chr & 4 ? '1' : '0';
   binaryHolder[4] = chr & 8 ? '1' : '0';
   binaryHolder[3] = chr & 16 ? '1' : '0';
   binaryHolder[2] = chr & 32 ? '1' : '0';
   binaryHolder[1] = chr & 64 ? '1' : '0';
   binaryHolder[0] = chr & 128 ? '1' : '0';
}

/**
 * Transform every char in the string to binary format. Return a heap string for
 * ease of use.
 *
 * @param[out] binaryLengthPtr Pointer to receives the returned binary length.
 */
char* strToBinary(char *str, size_t strSize, size_t *binaryLengthPtr)
{
   size_t binarySize = strSize * BYTE_SIZE + 1;
   char   *binary    = malloc2(binarySize);

   if (binaryLengthPtr != NULL) {
      *binaryLengthPtr = binarySize - 1;
   }

   size_t binaryIdx = 0;
   for (size_t strIdx = 0; strIdx < strSize; ++strIdx) {
      fBin_parseChar(str[strIdx], binary + binaryIdx);
      binaryIdx += BYTE_SIZE;
   }

   return binary;
}

#endif