#ifndef _BASE64_H
#define _BASE64_H

#include <stdlib.h>
#include <string.h>
#include "binary.h"

const char pB64_charMap[] =
   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
   "abcdefghijklmnopqrstuvwxyz"
   "0123456789+/";

/**
 * Encode a string with base64 method. Return a heap string for ease of use.
 */
char *base64Encode(unsigned char *raw, size_t rawSize)
{
   // calculate zero (0) padding for the binary,
   // and equal (=) padding for the encoded text
   size_t remainder  = rawSize % 3;
   size_t eqlPadSize = 3 - (remainder == 0 ? 3 : remainder);
   size_t zerPadSize = 2 * eqlPadSize;

   size_t binaryLength;
   char   *binary = strToBinary(raw, rawSize, &binaryLength);

   if (zerPadSize > 0) {
      size_t oldBinaryLength = binaryLength;
      binaryLength += zerPadSize;
      binary        = realloc(binary, binaryLength);
      memset(binary + oldBinaryLength, '0', zerPadSize);
   }

   size_t encodedSize = binaryLength / 6 + eqlPadSize + 1;
   char   *encoded    = malloc(encodedSize);
   encoded[encodedSize - 1] = '\0';

   // buffer to hold 6 bit of binary for `strtol`
   char   buffer[7];
   size_t encodedIdx = 0;

   // decode 6 bit of binary to a single character defined in `pB64_charMap`
   // TODO: define this execution in a function for decoder purpose
   for (size_t binaryIdx = 0; binaryIdx < binaryLength; binaryIdx += 6) {
      memcpy(buffer, binary + binaryIdx, 6);
      buffer[6] = '\0';

      size_t charMapIdx   = strtol(buffer, NULL, 2);
      encoded[encodedIdx] = pB64_charMap[charMapIdx];
      ++encodedIdx;
   }

   if (eqlPadSize > 0) {
      memset(encoded + encodedIdx, '=', eqlPadSize);
   }

   return encoded;
}

#endif