#include "include/utils.h"
#include "include/base64.h"

/**
 * A data type that hold option information.
 */
typedef struct Option {
   char   *short_;
   char   *long_;
   size_t fillMode;
   bool   isReadNextVal;
} Option;

/**
 * Exit if arguments are invalid.
 */
void exitInvalidArgs() {
   char *message =
      "Invalid arguments.\n"
      "Usage: b64 [<options>] <input>\n"
      "   or: b64 [<options>] --file <inputfile>\n";

   failureExit(message);
}

int main(int argc, char **argv)
{
   size_t uargc = argc;
   /**
    * 001 : operation has defined (encode/decode)
    * 010 : input has defined (file/argv)
    * 100 : output file has defined
    */
   size_t mode = 0;

   const Option opts[] = {
      {"-e", "--encode",   0b1, false},
      {"-d", "--decode",   0b1, false},
      {"-f", "--file"  ,  0b10, true},
      {"-o", "--output", 0b100, true}
   };
   const size_t optsLength = sizeof(opts) / sizeof(Option);

   bool isEncode = true;
   char *inputFilename  = NULL; 
   char *outputFilename = NULL;

   char   *input    = NULL;
   size_t inputSize = 0;

   for (size_t argIdx = 1; argIdx < uargc; ++argIdx) {
      char   *arg = argv[argIdx];
      bool   isArgValid = false;

      Option opt;
      char   *optVal;

      for (size_t optIdx = 0; optIdx < optsLength; optIdx++) {
         opt = opts[optIdx];

         bool isMatchShort = strcmp(arg, opt.short_) == 0;
         bool isMatchLong  = strcmp(arg, opt.long_) == 0;
         bool hasNext      = argIdx + opt.isReadNextVal < uargc;

         // if the argument match and next argument is available and required
         if ((isMatchShort || isMatchLong) && hasNext) {
            isArgValid = !(mode & opt.fillMode);
            optVal     = opt.isReadNextVal ? argv[argIdx + 1] : NULL;

            // option (current) and value (next), if value is required, we add 1
            argIdx += opt.isReadNextVal;
            break;
         }
      }

      // if there is no match we assume that this is an input
      if (!isArgValid) {
         if (mode & 0b10) {
            exitInvalidArgs();
         }

         input     = arg;
         inputSize = strlen(arg);
         mode |= 0b10;
         continue;
      }

      switch (opt.short_[1]) {
         case 'e':
            isEncode = true;
            break;
         case 'd':
            isEncode = false;
            break;
         case 'f':
            inputFilename = optVal;
            break;
         case 'o':
            outputFilename = optVal;
            break;
      }

      mode |= opt.fillMode;
   }

   // we need a valid input to continue
   if (!(mode & 0b10)) {
      exitInvalidArgs();
   }

   /* ========== MAIN PROCESS FLOW ========== */

   // read input file if provided
   if (inputFilename != NULL) {
      FILE *fptr = fopen(inputFilename, "rb");
      if (fptr == NULL) {
         failureExit("ERROR: An error occured when opening the file.");
      }

      fseek(fptr, 0, SEEK_END);
      inputSize = ftell(fptr);
      fseek(fptr, 0, SEEK_SET);

      input = malloc2(inputSize);
      fread(input, 1, inputSize, fptr);
      fclose(fptr);
   }

   // encode/decode process
   size_t outputSize;
   char   *output = isEncode ?
      base64Encode(input, inputSize, &outputSize)
      : base64Decode(input, inputSize + 1, &outputSize);

   // write to a file if asked
   if (outputFilename != NULL) {
      FILE *fptr = fopen(outputFilename, "wb");
      if (fptr == NULL) {
         failureExit("ERROR: An error occured when creating the file.");
      }
      fwrite(output, 1, outputSize, fptr);
      fclose(fptr);
   }

   else {
      puts(output);
   }

   if (inputFilename != NULL) {
      free(input);
   }
   free(output);

   return 0;
}