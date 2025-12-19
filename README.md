# Base64 CLI

CLI program to encode and decode base64 format. For file support, to
encode/decode a file are still in develop.

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

The program has 2 different arguments:
- `-e <raw>`: Encode the text into base64 text, excluding the null terminator.
- `-d <encoded>`: Decode base64 text into regular text.

The program requires either `-e` option, or `-d` option.

## Example

Command line:
```
gcc -O3 src/main.c -o b64
./b64 -e "Hello World"
```

Output:
```
SGVsbG8gV29ybGQ=
```

> [!TIP]
> The result can be stored to a file using `>` operator in the terminal. This
> trick should work on all OS, including Windows.