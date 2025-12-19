#ifndef _BASE64_H
#define _BASE64_H

#define ALPHA_SIZE 26
#define B64_READ_SIZE 6

#include <stdlib.h>
#include <string.h>
#include "binary.h"
#include "utils.h"

const char pB64_charMap[] =
   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
   "abcdefghijklmnopqrstuvwxyz"
   "0123456789+/";

char pB64_revMap[64];
bool pB64_isRevMapInit = false;

/**
 * Initialize reverse map for the decoder.
 */
void fB64_initReverseMap()
{
   memset(pB64_revMap, -1, 64);

   // alpha, upper and lower case
   for (size_t i = 0; i < ALPHA_SIZE; ++i) {
      pB64_revMap['A' + i] = i;
      pB64_revMap['a' + i] = i + ALPHA_SIZE;
   }
   // numeric
   for (size_t i = 0; i < 10; ++i) {
      pB64_revMap['0' + i] = i + ALPHA_SIZE * 2;
   }
   pB64_revMap['+'] = 62;
   pB64_revMap['/'] = 63;

   pB64_isRevMapInit = true;
}

/**
 * Encode a string with base64 method. Return a heap string for ease of use.
 */
char *base64Encode(unsigned char *raw, size_t rawSize)
{
   // calculate equal padding for the encoded text
   size_t remainder  = rawSize % 3;
   size_t eqlPadSize = 3 - (remainder == 0 ? 3 : remainder);

   size_t binaryLength;
   char   *binary = strToBinary(raw, rawSize, &binaryLength);

   if (eqlPadSize > 0) {
      // zero padding is always 2 * eqlPadSize
      size_t zerPadSize      = 2 * eqlPadSize;
      size_t oldBinaryLength = binaryLength;

      binaryLength += zerPadSize;
      binary        = realloc(binary, binaryLength);
      memset(binary + oldBinaryLength, '0', zerPadSize);
   }

   size_t encodedSize = binaryLength / B64_READ_SIZE + eqlPadSize + 1;
   char   *encoded    = calloc(encodedSize, 1);

   // buffer to hold 6 bit of binary for `strtol`
   char buffer[B64_READ_SIZE + 1];
   buffer[B64_READ_SIZE] = '\0';

   // decode 6 bit of binary to a single character defined in `pB64_charMap`
   size_t encodedIdx = 0;
   for (size_t binaryIdx = 0; binaryIdx < binaryLength; binaryIdx += B64_READ_SIZE) {
      memcpy(buffer, binary + binaryIdx, B64_READ_SIZE);

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
      failureExit("Invalid base64 string. Aborting the program.");
   }

   if (!pB64_isRevMapInit) {
      fB64_initReverseMap();
   }

   // calculate equal padding
   size_t eqlPadSize = 0;
   for (size_t i = encodedSize - 3; i < encodedSize; i++) {
      eqlPadSize += encoded[i] == '=';
   }

   size_t encodedLength = encodedSize - 1 - eqlPadSize;
   size_t binaryLength  = encodedLength * B64_READ_SIZE;
   char   *binary       = malloc(binaryLength);

   size_t binaryIdx = 0;
   for (size_t encodedIdx = 0; encodedIdx < encodedLength; ++encodedIdx) {
      char   encodedChar = encoded[encodedIdx];
      size_t charMapIdx  = pB64_revMap[encodedChar];

      if (charMapIdx == -1) {
         failureExit("Invalid base64 string. Aborting the program.");
      }

      // encode the index to 6 bit of binary
      char buffer[BYTE_SIZE];
      fBin_parseChar(charMapIdx, buffer);
      memcpy(binary + binaryIdx, buffer + 2, B64_READ_SIZE);

      binaryIdx += B64_READ_SIZE;
   }

   // remove zero padding, zero padding is always 2 * eqlPadSize
   binaryLength -= 2 * eqlPadSize;

   size_t decodedLength = binaryLength / BYTE_SIZE;
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