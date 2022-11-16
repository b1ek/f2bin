# f2bin
A small command line utility to convert a file to C code

Building:
```
gcc program.c -o f2bin -s -Os
```

Usage:
```
f2bin [args]
Args:
 -h - Display this message
 -f - Specify input file
 -o - Specify output file, otherwise will write to stdout
WARNING: THIS WILL OVERWRITE YOUR FILE!!!!!!!!!!!!!
 -v - Specify the string variable name
```
