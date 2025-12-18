#ifndef _BASE64_H
#define _BASE64_H

#define ALPHA_LENGTH 26

#include <stdlib.h>
#include <string.h>
#include "binary.h"
#include "utils.h"

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

/**
 * Decode base64 text to 1 byte character. Return a heap string for ease of use.
 */
char *base64Decode(char *encoded, size_t encodedSize)
{
   if ((encodedSize - 1) % 4 != 0) {
      failureExit("Invalid base64 string. Aborting the program.\n");
   }

   // calculate equal padding
   size_t eqlPadSize = 0;
   for (size_t i = encodedSize - 3; i < encodedSize; i++) {
      eqlPadSize += encoded[i] == '=';
   }

   size_t encodedLength = encodedSize - 1 - eqlPadSize;
   size_t binaryLength  = encodedLength * 6;
   char   *binary       = malloc(binaryLength);

   size_t binaryIdx = 0;
   for (size_t encodedIdx = 0; encodedIdx < encodedLength; ++encodedIdx) {
      size_t charMapIdx  = -1;
      char   encodedChar = encoded[encodedIdx];

      // search the index of encoded char according to `pB64_charMap`
      if ((size_t) (encodedChar - 'A') < ALPHA_LENGTH) {
         charMapIdx = encodedChar - 'A';
      }
      else if ((size_t) (encodedChar - 'a') < ALPHA_LENGTH) {
         charMapIdx = encodedChar - 'a' + ALPHA_LENGTH;
      }
      else if ((size_t) (encodedChar - '0') < 10) {
         charMapIdx = encodedChar - '0' + ALPHA_LENGTH * 2;
      }
      else if (encodedChar == '+') {
         charMapIdx = 62;
      }
      else if (encodedChar == '/') {
         charMapIdx = 63;
      }
      else {
         failureExit("Invalid base64 string. Aborting the program.\n");
      }

      // encode the index to 6 bit of binary
      char buffer[BYTE_SIZE];
      fBin_parseChar(charMapIdx, buffer);
      memcpy(binary + binaryIdx, buffer + 2, 6);

      binaryIdx += 6;
   }

   // remove zero padding, zero padding is always 2 * eqlPadSize
   binaryLength -= 2 * eqlPadSize;

   size_t decodedLength = binaryLength / 8;
   char   *decoded      = calloc(decodedLength + 1, 1);

   // buffer to hold 8 bit of binary for `strtol`
   char buffer[BYTE_SIZE + 1];
   buffer[BYTE_SIZE] = '\0';

   // decode 8 bit of binary to a single character
   binaryIdx = 0;
   for (size_t decodedIdx = 0; decodedIdx < decodedLength; ++decodedIdx) {
      memcpy(buffer, binary + binaryIdx, BYTE_SIZE);
      decoded[decodedIdx] = strtol(buffer, NULL, 2);
      binaryIdx += BYTE_SIZE;
   }

   return decoded;
}

#endif