#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/base64.h"
#include "include/utils.h"

void exitInvalidArgs() {
   char *message =
      "Invalid arguments.\n"
      "Usage:\n"
      "\tb64 -e <raw>       Encode the text into base64 text.\n"
      "\tb64 -d <encoded>   Decode base64 text into regular text.";
   failureExit(message);
}

int main(int argc, char* argv[]) {
   if (argc < 3) {
      exitInvalidArgs();
   }

   char *result;

   if (argv[1][1] == 'e') {
      result = base64Encode(argv[2], strlen(argv[2]));
   } else if (argv[1][1] == 'd') {
      result = base64Decode(argv[2], strlen(argv[2]) + 1);
   } else {
      exitInvalidArgs();
   }

   puts(result);

   free(result);
   return 0;
}