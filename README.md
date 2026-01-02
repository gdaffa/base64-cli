# Base64 CLI

CLI program to encode and decode base64 format. This program works by read the
input, convert each input character to a binary format, and parse the input to
encoded/decoded base64 format. The program has a support to read and write to a
file with the current argument.

## Prerequisites / Installation

Install the C/C++ compiler to your computer, either GCC or LLVM, and clone this 
repository to your computer.

## Usage

After cloning this repository, compile the program with these command:

Example with GCC:
```bash
gcc src/main.c -o b64
```

> [!TIP]
> To optimize the peformance consider using -O flag.

And run the program with this command:
```bash
b64 [<options>] <input>
# or
b64 [<options>] --file <inputfile>
```

The program has 4 different options:
- `-e --encode`: Encode the text into base64 text, excluding the null
                 terminator.
- `-d --decode`: Decode base64 text into regular text.
- `-f --file <inputfile>`: Read the input from `<inputfile>` file.
- `-o --output <outputfile>`: Write the result to `<outputfile>` file.

If the input file is not provided, the program read the input from the
arguments. If the output file is not provided, the program will print the result
to the terminal.

## Example

Command line:
```bash
./b64 -e "Hello World"
```

Output:
```
SGVsbG8gV29ybGQ=
```

## Future

The program is much slower compared to `base64` CLI in linux. So maybe I will
improve the encode/decode process by batch process and loop unrolling.

## License

The source code licensed under MIT license, that can be found in
[LICENSE](./LICENSE) file.