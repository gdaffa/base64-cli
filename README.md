# Base64 CLI

CLI program to convert a text to base64 format. For now only the encoder, the
decoder and file support are still in development.

## Prerequisites / Installation

Install the C/C++ compiler to your computer, either GCC or LLVM, and clone this repository to your computer.

## Usage

After cloning this repository, compile the program with these command:

Example with GCC:
```bash
gcc src/main.c -o base64
```

> [!TIP]
> To optimize the peformance consider using -O flag.

And then run the program with these command:

Example in Linux:
```
./base64 <text>
```

## Example

Command line:
```
gcc src/main.c -o base64
./base64 "Hello World"
```

Output:
```
SGVsbG8gV29ybGQ=
```

> The program ignore the null terminator, so the output only shows text without
> including the null terminator.