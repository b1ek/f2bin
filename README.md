# f2bin
A small command line utility to convert a file to C code. Works on both Windows and Linux

Building:
```
gcc program.c -o f2bin -s -Os
# mv f2bin /usr/local/bin/f2bin
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
