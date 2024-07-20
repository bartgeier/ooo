# OOO

> [!WARNING]
> This software is unfinished. Keep your expectations low.

## Build ooo from Source

### Linux

```console
$ cc -o nob nob.c # ONLY ONCE!!!
$ ./nob
```

### Windows 

#### Compiler
WinLibs standalone build of GCC and MinGW-w64 for Windows\
https://winlibs.com/

```console
$ gcc -o nob nob.c # ONLY ONCE!!!
$ ./nob
```

## Nob
Build system from Tsoding\
https://youtu.be/eRt7vhosgKE?si=ECJcNhP0EiBCh3Ib \
https://youtu.be/l9_TUMZSewo?si=gic5P1pCdEyIOB0Y \
https://youtu.be/wH963jJ1lRM?si=8i0S8ofyEtxvPAvk \
I made some modifications to nob.h for my needs.

### ./nob
If ./nob is executed it will download unzip and build:
- tree-sitter 
- tree-sitter-c 
- google unittest 

Next it will create the build directory with the executables:
+ ooo_copy_treesitter_symbols (generates ooo_treesitter_symbols_ids.h see /tree-sitter-c)
+ ooo
+ otest

### ./nob clean
Deletes all the downloads and builds.


## Examples

```console
$ cd examples
$ gcc -o style style.c # ONLY ONCE!!!
$ ./style
```

Files in examples/source are formatted and saved in examples/styled_source.
- donut.c is from => https://www.a1k0n.net/2006/09/15/obfuscated-c-donut.html
- conway.c is from => https://github.com/tsoding/conway/blob/main/conway.c
- arr.c is my test file.

donut.c and conway.c can be compiled and executed
```console
$ gcc -o donut donut.c -lm
$ ./donut
```
```console
$ gcc -o conway conway.c
$ ./conway
```



