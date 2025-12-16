#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/base64.h"

int main(int argc, char* argv[]) {
   if (argc < 2) {
      printf("Invalid arguments.\n");
      printf("Usage: b64 <raw>\n");
      return 1;
   }

   char *encoded = base64Encode(argv[1], strlen(argv[1]));
   puts(encoded);
   free(encoded);
   return 0;
}